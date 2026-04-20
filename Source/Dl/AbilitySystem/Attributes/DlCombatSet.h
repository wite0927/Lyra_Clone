// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DlAttributeSet.h"
#include "DlCombatSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UDlCombatSet : public UDlAttributeSet
{
	GENERATED_BODY()
public:
	UDlCombatSet();

	ATTRIBUTE_ACCESSORS(UDlCombatSet, BaseHeal);

	/**
	 * FGameplayAttribute가 참고하는 실제 AttributeSet에 있는 데이터이다 (float보다 해당 Struct를 사용하는 것을 추천)
	 * - Healing의 단위를 의미한다
	 * - e.g. 5.0f라면, Period당 5씩 Healing된다는 의미
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Dl|Combat")
	FGameplayAttributeData BaseHeal;
};