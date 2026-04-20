// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DlGameplayAbility.generated.h"

class UDlAbilityCost;

UENUM(BlueprintType)
enum class EDlAbilityActivationPolicy : uint8
{
	OnInputTriggered, // Input이 트리거 됐을 때. (Pressed, Released)
	WhileInputActive, // Held 됐을 때
	OnSpawn,		  // Avatar가 생성되었을 때 바로 할당 (패시브 스킬)
};

/**
 * 
 */
UCLASS(Abstract)
class DL_API UDlGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UDlGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	/* 언제 GA가 활성화 될 지 결정*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dl|AbilityActivation")
	EDlAbilityActivationPolicy ActivationPolicy;

	/** ability costs to apply DlGameplayAbility separately */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<UDlAbilityCost>> AdditionalCosts;
};
