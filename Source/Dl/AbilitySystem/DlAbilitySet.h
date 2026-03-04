// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"
#include "DlAbilitySet.generated.h"

class UDlAbilitySystemComponent;
class UDlGameplayAbility;

USTRUCT(BlueprintType)
struct FDlAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	/** 허용된 GameplayAbility */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDlGameplayAbility> Ability = nullptr;

	/** Input 처리를 위한 GameplayTag */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	/** Ability의 허용 조건 (Level) */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FDlAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(UDlAbilitySystemComponent* DlASC);

public:

protected:
	/*허용된 GameplayAbilitySpecHandle */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

/**
 * 
 */
UCLASS()
class DL_API UDlAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UDlAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UDlAbilitySystemComponent* DlASC, FDlAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FDlAbilitySet_GameplayAbility> GrantedGameplayAbilities;
}; 
