// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlCharacterPartTypes.h"
#include "DlCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "DlPawnComponent_CharacterParts.generated.h"

class UDlPawnComponent_CharacterParts;

USTRUCT()
struct FDlAppliedCharacterPartEntry
{
	GENERATED_BODY()

	/*Character Part의 정의(메타데이터)*/
	UPROPERTY()
	FDlCharacterPart Part;

	/*FDlCharacterPartList에서 할당 받은 Part 핸들값
	FDlAppliedCharacterPartEntry의 Handle값과 같아야 함 -> 같으면 같은 Part
	*/
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	/*인스턴스화 된 Cahracter Part용 Actor*/
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr; //UChildActorComponent -> Actor와 Actor를 붙힐 때 사용
};

/* DlPawnComponent_CharacterParts 에서 실질적 Character Parts를 관리하는 클래스 */
USTRUCT(BlueprintType)
struct FDlCharacterPartList
{
	GENERATED_BODY()

	FDlCharacterPartList()
		: OwnerComponent(nullptr)
	{}

	FDlCharacterPartList(UDlPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}

	bool SpawnActorForEntry(FDlAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FDlAppliedCharacterPartEntry& Entry);

	FDlCharacterPartHandle AddEntry(FDlCharacterPart NewPart);
	void RemoveEntry(FDlCharacterPartHandle Handle);

	FGameplayTagContainer CollectCombinedTags() const;

	/* 현재 인스턴스화 된 Character Part*/
	UPROPERTY()
	TArray<FDlAppliedCharacterPartEntry> Entries;

	/* 해당 DlCharacterPartList의 Owner인 PawnComponent*/
	UPROPERTY()
	TObjectPtr<UDlPawnComponent_CharacterParts> OwnerComponent;

	/*앞서 보았던 PartHandle의 값을 할당 및 관리하는 변수 */
	int32 PartHandleCounter = 0;
};

/**
 * 
 */
UCLASS()
class DL_API UDlPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	UDlPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	FDlCharacterPartHandle AddCharacterPart(const FDlCharacterPart& NewPart);
	void RemoveCharacterPart(FDlCharacterPartHandle Handle);

	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	void BroadcastChanged();

	/*인스턴스화 된 캐릭터 파츠*/
	UPROPERTY()
	FDlCharacterPartList CharacterPartList;

	/* 애니메이션 적용을 위한 메시와 연결고리 */
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FDlAnimBodyStyleSelectionSet BodyMeshes;
};
