// Fill out your copyright notice in the Description page of Project Settings.


#include "DlEquipmentManagerComponent.h"
#include "DlEquipmentInstance.h"
#include "DlEquipmentDefinition.h"

UDlEquipmentInstance* FDlEquipmentList::AddEntry(TSubclassOf<UDlEquipmentDefinition> EquipmentDefinition)
{
	UDlEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	// EquipmentDefinition의 멤버 변수들은 EditDefaultsOnly로 정의되어 있어 GetDefault로 들고 와도 우리에게 필요한 것들이 모두 들어있다
	const UDlEquipmentDefinition* EquipmentCDO = GetDefault<UDlEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UDlEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UDlEquipmentInstance::StaticClass();
	}

	//Entries에 추가
	FDlAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UDlEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FDlEquipmentList::RemoveEntry(UDlEquipmentInstance* Instance)
{
	// 단순 순회하면서 삭제 (많지 않아서)
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDlAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

UDlEquipmentManagerComponent::UDlEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	EquipmentList(this)
{
}

UDlEquipmentInstance* UDlEquipmentManagerComponent::EquipItem(TSubclassOf<UDlEquipmentDefinition> EquipmentDefinition)
{
	UDlEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}
	return Result;
}

void UDlEquipmentManagerComponent::UnequipItem(UDlEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ItemInstance->OnUnEquipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}
