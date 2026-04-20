#pragma once

#include "DlAbilityCost.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "DlAbilityCost_ItemTagStack.generated.h"

/**
 * Represents a cost that requires expanding a quantity of a tag stack on the associated item instance
 */
UCLASS(meta = (DisplayName = "Item Tag Stack"))
class UDlAbilityCost_ItemTagStack : public UDlAbilityCost
{
	GENERATED_BODY()
public:
	UDlAbilityCost_ItemTagStack();

	virtual bool CheckCost(const UDlGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UDlGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/**
	 * how much of the tag spend:
	 * - FScalableFloat is scaled by Curve with base float value
	 * - Curve is normally indexed by ability level
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FScalableFloat Quantity;

	/** gameplay tag combined with the cost */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag Tag;

	/** failure identifier with gameplay-tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag FailureTag;
};