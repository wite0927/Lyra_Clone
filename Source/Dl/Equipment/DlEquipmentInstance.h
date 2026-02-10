// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DlEquipmentInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DL_API UDlEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UDlEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/** 어떤 InventoryItemInstance에 의해 활성화되었는지 (uickBarComponent에서 보게 될 예정) */
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	/** DlEquipementDefinition에 맞게 Spawn된 Actor Instance들 */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
