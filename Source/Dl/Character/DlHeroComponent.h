// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "DlHeroComponent.generated.h"

/**
 * 카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리함
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class DL_API UDlHeroComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UDlHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
