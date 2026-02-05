// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "DlPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DL_API ADlPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ADlPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
