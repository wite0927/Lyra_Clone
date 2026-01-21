// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "DlCameraMode.h"
#include "DlCameraComponent.generated.h"

class UDlCameraModeStack;

template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UDlCameraMode>, FDlCameraModeDelegate);

/**
 * 
 */
UCLASS()
class DL_API UDlCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UDlCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UDlCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UDlCameraComponent>() : nullptr); }

	/*Member Methods*/
	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();

	/* CameraComponenet Interface */
	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	/* 카메라의 blending 기능을 지원하는 Stack*/
	UPROPERTY()
	TObjectPtr<UDlCameraModeStack> CameraModeStack;

	/*현재 CameraMode를 가져오는 Delegate */
	FDlCameraModeDelegate DetermineCameraModeDelegate;
};
