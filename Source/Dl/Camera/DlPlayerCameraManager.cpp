// Fill out your copyright notice in the Description page of Project Settings.


#include "DlPlayerCameraManager.h"

ADlPlayerCameraManager::ADlPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = DL_CAMERA_DEFAULT_FOV;
	ViewPitchMin = DL_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = DL_CAMERA_DEFAULT_PITCH_MAX;
}
