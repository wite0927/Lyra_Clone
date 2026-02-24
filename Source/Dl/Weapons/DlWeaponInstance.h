// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dl/Cosmetics/DlCosmeticAnimationTypes.h"
#include "Dl/Equipment/DlEquipmentInstance.h"
#include "DlWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class DL_API UDlWeaponInstance : public UDlEquipmentInstance
{
	GENERATED_BODY()
public:
	UDlWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	/* Weapon에 착용/미착용 에 대한 Animation Set 정보를 들고 있다 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FDlAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FDlAnimLayerSelectionSet UnequippedAnimSet;
};
