// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "DlEquipmentManagerComponent.generated.h"

class UDlEquipmentInstance;
class UDlEquipmentDefinition;

USTRUCT(BlueprintType)
struct FDlAppliedEquipmentEntry
{
	GENERATED_BODY()

	/** 장착물에 대한 메타 데이터 */
	UPROPERTY()
	TSubclassOf<UDlEquipmentDefinition> EquipmentDefinition;

	/** EquipmentDefinition을 통해 생성도니 인스턴스 */
	UPROPERTY()
	TObjectPtr<UDlEquipmentInstance> Instance = nullptr;
};

/**
 * 참고로 EquipmentInstance의 인스턴스를 Entry에서 관리하고 있다:
 * - DlEquipmentList는 생성된 객체를 관리
 */
USTRUCT(BlueprintType)
struct FDlEquipmentList
{
	GENERATED_BODY()

	FDlEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{
	}

	UDlEquipmentInstance* AddEntry(TSubclassOf<UDlEquipmentInstance> EquipmentDefinition);
	void RemoveEntry(UDlEquipmentInstance* Instance);

	/** 장착물에 대한 관리 리스트 */
	UPROPERTY()
	TArray<FDlAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * Pawn의 Component로서 장착물에 대한 관리를 담당
 */
UCLASS()
class DL_API UDlEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UDlEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	FDlEquipmentList EquipmentList;
};
