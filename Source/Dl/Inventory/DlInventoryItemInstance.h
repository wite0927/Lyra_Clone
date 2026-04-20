// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Dl/System/DlGameplayTagStack.h"
#include "DlInventoryItemInstance.generated.h"

class UDlInventoryItemDefinition;
class UDlInventoryItemFragment;

/**
 * 
 */
UCLASS(BlueprintType)
class DL_API UDlInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UDlInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const UDlInventoryItemFragment* FindFragmentByClass(TSubclassOf<UDlInventoryItemFragment> FragmentClass) const;

	template<typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	/** add/remove stack count to stat tag(=gameplay-tag stack) */
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	/** whether stat tag has in StatTags */
	bool HasStatTag(FGameplayTag Tag) const;

	/** get the current count of gameplay-tag stack */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	/** gameplay-tag stacks for inventory item instance */
	UPROPERTY()
	FDlGameplayTagStackContainer StatTags;

	/* Inventory Item의 인스턴스에는 무엇으로 정의되었는지 */
	UPROPERTY()
	TSubclassOf<UDlInventoryItemDefinition> ItemDef;
};
