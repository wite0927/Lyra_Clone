// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DlGameModeBase.generated.h"

class UDlExperienceDefinition;
/**
 * 
 */
UCLASS()
class DL_API ADlGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ADlGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;

	void HandleMatchAssignmentIfNotExpectingOne();
	void OnExperienceLoaded(const class UDlExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;
};

/*
Character를 만들어놓고 그 밑에 부품을 붙히는 방식
GameMode와 GameState는 한쌍으로 움직인다.

PlayerState는 폰 데이터를 갖고 그 안에는 APawn, InputConfig, CameraComponent 등이 붙어있다.

PlayerController -> CameraManager

Character->PawnExtensionComponent
		->CameraComponent

*/