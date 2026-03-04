// Fill out your copyright notice in the Description page of Project Settings.

#include "DlPlayerController.h"
#include "DlPlayerState.h"
#include "Dl/AbilitySystem/DlAbilitySystemComponent.h"
#include "Dl/Camera/DlPlayerCameraManager.h"

ADlPlayerController::ADlPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ADlPlayerCameraManager::StaticClass();
}

void ADlPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UDlAbilitySystemComponent* DlASC = GetDlAbilitySystemComponent())
	{
		DlASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

ADlPlayerState* ADlPlayerController::GetDlPlayerState() const
{
	return CastChecked<ADlPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UDlAbilitySystemComponent* ADlPlayerController::GetDlAbilitySystemComponent() const
{
	const ADlPlayerState* DlPS = GetDlPlayerState();
	return (DlPS ? DlPS->GetDlAblilitySystemComponent() : nullptr);
}
