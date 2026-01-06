// Fill out your copyright notice in the Description page of Project Settings.

#include "DlGameModeBase.h"
#include "DlExperienceManagerComponent.h"
#include "DlExperienceDefinition.h"
#include "Dl/DlLogChannels.h"
#include "DlGameState.h"
#include "Dl/Player/DlPlayerController.h"
#include "Dl/Player/DlPlayerState.h"
#include "Dl/Character/DlCharacter.h"
#include "Dl/Character/DlPawnData.h"

ADlGameModeBase::ADlGameModeBase()
{
	// Set default classes for the game mode
	PlayerControllerClass = ADlPlayerController::StaticClass();
	PlayerStateClass = ADlPlayerState::StaticClass();
	GameStateClass = ADlGameState::StaticClass();
	DefaultPawnClass = ADlCharacter::StaticClass();
	//생성자에서 게임 모드 기본값 지정
	//게임에서 여러 캐릭터들을 어떻게 하나의 기본값에서 파생시킬지
}

void ADlGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//다음 프레임에 실행해라
	//Experience가 아직 생성안되어있음.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ADlGameModeBase::InitGameState()
{
	Super::InitGameState();
	//Experience 비동기 로딩을 위한 Delegate를 준비한다:
	UDlExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UDlExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnDlExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ADlGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UDlPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ADlGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ADlGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	UE_LOG(LogDl, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called!"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

//modular gameplay / game
void ADlGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	// 우리가 로딩할 Experience에 대해 PrmaryAssetId를 생성하여 OnMatchAssignmentGiven에 넘겨준다

	FPrimaryAssetId ExperienceId;

	UWorld* World = GetWorld();

	//일단 기본 옵션으로 Default하게 B_DlDefaultExperience로 설정해놓음
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("DlExperienceDefinition"), FName("B_DlDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void ADlGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExpereieceId)
{
	//ExperienceManagerComponent을 활용하여 Experience을 로딩하기 위해, ExperienceManagerComponent의 ServerSetCurrentExperience를 호출한다
	check(ExpereieceId.IsValid());

	UDlExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UDlExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExpereieceId);
}

void ADlGameModeBase::OnExperienceLoaded(const UDlExperienceDefinition* CurrentExperience)
{
	//PlayerController 순회
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		//PlayerController가 Pawn을 Possess 하지 않으면, RestartPlayer를 통해 Pawn을 다시 스폰
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool ADlGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	UDlExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UDlExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

const UDlPawnData* ADlGameModeBase::GetPawnDataForController(const AController* InController) const
{
	if (InController)
	{
		if (const ADlPlayerState* DlPS = InController->GetPlayerState<ADlPlayerState>())
		{
			if (const UDlPawnData* PawnData = DlPS->GetPawnData<UDlPawnData>())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	UDlExperienceManagerComponent* ExperienceManagerComponenet = GameState->FindComponentByClass<UDlExperienceManagerComponent>();
	check(ExperienceManagerComponenet);

	if (ExperienceManagerComponenet->IsExperienceLoaded())
	{
		const UDlExperienceDefinition* Experience = ExperienceManagerComponenet->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}
	return nullptr;
}
