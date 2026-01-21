// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DlPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DL_API ADlPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADlPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
