// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

/* DYNAMIC은 BP를 통한 UFUNCTION으로 바인딩하는 멀티케스트 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

class UDlExperienceDefinition;
class AGameStateBase;

/**
 * 
 */
UCLASS()
class DL_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

	virtual void Activate() override;

	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const UDlExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();

	/** BlueprintAssignable은 BP상에서 할당 가능한 변수로 정의한다 */
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	/** WorldPtr을 단순 로직상 캐싱하는 용도 */
	TWeakObjectPtr<UWorld> WorldPtr;
};
