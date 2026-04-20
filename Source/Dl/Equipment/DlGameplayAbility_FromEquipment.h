// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dl/AbilitySystem/Abilities/DlGameplayAbility.h"
#include "DlGameplayAbility_FromEquipment.generated.h"

class UDlEquipmentInstance;
class UDlInventoryItemInstance;

/**
 * 
 */
UCLASS()
class DL_API UDlGameplayAbility_FromEquipment : public UDlGameplayAbility
{
	GENERATED_BODY()
public:
	UDlEquipmentInstance* GetAssociatedEquipment() const;
	UDlInventoryItemInstance* GetAssociatedItem() const;
};
