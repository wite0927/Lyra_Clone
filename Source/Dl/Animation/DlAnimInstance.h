// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "DlAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DL_API UDlAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeInitializeAnimation() override;

	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

	/* Lyra의 AnimBp에서 사용되는 값 */
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;

	/*
	* GameplayTag와 AnimInstance의 속성값을 매핑해준다.
	* 
	* AnimInstance안에 있는 Bool 값이랑 GameplayTag랑 매칭
	* GameplayTag를 활성화 시키면, 매핑된 불리언 값을 자동으로 true로 바꿔줌
	*/
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
