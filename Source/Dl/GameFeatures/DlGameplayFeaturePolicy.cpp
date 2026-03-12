#include "DlGameplayFeaturePolicy.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddGameplayCuePath.h"
#include "GameFeatureData.h"
#include "GameplayCueSet.h"
#include "Dl/AbilitySystem/DlGameplayCueManager.h"

UDlGameplayFeaturePolicy::UDlGameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDlGameplayFeaturePolicy::InitGameFeatureManager()
{
	// GameFeature_AddGameplayCuePaths를 등록
	Observers.Add(NewObject<UDlGameFeature_AddGameplayCuePaths>());

	// Observers를 순회하며, GameFeaturesSubsystem에 Observers를 순회하며 등록
	UGameFeaturesSubsystem& SubSystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		SubSystem.AddObserver(Observer);
	}

	Super::InitGameFeatureManager();
}

void UDlGameplayFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();

	UGameFeaturesSubsystem& SubSystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		SubSystem.RemoveObserver(Observer);
	}
	Observers.Empty();
}

void UDlGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString PluginRootPath = TEXT("/") + PluginName;

	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
		{
			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->DirectoryPathsToAdd;

			if (UDlGameplayCueManager* GCM = UDlGameplayCueManager::Get())
			{
				UGameplayCueSet* RuntimeGameplayCueSet = GCM->GetRuntimeCueSet();
				const int32 PreInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;

				for (const FDirectoryPath& Directory : DirsToAdd)
				{
					FString MutablePath = Directory.Path;

					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);

					GCM->AddGameplayCueNotifyPath(MutablePath, false);
				}

				if (!DirsToAdd.IsEmpty())
				{
					GCM->InitializeRuntimeObjectLibrary();
				}

				const int32 PostInitialzeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
				if (PreInitializeNumCues != PostInitialzeNumCues)
				{
					GCM->RefreshGameplayCuePrimaryAsset();
				}
			}
		}
	}
}

void UDlGameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString PluginRootPath = TEXT("/") + PluginName;

	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
		{
			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->DirectoryPathsToAdd;

			if (UDlGameplayCueManager* GCM = UDlGameplayCueManager::Get())
			{
				int32 NumRemoved = 0;
				for (const FDirectoryPath& Directory : DirsToAdd)
				{
					FString MutablePath = Directory.Path;
					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
					NumRemoved += GCM->RemoveGameplayCueNotifyPath(MutablePath, false);
				}

				ensure(NumRemoved == DirsToAdd.Num());

				if (NumRemoved > 0)
				{
					GCM->InitializeRuntimeObjectLibrary();
				}
			}
		}
	}
}
