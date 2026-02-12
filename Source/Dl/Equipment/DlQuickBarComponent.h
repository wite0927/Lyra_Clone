// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "DlQuickBarComponent.generated.h"

class UDlInventoryItemInstance;
class UDlEquipmentInstance;

/**
 * MMORPG의 ShortCut HUD 임.
 *
 * 해당 Component는 ControllerComponent로서, PlayerController에 의해 조작계 중 하나로 생각해도 된다
 * - HUD(Slate)와 Inventory/Equipment(Gameplay)의 다리(Bridge) 역활하는 Component로 생각
 */
UCLASS()
class DL_API UDlQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	UDlQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, UDlInventoryItemInstance* Item);

	/** HUD QuickBar Slot 갯수 */
	UPROPERTY()
	int32 NumSlots = 3;

	/** HUD QuickBar Slot 리스트 */
	UPROPERTY()
	TArray<TObjectPtr<UDlInventoryItemInstance>> Slots;

	/** 현재 활성화된 Slot Index (아마 Lyra는 딱 한개만 Slot이 활성화되는가보다?) */
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	/** 현재 장착한 장비 정보 */
	UPROPERTY()
	TObjectPtr<UDlEquipmentInstance> EquippedItem;
};
