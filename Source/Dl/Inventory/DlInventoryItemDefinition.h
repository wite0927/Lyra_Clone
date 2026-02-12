// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DlInventoryItemDefinition.generated.h"

/**
 * ItemComponent 같은 느낌
 * Lyra에서는 장착(권총)/소비(물약) 로 나눔
 * 1회용 권총이 생기면 -> 장착 + 소비
 * - Lyra에서 사용하는 예시
 *   - ULyraInventoryFragment_EquippableItem은 EquipmentItemDefinition을 가지고 있으며, 장착 가능한 아이템을 의미한다
 *   - ULyraInventoryFramgent_SetStats는 아이템에 대한 정보를 가지고 있다
 *     - Rifle에 대한 SetStats으로 총알(Ammo)에 대한 장착 최대치와 현재 남은 잔탄 수를 예시로 들 수 있다
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class UDlInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:

};

/**
 * 
 */
UCLASS(Blueprintable)
class DL_API UDlInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UDlInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UDlInventoryItemFragment* FindFragmentByClass(TSubclassOf<UDlInventoryItemFragment> FragmentClass) const;

	/** Inventory Item 정의(메타) 이름 **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	/** Inventory Item의 Component를 Fragment로 인식하면 된다 */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<UDlInventoryItemFragment>> Fragments;
};
