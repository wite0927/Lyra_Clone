// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	/* Lyra의 AnimBp에서 사용되는 값 */
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
