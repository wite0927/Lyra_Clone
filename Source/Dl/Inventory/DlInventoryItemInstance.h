// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DlInventoryItemInstance.generated.h"

class UDlInventoryItemDefinition;
class UDlInventoryItemFragment;

/**
 * 
 */
UCLASS()
class DL_API UDlInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UDlInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	const UDlInventoryItemFragment* FindFragmentByClass(TSubclassOf<UDlInventoryItemFragment> FragmentClass) const;

	template<typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	/* Inventory Item의 인스턴스에는 무엇으로 정의되었는지 */
	UPROPERTY()
	TSubclassOf<UDlInventoryItemDefinition> ItemDef;
};
