// Fill out your copyright notice in the Description page of Project Settings.

#include "DlExperienceManagerComponent.h"
#include "GameFeaturesSubsystemSettings.h"
#include "DlExperienceDefinition.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "DlExperienceActionSet.h"
#include "Dl/System/DlAssetManager.h"

void UDlExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnDlExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		//로딩이 됐으면 바로 실행
		Delegate.ExecuteIfBound(CurrentExperience);
	}
	else
	{
		//안됐으면 이 때 델리게이트에 추가
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void UDlExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceID)
{
	UDlAssetManager& AssetManager = UDlAssetManager::Get();

	TSubclassOf<UDlExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceID);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	const UDlExperienceDefinition* Experience = GetDefault<UDlExperienceDefinition>(AssetClass); // CDO
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void UDlExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == EDlExperienceLoadState::Unloaded);

	LoadState = EDlExperienceLoadState::Loading;

	UDlAssetManager& AssetManager = UDlAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	TArray<FName> BundlesToLoad;
	{
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}
}

void UDlExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	check(LoadState == EDlExperienceLoadState::Loading);
	check(CurrentExperience);

	// 활성화 된 플러그인 URL 클리어
	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
		{
			// FeaturePluginList를 순회하며, PluginURL을 ExperienceManagerComponent의 GameFeaturePluginURLS에 추가
			for (const FString& PluginName : FeaturePluginList)
			{
				FString PluginURL;
				if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
				{
					This->GameFeaturePluginURLs.AddUnique(PluginURL);
				}
			}
		};

	// GameFeautresToEnable에 잇는 Plugin만 일단 활성화할 GameFeature Plugin 후보군으로 등록
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	// GameFeaturePluginURLs에 등록된 plugin 을 로딩 및 활성화
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = EDlExperienceLoadState::LoadingGameFeature;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			// 매 Plugin이 로딩 및 활성화 이후, OnGameFeaturePluginLoadComplete 콜백 함수 등록
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		// 해당 함수가 불리는 것은 앞서 보았던 StreamableDelegateDelayHelper에 의해 불림
		OnExperienceFullLoadCompleted();		
	}
}

void UDlExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	// 매 GameFeature Plugin이 로딩될 때, 해당 함수가 콜백으로 불린다
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		// GameFeaturePlugin 로딩이 다 끝났을 경우, 기존대로 Loaded로서, OnExperienceFullLoadCompleted 호출한다
		// GameFeaturePlugin 로딩과 활성화가 끝났다면? UGameFeatureAction을 활성화해야겠지 (조금만 있다가 하장)
		OnExperienceFullLoadCompleted();
	}
}

void UDlExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != EDlExperienceLoadState::Loaded);

	// GameFeature Plugin의 로딩과 활성화 이후, GameFeature Action들을 활성화
	{
		LoadState = EDlExperienceLoadState::ExecutingActions;

		// GameFeatureAction 활성화를 위한 Context 준비
		FGameFeatureActivatingContext Context;
		{
			const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
			if (ExistingWorldContext)
			{
				Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
			}
		}

		auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
			{
				for (UGameFeatureAction* Action : ActionList)
				{
					if (Action)
					{
						Action->OnGameFeatureRegistering();
						Action->OnGameFeatureLoading();
						Action->OnGameFeatureActivating(Context);
					}
				}
			};

		// 1. Experience의 Actions
		ActivateListOfActions(CurrentExperience->Actions);

		// 1. Experience의 ActionSets
		for (const TObjectPtr<UDlExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}

	LoadState = EDlExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UDlExperienceDefinition* UDlExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == EDlExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
