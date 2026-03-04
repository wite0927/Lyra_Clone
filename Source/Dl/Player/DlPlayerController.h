// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "DlPlayerController.generated.h"

class ADlPlayerState;
class UDlAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DL_API ADlPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ADlPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	ADlPlayerState* GetDlPlayerState() const;
	UDlAbilitySystemComponent* GetDlAbilitySystemComponent() const;
};
