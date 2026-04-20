// Fill out your copyright notice in the Description page of Project Settings.

#include "DlCharacter.h"
#include "DlPawnExtensionComponent.h"
#include "Dl/AbilitySystem/DlAbilitySystemComponent.h"
#include "Dl/Camera/DlCameraComponent.h"
#include "DlHealthComponent.h"

// Sets default values
ADlCharacter::ADlCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	PawnExtComponent = CreateDefaultSubobject<UDlPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	{
		PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
		PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
	}

	// CameraComponent 생성
	{
		CameraComponent = CreateDefaultSubobject<UDlCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}

	{
		HealthComponent = CreateDefaultSubobject<UDlHealthComponent>(TEXT("HealthComponent"));
	}
}

void ADlCharacter::OnAbilitySystemInitialized()
{
	UDlAbilitySystemComponent* DlASC = Cast<UDlAbilitySystemComponent>(GetAbilitySystemComponent());
	check(DlASC);

	// HealthComponent의 ASC를 통한 초기화
	HealthComponent->InitializeWithAbilitySystem(DlASC);
}

void ADlCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeWithAbilitySystem();
}

UAbilitySystemComponent* ADlCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetDlAbilitySystemComponent();
}

// Called when the game starts or when spawned
void ADlCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADlCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

