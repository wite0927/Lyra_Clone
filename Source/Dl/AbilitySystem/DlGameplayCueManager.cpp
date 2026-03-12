// Fill out your copyright notice in the Description page of Project Settings.


#include "DlGameplayCueManager.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueSet.h"
#include "Engine/AssetManager.h"

UDlGameplayCueManager::UDlGameplayCueManager(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

UDlGameplayCueManager* UDlGameplayCueManager::Get()
{
	return Cast<UDlGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}

const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateCliend = FName(TEXT("Client"));

void UDlGameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
	TArray<FSoftObjectPath> CuePaths;
	UGameplayCueSet* RuntimeGameplayCueSet = GetRuntimeCueSet();
	if (RuntimeGameplayCueSet)
	{
		RuntimeGameplayCueSet->GetSoftObjectPaths(CuePaths);
	}

	// 새로 추가된 에셋의 경로를 CuePaths에 추가
	FAssetBundleData BundleData;
	BundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateCliend, CuePaths);

	// PrimaryAssetId를 고정된 이름으로 설정하여 (const로 설정되어 있음)
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType, UFortAssetManager_GameplayCueRefsName);

	// 에셋 매니저에 추가 진행
	UAssetManager::Get().AddDynamicAsset(PrimaryAssetId, FSoftObjectPath(), BundleData);
}
