// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DlCameraMode.generated.h"

class UDlCameraComponent;

/*
* FDlCameraModeView
*/
struct FDlCameraModeView
{
	FDlCameraModeView();

	void Blend(const FDlCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/*
* [0,1]을 BlendFunction에 맞게 재매핑을 위한 타입
*/
UENUM(BlueprintType)
enum class EDlCameraModeBlendFunction : uint8
{
	Linear,
	EaseIn, //Exponent값에 의해 조정
	EaseOut,
	EaseInOut,
	COUNT,
};

/**
 * camera blending 대상 유닛
 */
UCLASS(Abstract, NotBlueprintable)
class DL_API UDlCameraMode : public UObject
{
	GENERATED_BODY()
public:
	UDlCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* member methods
	*/
	virtual void UpdateView(float DeltaTiem);
	void UpdateCameraMode(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	UDlCameraComponent* GetDlCameraComponent() const;
	AActor* GetTargetActor() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;



	/**
	 * member variables
	 */

	 /** CameraMode에 의해 생성된 CameraModeView */
	FDlCameraModeView View;

	/** Camera Mode의 FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	/** View에 대한 Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	/** 얼마동안 Blend를 진행할지 시간을 의미 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	/*선형적인 Blend값[0,1]*/
	float BlendAlpha;

	/*
	* CameraMode의 최종 Blend 값
	* 앞서 BlendAlpha의 선형 값을 매핑하여 최종 BlendWeight를 꼐산
	*/
	float BlendWeight;
	
	/*
	* EaseIn/Out에 사용한 Exponent
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	/* Blend Fucntion */
	EDlCameraModeBlendFunction BlendFunction;
};

/* Camera Blending을 담당하는 객체*/
UCLASS()
class UDlCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UDlCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* member methods
	*/
	UDlCameraMode* GetCameraModeInstance(TSubclassOf<UDlCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<UDlCameraMode>& CameraModeClass);
	void UpdateStack(float DeltaTime);
	void EvaluateStack(float DeltaTime, FDlCameraModeView& OutCameraModeView);
	void BlendStack(FDlCameraModeView& OutCameraModeView) const;

	/* 생성된 CameraMode를 관리 */
	UPROPERTY()
	TArray<TObjectPtr<UDlCameraMode>> CameraModeInstances;

	/*Camera Matrix Blend 업데이트 진행 큐*/
	UPROPERTY()
	TArray<TObjectPtr<UDlCameraMode>> CameraModeStack;

};
