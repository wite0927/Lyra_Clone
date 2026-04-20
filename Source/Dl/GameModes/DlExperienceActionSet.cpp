// Fill out your copyright notice in the Description page of Project Settings.


#include "DlExperienceActionSet.h"
#include "GameFeatureAction.h"

UDlExperienceActionSet::UDlExperienceActionSet()
{
}

#if WITH_EDITORONLY_DATA
void UDlExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif