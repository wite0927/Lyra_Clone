// Fill out your copyright notice in the Description page of Project Settings.


#include "DlGameState.h"
#include "Dl/GameModes/DlExperienceManagerComponent.h"

ADlGameState::ADlGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UDlExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
