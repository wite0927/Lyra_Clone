// Fill out your copyright notice in the Description page of Project Settings.

#include "DlHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Dl/DlLogChannels.h"
#include "Dl/DlGamePlayTags.h"
#include "DlPawnExtensionComponent.h"
#include <Dl/Player/DlPlayerState.h>

const FName UDlHeroComponent::NAME_ActorFeatureName("Hero");

UDlHeroComponent::UDlHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	//매 틱마다 계산 안하고 델리게이트 같은 호출 방식으로 처리 -> 효율적
}

void UDlHeroComponent::OnRegister()
{
	Super::BeginPlay();

	//FeatureName에 None을 넣으면, Acotr에 등록된 Feature Compnent의 InitState를 관찰하곘다는 의미
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	/*
	InitState_Spawned로 상태 변환
	1. CanChangeInitState로 변환 가능한지 판단
	2. HandleChangeInitState로 상태 변경
	3. BindOnActorInitStateChanged로 Bind된 Delegate를 조건에 맞게 호출
	 - DlPawnExtensionComponent의 경우, 모든 Actor의 Feature 상태 변화에 대해 OnActorInitStateChanged()가 호출됨
	*/
	ensure(TryToChangeInitState(FDlGameplayTags::Get().InitState_Spawned));

	/*
	강제로 상태 업데이트를 실행
	*/
	CheckDefaultInitialization();
}

void UDlHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	//PawnExtensionComponent에 대해서 (PawnExtension Feature) OnActorInitStateChanged() 관찰하도록(Observing)
	BindOnActorInitStateChanged(UDlPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	/*
	InitState_Spawned로 초기화
	*/
	ensure(TryToChangeInitState(FDlGameplayTags::Get().InitState_Spawned));

	/*
	강제로 상태 업데이트를 실행
	*/
	CheckDefaultInitialization();
}

void UDlHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 앞서, OnRegister의 RegisterInitStateFeature()의 쌍을 맞추어주자
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UDlHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FDlGameplayTags& InitTags = FDlGameplayTags::Get();

	if (Params.FeatureName == UDlPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// DlPawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, 
		// DlHeroComponent도 DataInitialized 상태로 변경
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UDlHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FDlGameplayTags& InitTags = FDlGameplayTags::Get();
	ADlPlayerState* DlPS = GetPlayerState<ADlPlayerState>();

	//InitState_Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		//Pawn이 있으면 바로 Spawned로 넘어감
		if (Pawn)
			return true;
	}

	//Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!DlPS)
		{
			return false;
		}

		return true;
	}

	//DataAvailabe -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
		return DlPS && Manager->HasFeatureReachedInitState(Pawn, UDlPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	//DataInitialized->GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UDlHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	//Data 핸들링이 필요해서 ExtensionComponent에는 없고 여기에 있음.
}

void UDlHeroComponent::CheckDefaultInitialization()
{
	// 앞서 BindOnActorInitStateChanged에서 보았듯이 Hero Feature는 Pawn Extension Feature에 종속되어 있으므로, 
	// CheckDefaultInitializationForImplementers 호출하지 않음:

	// ContinueInitStateChain은 앞서 PawnExtComponent와 같음
	const FDlGameplayTags& InitTags = FDlGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}
