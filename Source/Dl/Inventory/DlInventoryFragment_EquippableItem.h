// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "DlInventoryFragment_EquippableItem.generated.h"

class UDlEquipmentDefinition;

/**
 * 
 */
UCLASS()
class UDlInventoryFragment_EquippableItem : public UDlInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Dl)
	TSubclassOf<UDlEquipmentDefinition> EquipmentDefinition;
};