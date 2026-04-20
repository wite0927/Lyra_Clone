// Fill out your copyright notice in the Description page of Project Settings.


#include "DlExperienceDefinition.h"
#include "GameFeatureAction.h"

UDlExperienceDefinition::UDlExperienceDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

#if WITH_EDITORONLY_DATA
void UDlExperienceDefinition::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			// AddAddditionalAssetBundleData()는 UGameFeatureAction의 메서드
			// - 우리가 임의적으로 호출을 통해 AssetBundleData에 추가해준다
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif