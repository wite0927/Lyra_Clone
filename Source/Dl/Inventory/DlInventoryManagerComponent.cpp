// Fill out your copyright notice in the Description page of Project Settings.

#include "DlInventoryManagerComponent.h"
#include "DlInventoryItemInstance.h"
#include "DlInventoryItemDefinition.h"

UDlInventoryItemInstance* FDlInventoryList::AddEntry(TSubclassOf<UDlInventoryItemDefinition> ItemDef)
{
	UDlInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FDlInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UDlInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	Result = NewEntry.Instance;
	return Result;
}

UDlInventoryManagerComponent::UDlInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}

UDlInventoryItemInstance* UDlInventoryManagerComponent::AddItemDefinition(TSubclassOf<UDlInventoryItemDefinition> ItemDef)
{
	UDlInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}
