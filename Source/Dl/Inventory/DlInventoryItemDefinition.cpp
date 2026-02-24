// Fill out your copyright notice in the Description page of Project Settings.


#include "DlInventoryItemDefinition.h"

UDlInventoryItemDefinition::UDlInventoryItemDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UDlInventoryItemFragment* UDlInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UDlInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (UDlInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
