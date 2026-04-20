// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "DlHealthComponent.generated.h"

class UDlAbilitySystemComponent;
class UDlHealthSet;
class UDlHealthComponent;
class AActor;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDlHealth_AttributeChanged, UDlHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * 
 */
UCLASS()
class DL_API UDlHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
public:
	UDlHealthComponent(const FObjectInitializer& ObjectInitializer);
	
	/** Actor(보통 ACharacter/APawn)의 HealthComponent를 반환 */
	UFUNCTION(BlueprintPure, Category = "Dl|Health")
	static UDlHealthComponent* FindHealthComponent(const AActor* Actor);

	/** 아래의 UFUNCTION은 HealthSet의 Attribute에 접근하기 위한 BP Accessor 함수들 */
	UFUNCTION(BlueprintCallable, Category = "Dl|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Dl|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Dl|Health")
	float GetHealthNormalized() const;

	/** ASC와 HealthSet 초기화 */
	void InitializeWithAbilitySystem(UDlAbilitySystemComponent* InASC);
	void UninitializeWithAbilitySystem();

	/** ASC를 통해, HealthSet의 HealthAttribute 변경이 있을때 호출하는 메서드 (내부적으로 OnHealthChanged 호출) */
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	/** HealthSet을 접근하기 위한 AbilitySystemComponent */
	UPROPERTY()
	TObjectPtr<UDlAbilitySystemComponent> AbilitySystemComponent;

	/** 캐싱된 HealthSet 레퍼런스 */
	UPROPERTY()
	TObjectPtr<const UDlHealthSet> HealthSet;

	/** health 변화에 따른 Delegate(Multicast) */
	UPROPERTY(BlueprintAssignable)
	FDlHealth_AttributeChanged OnHealthChanged;
};
