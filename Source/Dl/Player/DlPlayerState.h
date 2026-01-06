// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DlPlayerState.generated.h"

class UDlExperienceDefinition;
class UDlPawnData;
/**
 * 
 */
UCLASS()
class DL_API ADlPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() final;

	void OnExperienceLoaded(const UDlExperienceDefinition* CurrentExperience);
	template <class T>
	const T* GetPawnData() const {
		return Cast<T>(PawnData);
	}
	void SetPawnData(const UDlPawnData* InPawnData);

	UPROPERTY()
	TObjectPtr<const UDlPawnData> PawnData;
	
};
