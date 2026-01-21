// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Dl/Camera/DlCameraMode.h"
#include "DlPawnData.generated.h"

/**
 * 
 */
UCLASS()
class DL_API UDlPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDlPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/* Pawn¿« Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dl|PawnData")
	TSubclassOf<APawn> PawnClass;

	/* Camera Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dl|Camera")
	TSubclassOf<UDlCameraMode> DefaultCameraMode;
};
