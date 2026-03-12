// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "DlGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class DL_API UDlGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
public:
	UDlGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UDlGameplayCueManager* Get();

	void RefreshGameplayCuePrimaryAsset();
};
