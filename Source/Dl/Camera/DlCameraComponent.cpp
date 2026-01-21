// Fill out your copyright notice in the Description page of Project Settings.

#include "DlCameraComponent.h"
#include "DlCameraMode.h"

UDlCameraComponent::UDlCameraComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CameraModeStack(nullptr)
{

}

void UDlCameraComponent::OnRegister()
{
	Super::OnRegister();
	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<UDlCameraModeStack>(this);
	}
}

void UDlCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();

	FDlCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);
}

void UDlCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf<UDlCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
