// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DlAbilitySystemComponent.generated.h"

/**
 * 어빌리티 시스템의 매니저 같은 것
 * 어디에 붙힐까?
 * Plyaer state(네트워크) OR Character(싱글)
 * 데디케이트에서는 컨트롤러가 핵심.
 */
UCLASS()
class DL_API UDlAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UDlAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
