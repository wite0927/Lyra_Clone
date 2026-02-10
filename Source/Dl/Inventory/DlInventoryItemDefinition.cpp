// Fill out your copyright notice in the Description page of Project Settings.


#include "DlInventoryItemDefinition.h"

UDlInventoryItemDefinition::UDlInventoryItemDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UDlInventoryItemFragment* UDlInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UDlInventoryItemFragment> FragmentClass) const
{
	return nullptr;
}
