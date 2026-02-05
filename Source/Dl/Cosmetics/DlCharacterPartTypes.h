// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DlCharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FDlCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};


USTRUCT(BlueprintType)
struct FDlCharacterPart
{
	GENERATED_BODY()

	/* 파츠로 생성된 액터의 클래스 정보를 가지고 있음 -> Actor BP를 하나의 파츠로 생각하면 됨*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	/* 특정 Bone에 붙이기 위한 정보 (디폴트는 Root) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};