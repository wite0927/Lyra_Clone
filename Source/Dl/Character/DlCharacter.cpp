// Fill out your copyright notice in the Description page of Project Settings.

#include "DlCharacter.h"
#include "DlPawnExtensionComponent.h"
#include "Dl/Camera/DlCameraComponent.h"

// Sets default values
ADlCharacter::ADlCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	PawnExtComponent = CreateDefaultSubobject<UDlPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent »ý¼º
	{
		CameraComponent = CreateDefaultSubobject<UDlCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}
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

