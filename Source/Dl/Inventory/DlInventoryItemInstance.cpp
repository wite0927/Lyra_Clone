// Fill out your copyright notice in the Description page of Project Settings.


#include "DlInventoryItemInstance.h"
#include "DlInventoryItemDefinition.h"

UDlInventoryItemInstance::UDlInventoryItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UDlInventoryItemFragment* UDlInventoryItemInstance::FindFragmentByClass(TSubclassOf<UDlInventoryItemFragment> FragmentClass) const
{
    if ((ItemDef != nullptr) && (FragmentClass != nullptr))
    {
        return GetDefault<UDlInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
    }
    return nullptr;
}
