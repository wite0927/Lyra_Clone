// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "DlControllerComponent_CharacterParts.generated.h"

class UDlPawnComponent_CharacterParts;

USTRUCT()
struct FDlControllerCharacterPartEntry
{
	GENERATED_BODY()

	/* 캐릭터 파트에 대한 정의(메타 데이터 == MetaData)*/
	UPROPERTY(EditAnywhere)
	FDlCharacterPart Part;

	/*Character Part 핸들(고유값) - Controller가 소유하고 있는 Pawn에서 생성한 Character Part 핸들값*/
	FDlCharacterPartHandle Handle;
};

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class DL_API UDlControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UDlControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UDlPawnComponent_CharacterParts* GetPawnCustomizer() const;;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FDlCharacterPart& NewPart);

	void AddCharacterPartInternal(const FDlCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FDlControllerCharacterPartEntry> CharacterParts;
};
