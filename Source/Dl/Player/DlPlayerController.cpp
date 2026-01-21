// Fill out your copyright notice in the Description page of Project Settings.


#include "DlPlayerController.h"
#include "Dl/Camera/DlPlayerCameraManager.h"

ADlPlayerController::ADlPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ADlPlayerCameraManager::StaticClass();
}
