// Fill out your copyright notice in the Description page of Project Settings.


#include "DlGameplayAbility.h"

UDlGameplayAbility::UDlGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = EDlAbilityActivationPolicy::OnInputTriggered;
}
