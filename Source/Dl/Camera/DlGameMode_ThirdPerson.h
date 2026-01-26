// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlCameraMode.h"
#include "DlGameMode_ThirdPerson.generated.h"

class UCurveVector;

/**
 * 
 */
UCLASS(Abstract, BluePrintable)
class DL_API UDlGameMode_ThirdPerson : public UDlCameraMode
{
	GENERATED_BODY()
public:
	UDlGameMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* UDlCameraMode's interface
	*/
	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
