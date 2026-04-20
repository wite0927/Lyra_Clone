#pragma once

#include "GameplayTagContainer.h"
#include "DlGameplayTagStack.generated.h"

/**
 * Represents one stack of a gameplay tag (tag + count)
 * : for example, Ammo is representative example for GameplayTagStack
 */
USTRUCT(BlueprintType)
struct FDlGameplayTagStack
{
	GENERATED_BODY()

	FDlGameplayTagStack() {}
	FDlGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		, StackCount(InStackCount)
	{
	}

	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 StackCount = 0;
};

/** container of DlGameplayTagStack */
USTRUCT(BlueprintType)
struct FDlGameplayTagStackContainer
{
	GENERATED_BODY()

	FDlGameplayTagStackContainer() {}

	/** add/remove stack count by gameplay-tag */
	void AddStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

	/** get the count by the gameplay tag */
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	/** whether gameplay tag exists in DlGameplayTagStackContainer */
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	/** a list of gameplay tag stacks */
	UPROPERTY()
	TArray<FDlGameplayTagStack> Stacks;

	/**
	 * LUT(Look-up table) to accelerate gameplay tag stack to query [GameplayTag -> Count]
	 * - we also use this LUT to find existance for corresponding gameplay tag
	 * Array의 문제는 탐색의 비용이 높을 수 있다는 것.
	 * Tag를 들고 있으면서 Count를 빠르게 찾을 수 있게
	 * 2개로 들고 있으면서 속도를 더 빠르게
	 */
	TMap<FGameplayTag, int32> TagToCountMap;
};
