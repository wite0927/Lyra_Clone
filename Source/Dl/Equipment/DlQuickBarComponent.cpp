// Fill out your copyright notice in the Description page of Project Settings.


#include "DlQuickBarComponent.h"

UDlQuickBarComponent::UDlQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDlQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
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
