// Fill out your copyright notice in the Description page of Project Settings.

#include "DlPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Dl/DlLogChannels.h"
#include "Dl/DlGamePlayTags.h"
#include "Dl/AbilitySystem/DlAbilitySystemComponent.h"

const FName UDlPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UDlPawnExtensionComponent::UDlPawnExtensionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	//매 틱마다 계산 안하고 델리게이트 같은 호출 방식으로 처리 -> 효율적
}

void UDlPawnExtensionComponent::SetPawnData(const UDlPawnData* InPawnData)
{
	// Pawn에 대해 Authority가 없을 경우, SetPawnData는 진행하지 않음
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	// PawnData 업데이트
	PawnData = InPawnData;
}

void UDlPawnExtensionComponent::SetupPlayerInputComponent()
{
	// 강제 업데이트로 다시 InitState 상태 변환 시작
	CheckDefaultInitialization();
}

void UDlPawnExtensionComponent::InitializeAbilitySystem(UDlAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC && InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		return;
	}

	if (AbilitySystemComponent)
	{
		UninitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvator = InASC->GetAvatarActor();
	check(!ExistingAvator);

	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);
}

void UDlPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent = nullptr;
}

void UDlPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	//올바른 Acotr에 등록되었는지 확인
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogDl, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	// GameFrameworkComponentManager에 InitState 사용을 위해 등록 진행:
	// - 등록은 상속받았던 IGameFrameworkInitStateInterface 메서드 RegisterInitStateFeature()를 활용
	// 여러 컴포넌트를 ID부여하듯이 등록
	RegisterInitStateFeature();

	//디버깅용
	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

void UDlPawnExtensionComponent::BeginPlay()
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

void UDlPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 앞서, OnRegister의 RegisterInitStateFeature()의 쌍을 맞추어주자
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UDlPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// PawnExtensionComponent 는 다른 Feature component들의 상태가 DataAvailable일 때 Sync를 맞추기 위해
		// OnActorInitStateChanged에서는 DataAvailable에 대해 지속적으로 CheckDefaultInitialization을 호출하여, 상태를 확인한다
		const FDlGameplayTags& InitTags = FDlGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UDlPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FDlGameplayTags& InitTags = FDlGameplayTags::Get();

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
		if (!PawnData)
		{
			return false;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	//DataAvailabe -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		//Actor에 바인드 된 모든 Feature(컴포넌트)들이 DataAvailable 일 때, DataInitialized로 넘어감.
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	//DataInitialized->GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UDlPawnExtensionComponent::CheckDefaultInitialization()
{
	//Actor에 바인딩 된 Feature Component들에 대해 CheckDefaultInitialzation을 호출.
	CheckDefaultInitializationForImplementers();

	const FDlGameplayTags& InitTags = FDlGameplayTags::Get();

	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	/*
	CanChangeInitState()와 HandleChangeInitState() 그리고 ChangeFeatureInitState 호출을 통한 
	OnActorInitStateChanged Delegate 호출까지 진행해준다
	*/
	ContinueInitStateChain(StateChain);
}
