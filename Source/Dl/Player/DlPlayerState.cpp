// Fill out your copyright notice in the Description page of Project Settings.


#include "DlPlayerState.h"
#include "Dl/GameModes/DlExperienceManagerComponent.h"

void ADlPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UDlExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UDlExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnDlExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));

}

void ADlPlayerState::OnExperienceLoaded(const UDlExperienceDefinition* CurrentExperience)
{

}
