// Fill out your copyright notice in the Description page of Project Settings.

#include "DlAbilitySet.h"
#include "DlAbilitySystemComponent.h"
#include "Abilities/DlGameplayAbility.h"

void FDlAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FDlAbilitySet_GrantedHandles::TakeFromAbilitySystem(UDlAbilitySystemComponent* DlASC)
{
	if (!DlASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			DlASC->ClearAbility(Handle);
		}
	}
}

UDlAbilitySet::UDlAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDlAbilitySet::GiveToAbilitySystem(UDlAbilitySystemComponent* DlASC, FDlAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(DlASC);
	if (!DlASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FDlAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		UDlGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UDlGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = DlASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}
