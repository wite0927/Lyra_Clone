// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlCameraMode.h"
#include "DlGameMode_ThirdPerson.generated.h"

/**
 * 
 */
UCLASS(Abstract, BluePrintable)
class DL_API UDlGameMode_ThirdPerson : public UDlCameraMode
{
	GENERATED_BODY()
public:
	UDlGameMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
