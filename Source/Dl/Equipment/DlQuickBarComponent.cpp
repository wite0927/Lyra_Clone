// Fill out your copyright notice in the Description page of Project Settings.

#include "DlQuickBarComponent.h"
#include "DlEquipmentManagerComponent.h"
#include "DlEquipmentInstance.h"
#include "DlEquipmentDefinition.h"
#include "Dl/Inventory/DlInventoryFragment_EquippableItem.h"
#include "Dl/Inventory/DlInventoryItemInstance.h"

UDlQuickBarComponent::UDlQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDlQuickBarComponent::BeginPlay()
{
	// NumSlots에 따라 Slots를 미리 할당
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
}

UDlEquipmentManagerComponent* UDlQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UDlEquipmentManagerComponent>();
		}
	}

	return nullptr;
}

void UDlQuickBarComponent::UnequipItemInSlot()
{
	if (UDlEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			EquipmentManager->UnequipItem(EquippedItem);

			EquippedItem = nullptr;
		}
	}
}

void UDlQuickBarComponent::EquipItemSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	// 현재 활성화된 ActiveSlotIndex를 활용하여 활성화된 InventoryItemInstance를 찾는다
	if (UDlInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		// Slot Item을 통해 (InventoryItemInstance) InventoryFragment_EquippableItem의 Fragment를 찾는다:
		// - 찾는 것이 실패했다면, 장착할 수 없는 Inventory Item임을 의미한다
		if (const UDlInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UDlInventoryFragment_EquippableItem>())
		{
			// EquippableItem에서 EquipmentDefinition을 찾는다:
			// - EquipmentDefinition이 있어야, 장착할 수 있다
			TSubclassOf<UDlEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				// 아래는 Unequip이랑 비슷하게 EquipmentManager를 통해 장착한다
				if (UDlEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);

					if (EquippedItem)
					{
						EquippedItem->Instigator = SlotItem;
					}
				}
			}

		}
	}
}

void UDlQuickBarComponent::AddItemToSlot(int32 SlotIndex, UDlInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void UDlQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		// UnequipItem/EquipItem을 통해, NewIndex를 통해 할당된 Item을 창착 및 업데이트를 진행
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemSlot();
	}
}
