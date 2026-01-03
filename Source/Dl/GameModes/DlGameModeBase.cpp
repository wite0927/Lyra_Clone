// Fill out your copyright notice in the Description page of Project Settings.


#include "DlGameModeBase.h"
#include "DlGameState.h"
#include "Dl/Player/DlPlayerController.h"
#include "Dl/Player/DlPlayerState.h"
#include "Dl/Character/DlCharacter.h"

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
//modular gameplay / game
void ADlGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{

}
