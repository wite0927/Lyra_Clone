// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DlPlayerCameraManager.generated.h"

/**
 *  Controller에 바인딩 된 cameramanager
 */
#define DL_CAMERA_DEFAULT_FOV (80.0f)
#define DL_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define DL_CAMERA_DEFAULT_PITCH_MAX (89.0f)

UCLASS()
class DL_API ADlPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ADlPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
