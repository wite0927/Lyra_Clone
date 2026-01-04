// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DlPlayerState.generated.h"

class UDlExperienceDefinition;
class UDlPawnData;
/**
 * 
 */
UCLASS()
class DL_API ADlPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() final;

	void OnExperienceLoaded(const UDlExperienceDefinition* CurrentExperience);

	UPROPERTY()
	TObjectPtr<const UDlPawnData> PawnData;
	
};
