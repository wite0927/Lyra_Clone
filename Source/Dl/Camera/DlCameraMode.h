// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DlCameraMode.generated.h"

/**
 * camera blending 대상 유닛
 */
UCLASS(Abstract, NotBlueprintable)
class DL_API UDlCameraMode : public UObject
{
	GENERATED_BODY()
public:
	UDlCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};

/* Camera Blending을 담당하는 객체*/
UCLASS()
class UDlCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UDlCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/* 생성된 CameraMode를 관리 */
	UPROPERTY()
	TArray<TObjectPtr<UDlCameraMode>> CameraModeInstances;

	/*Camera Matrix Blend 업데이트 진행 큐*/
	UPROPERTY()
	TArray<TObjectPtr<UDlCameraMode>> CameraModeStack;

};
