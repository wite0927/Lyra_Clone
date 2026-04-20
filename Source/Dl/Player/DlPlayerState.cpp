// Fill out your copyright notice in the Description page of Project Settings.


#include "DlPlayerState.h"
#include "Dl/GameModes/DlExperienceManagerComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Dl/AbilitySystem/DlAbilitySystemComponent.h"
#include "Dl/AbilitySystem/Attributes/DlCombatSet.h"
#include "Dl/AbilitySystem/Attributes/DlHealthSet.h"
#include "Dl/GameModes/DlGameModeBase.h"
#include "Dl/AbilitySystem/DlAbilitySet.h"
#include "Dl/Character/DlPawnData.h"

ADlPlayerState::ADlPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComp = ObjectInitializer.CreateDefaultSubobject<UDlAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));

	CreateDefaultSubobject<UDlHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UDlCombatSet>(TEXT("CombatSet"));
}

void ADlPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComp);
	{
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComp->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	// 여기서 OwnerACtor와 AvatarActor가 같은 Actor인 PlayerState를 가르킴.
	// AvatarActor는 PlayerController가 소유하는 대상인 Pawn이 되어야 함. 그래서 PawnExtensionComponenet에서 재설정
	AbilitySystemComp->InitAbilityActorInfo(this, GetPawn());

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UDlExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UDlExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnDlExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));

}

void ADlPlayerState::OnExperienceLoaded(const UDlExperienceDefinition* CurrentExperience)
{
	if (ADlGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ADlGameModeBase>())
	{
		const UDlPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ADlPlayerState::SetPawnData(const UDlPawnData* InPawnData)
{
	check(InPawnData);

	check(!PawnData);
	PawnData = InPawnData;

	for (UDlAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComp, nullptr);
		}
	}
}
