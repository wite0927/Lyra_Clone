// Fill out your copyright notice in the Description page of Project Settings.


#include "DlEquipmentManagerComponent.h"
#include "DlEquipmentInstance.h"
#include "DlEquipmentDefinition.h"
#include "Dl/AbilitySystem/DlAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

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

	UDlAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	{
		for (const TObjectPtr<UDlAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

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
			UDlAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

UDlAbilitySystemComponent* FDlEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();

	return Cast<UDlAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
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

TArray<UDlEquipmentInstance*> UDlEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UDlEquipmentInstance> InstanceType) const
{
	TArray<UDlEquipmentInstance*> Results;

	// EquipmentList를 순회하며
	for (const FDlAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UDlEquipmentInstance* Instance = Entry.Instance)
		{
			// InstanceType에 맞는 Class이면 Results에 추가하여 반환
			// - HakRangedWeaponInstance가 될거임
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}
