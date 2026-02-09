// Fill out your copyright notice in the Description page of Project Settings.

#include "DlControllerComponent_CharacterParts.h"
#include "DlPawnComponent_CharacterParts.h"

UDlControllerComponent_CharacterParts::UDlControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDlControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UDlControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

UDlPawnComponent_CharacterParts* UDlControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		return ControlledPawn->FindComponentByClass<UDlPawnComponent_CharacterParts>();
	}
	return nullptr;
}

void UDlControllerComponent_CharacterParts::AddCharacterPart(const FDlCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void UDlControllerComponent_CharacterParts::AddCharacterPartInternal(const FDlCharacterPart& NewPart)
{
	FDlControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef(); //복사 연산 방지
	NewEntry.Part = NewPart;

	if (UDlPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void UDlControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UDlPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FDlControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UDlControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// 이전 OldPawn에 대해서는 Character Parts를 전부 제거해주자
	if (UDlPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UDlPawnComponent_CharacterParts>() : nullptr)
	{
		for (FDlControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// 새로운 Pawn에 대해서 기존 Controller가 가지고 있는 Character Parts를 추가해주자
	if (UDlPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UDlPawnComponent_CharacterParts>() : nullptr)
	{
		for (FDlControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}
