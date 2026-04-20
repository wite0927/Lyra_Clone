// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DlInventoryItemDefinition.h"
#include "DlInventoryFragment_SetStats.generated.h"

class UDlInventoryItemInstance;

/**
 * 
 */
UCLASS()
class DL_API UDlInventoryFragment_SetStats : public UDlInventoryItemFragment
{
	GENERATED_BODY()
public:
	virtual void OnInstanceCreated(UDlInventoryItemInstance* Instance) const override;

	/** InitialItemStats gives constructor's parameters for DlGameplayTagStackContainer */
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;
};
