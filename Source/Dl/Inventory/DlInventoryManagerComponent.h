// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DlInventoryManagerComponent.generated.h"

class UDlInventoryItemInstance;
class UDlInventoryItemDefinition;

/** Inventory Item 단위 객체 */
USTRUCT(BlueprintType)
struct FDlInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDlInventoryItemInstance> Instance = nullptr;
};

/** Inventory Item 관리 객체 */
USTRUCT(BlueprintType)
struct FDlInventoryList
{
	GENERATED_BODY()

	FDlInventoryList(UActorComponent* InOwnerComponent = nullptr) : OwnerComponent(InOwnerComponent)
	{
	}

	UDlInventoryItemInstance* AddEntry(TSubclassOf<UDlInventoryItemInstance> ItemDef);

	UPROPERTY()
	TArray<FDlInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * PlayerController의 Component로서 Inventory를 관리한다
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DL_API UDlInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UDlInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	FDlInventoryList InventoryList;
};
