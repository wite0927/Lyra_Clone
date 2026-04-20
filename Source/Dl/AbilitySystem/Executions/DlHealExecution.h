// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DlHealExecution.generated.h"

/**
 * UDlHealExecution은 GameplayEffect의 Execution을 사용자 정의에 따라 GameplayEffect의 처리할 수 있다:
 * - HealExecution 이름에서 알 수 있듯이, HealthAttribute의 Healing을 적용한다
 */
UCLASS()
class DL_API UDlHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDlHealExecution();

	/** 해당 메서드는 GameplayEffectExecutionCalculation의 Execute() BlueprintNativeEvent를 오버라이드 한다 */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
