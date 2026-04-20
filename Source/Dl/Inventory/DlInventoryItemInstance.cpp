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


void UDlInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
    StatTags.AddStack(Tag, StackCount);
}

void UDlInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
    StatTags.RemoveStack(Tag, StackCount);
}

int32 UDlInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
    return StatTags.GetStackCount(Tag);
}

bool UDlInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
    return StatTags.ContainsTag(Tag);
}