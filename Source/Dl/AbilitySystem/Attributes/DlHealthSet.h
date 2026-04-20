// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DlHealthSet.generated.h"

/**
 * 
 */
UCLASS()
class DL_API UDlHealthSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UDlHealthSet();

	/**
	 * 앞서 HakAttributeSet에서 정의했던, ATTRIBUTE_ACCESSORS를 통해, 아래 정의함 멤벼변수와 똑같이 이름을 설정한다
	 * - ATTRIBUTE_ACCESSORS의 Macro의 정의부분을 한번 실펴보자
	 */
	ATTRIBUTE_ACCESSORS(UDlHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UDlHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UDlHealthSet, Healing);

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// FGameplayAttributeData -> 이중 float , BaseValue/CurrentValue로 관리
	/** 현재 체력 */
	UPROPERTY(BlueprintReadOnly, Category = "Dl|Health")
	FGameplayAttributeData Health;

	/** 체력 최대치 */
	UPROPERTY(BlueprintReadOnly, Category = "Dl|Health")
	FGameplayAttributeData MaxHealth;

	/** 체력 회복치 */
	UPROPERTY(BlueprintReadOnly, Category = "Dl|Health")
	FGameplayAttributeData Healing;
	
};
