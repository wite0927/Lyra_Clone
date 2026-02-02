// Fill out your copyright notice in the Description page of Project Settings.

#include "DlHeroComponent.h"
#include "DlPawnData.h"
#include "DlPawnExtensionComponent.h"
#include "PlayerMappableInputConfig.h"
#include "Dl/Input/DlMappableConfigPair.h"
#include "Dl/Input/DlInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Dl/DlGameplayTags.h"
#include "Dl/DlLogChannels.h"
#include "Dl/Camera/DlCameraComponent.h"
#include "Dl/Player/DlPlayerController.h"
#include "Dl/Player/DlPlayerState.h"

const FName UDlHeroComponent::NAME_ActorFeatureName("Hero");

UDlHeroComponent::UDlHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	//매 틱마다 계산 안하고 델리게이트 같은 호출 방식으로 처리 -> 효율적
}

void UDlHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인:
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogDl, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}
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
	const FDlGameplayTags& InitTags = FDlGameplayTags::Get();

	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ADlPlayerState* DlPS = GetPlayerState<ADlPlayerState>();
		if (!ensure(Pawn && DlPS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UDlPawnData* PawnData = nullptr;
		if (UDlPawnExtensionComponent* PawnExtComp = UDlPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UDlPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			if (UDlCameraComponent* CameraComponent = UDlCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &UDlHeroComponent::DetermineCameraMode);
			}
		}

		if (ADlPlayerController* DlPC = GetController<ADlPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
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

TSubclassOf<UDlCameraMode> UDlHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UDlPawnExtensionComponent* PawnExtComp = UDlPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UDlPawnData* PawnData = PawnExtComp->GetPawnData<UDlPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}
	return nullptr;
}

void UDlHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	// LocalPlayer를 가져오기 위해
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem 가져오기 위해
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP);
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem에 MappingContext를 비워준다
	Subsystem->ClearAllMappings();

	// PawnExtensionComponent -> PawnData -> InputConfig 존재 유무 판단:
	if (const UDlPawnExtensionComponent* PawnExtComp = UDlPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UDlPawnData* PawnData = PawnExtComp->GetPawnData<UDlPawnData>())
		{
			if (const UDlInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FDlGameplayTags& GameplayTags = FDlGameplayTags::Get();

				// HeroComponent 가지고 있는 Input Mapping Context를 순회하며, EnhancedInputLocalPlayerSubsystem에 추가
				for (const FDlMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 Input Mapping Context를 추가
						for (const auto ConfigObject = Pair.Config.LoadSynchronous(); const auto & MappingContextPair : ConfigObject->GetMappingContexts())
						{
							const UInputMappingContext* MappingContext = MappingContextPair.Key;
							const int32 Priority = MappingContextPair.Value; // 우선순위 값도 가져옵니다
							Subsystem->AddMappingContext(MappingContext, Priority, Options);
						}
					}
				}

				UDlInputComponent* DlIC = CastChecked<UDlInputComponent>(PlayerInputComponent);
				{
					// InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move()와 Input_LookMouse() 멤버 함수에 바인딩시킨다:
					// - 바인딩한 이후, Input 이벤트에 따라 멤버 함수가 트리거된다
					DlIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					DlIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}
}

void UDlHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			// MovementDirection은 현재 카메라의 RightVector를 의미함 (World-Space)
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

			// 내부적으로 MovementDirection * Value.X를 MovementComponent에 적용(더하기)해준다
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UDlHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		// X에는 Yaw 값
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		// Y에는 Pitch 값
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}
