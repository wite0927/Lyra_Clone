// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DlUserFacingExperience.generated.h"

/**
 * 
 */
UCLASS()
class DL_API UDlUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/** the specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	/** the gameplay expierence to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "HakExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};
