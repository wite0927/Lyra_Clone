// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DlExperienceDefinition.generated.h"

class UDlPawnData;

/**
 * 
 */
UCLASS()
class DL_API UDlExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDlExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<UDlPawnData> DefaultPawnData;

	/**
	 * 해당 property는 단순히 마킹 및 기억용으로 남겨둔다
	 * - 게임 모드에 따른 GameFeature plugin을 로딩하는데 이에 대한 연결고리고 생각하면 된다
	 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
