// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "DlGameplayAbilityTargetData_SingleTargetHit.generated.h"

/**
* 
 */
USTRUCT()
struct FDlGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:
	FDlGameplayAbilityTargetData_SingleTargetHit()
		:CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FDlGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	/* Åº¾à ID */
	UPROPERTY()
	int32 CartridgeID;
};
