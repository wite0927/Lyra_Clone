// Fill out your copyright notice in the Description page of Project Settings.


#include "DlExperienceManagerComponent.h"
#include "GameFeaturesSubsystemSettings.h"
#include "DlExperienceDefinition.h"
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

	const UDlExperienceDefinition* Experience = GetDefault<UDlExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

PRAGMA_DISABLE_OPTIMIZATION
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

	// 해당 함수가 불리는 것은 앞서 보았던 StreamableDelegateDelayHelper에 의해 불림
	OnExperienceFullLoadCompleted();
}

void UDlExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != EDlExperienceLoadState::Loaded);

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
