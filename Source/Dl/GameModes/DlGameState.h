// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DlGameState.generated.h"

class UDlExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class DL_API ADlGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ADlGameState();
	
	UPROPERTY()
	TObjectPtr<UDlExperienceManagerComponent> ExperienceManagerComponent;
};
