// Fill out your copyright notice in the Description page of Project Settings.

#include "DlInventoryFragment_SetStats.h"
#include "DlInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(DlInventoryFragment_SetStats)

void UDlInventoryFragment_SetStats::OnInstanceCreated(UDlInventoryItemInstance* Instance) const
{
	// iterating InitialItemStats and add stat tag to InventoryItemInstance
	for (const auto& InitialItemStat : InitialItemStats)
	{
		Instance->AddStatTagStack(InitialItemStat.Key, InitialItemStat.Value);
	}
}