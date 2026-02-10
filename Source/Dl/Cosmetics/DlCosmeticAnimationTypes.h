// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DlCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FDlAnimLayerSelectionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FDlAnimLayerSelectionSet
{
	GENERATED_BODY()

	/* AnimInstance의 룰을 가진 LayerRules */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDlAnimLayerSelectionEntry> LayerRules;

	/* 디폴트 레이어 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultLayer;
};

USTRUCT(BlueprintType)
struct FDlAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	/* anim Layer를 적용할 스켈레탈 메시 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	/* 코스메틱 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FDlAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	/* GameplayTag를 통해 mesh rules에 따라 알맞은 BodyStyle을 변환 */
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

	/*AnimLayer를 적용할 Skeleton 메시를 들고 있음 -> 애니메이션-메시 간의 룰을 들고 있는 것*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDlAnimBodyStyleSelectionEntry> MeshRules;

	/*디폴트로 적용할 스켈레톤 메시*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	/*물리에셋은 하나로 동일 -> 모든 애니메이션의 물리 속성은 공유함 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};