// Fill out your copyright notice in the Description page of Project Settings.

#include "DlPawnComponent_CharacterParts.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"

UDlPawnComponent_CharacterParts::UDlPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, CharacterPartList(this)
{
}

USkeletalMeshComponent* UDlPawnComponent_CharacterParts::GetParentMeshComponent() const
{
    if (AActor* OwnerActor = GetOwner())
    {
        if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
        {
            if (USkeletalMeshComponent* MeshComp = OwningCharacter->GetMesh())
            {
                return MeshComp;
            }
        }
    }
    return nullptr;
}

USceneComponent* UDlPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
    if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
    {
        return MeshComponent;
    }

    if (AActor* OwnerActor = GetOwner())
    {
        return OwnerActor->GetRootComponent();
    }

    return nullptr;
}

FDlCharacterPartHandle UDlPawnComponent_CharacterParts::AddCharacterPart(const FDlCharacterPart& NewPart)
{
    return CharacterPartList.AddEntry(NewPart);
}

void UDlPawnComponent_CharacterParts::RemoveCharacterPart(FDlCharacterPartHandle Handle)
{
    CharacterPartList.RemoveEntry(Handle);
}

FGameplayTagContainer UDlPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
    // 현재 장착된 CharacterPartList의 Merge된 Tags를 반환한다
    FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
    if (RequiredPrefix.IsValid())
    {
        // 만약 GameplayTag를 통해 필터링할 경우, 필터링해서 진행한다
        return Result.Filter(FGameplayTagContainer(RequiredPrefix));
    }
    else
    {
        // 필터링할 GameplayTag가 없으면 그냥 반환
        return Result;
    }
}

void UDlPawnComponent_CharacterParts::BroadcastChanged()
{
    const bool bReinitPose = true;

    if (USkeletalMeshComponent* MeshComp = GetParentMeshComponent())
    {
        const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
        USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

        MeshComp->SetSkeletalMesh(DesiredMesh, bReinitPose);

        if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
        {
            MeshComp->SetPhysicsAsset(PhysicsAsset, bReinitPose);
        }
    }
}

bool FDlCharacterPartList::SpawnActorForEntry(FDlAppliedCharacterPartEntry& Entry)
{
    bool bCreatedAnyActor = false;

    // 전달된 AppliedCharacterPartEntry의 Part Class가 제대로 세팅되어 있다면
    if (Entry.Part.PartClass != nullptr)
    {
        // OwnerComponent의 Owner에 속한 World를 반환
        UWorld* World = OwnerComponent->GetWorld();

        // DlPawnComponent_CharacterParts에 어느 Component에 붙일 것인지 결정한다: 
        // - GetSceneComponentToAttachTo
        if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
        {
            // 붙일 Component인 ComponentToAttachTo의 Bone 혹은 SocketName을 통해 어디에 붙일지 Transform을 계산한다
            const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

            // 우리는 Actor-Actor의 결합이므로, ChildActorComponent를 활용한다
            UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
            PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
            PartComponent->SetChildActorClass(Entry.Part.PartClass);
            // RegisterComponent를 통해 마지막으로 RenderWorld인 FScene에 변경 내용을 전달
            PartComponent->RegisterComponent();

            // ChildActorComponent에서 생성한 Actor를 반환하여
            if (AActor* SpawnedActor = PartComponent->GetChildActor())
            {
                //해당 Actor가 Parent인 HakPawnComponent_CharacterParts의 Owner Actor보다 먼저 Tick이 실행되지 않도록 선행조건을 붙인다
                if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
                {
                    SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
                }
            }

            Entry.SpawnedComponent = PartComponent;
            bCreatedAnyActor = true;
        }
    }

    return bCreatedAnyActor;
}

void FDlCharacterPartList::DestroyActorForEntry(FDlAppliedCharacterPartEntry& Entry)
{
    if (Entry.SpawnedComponent)
    {
        Entry.SpawnedComponent->DestroyComponent();
        Entry.SpawnedComponent = nullptr;
    }
}

FDlCharacterPartHandle FDlCharacterPartList::AddEntry(FDlCharacterPart NewPart)
{
    // PawnComponent의 CharacterPartList가 PartHandle를 관리하고, 이를 ControllerComponent_CharacterParts에 전달
    FDlCharacterPartHandle Result;
    Result.PartHandle = PartHandleCounter++;

    // Authority가 있다면, AppliedCharacterPartEntry를 Entries에 추가
    if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
    {
        FDlAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
        NewEntry.Part = NewPart;
        NewEntry.PartHandle = Result.PartHandle;

        // 여기서 실제 Actor를 생성하고, OwnerComponent의 Owner Actor에 Actor끼리 RootComponent로 Attach 시킨다
        if (SpawnActorForEntry(NewEntry))
        {
            // BroadcastChanged를 통해, OwnerComponent에서 Owner의 SkeletalMeshComponent를 활용하여,
            // Animation 및 Physics를 Re-initialize해준다
            OwnerComponent->BroadcastChanged();
        }
    }

    return Result;
}

void FDlCharacterPartList::RemoveEntry(FDlCharacterPartHandle Handle)
{
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FDlAppliedCharacterPartEntry& Entry = *EntryIt;

        // 제거할 경우, PartHandle을 활용한다
        if (Entry.PartHandle == Handle.PartHandle)
        {
            DestroyActorForEntry(Entry);
        }
    }
}

FGameplayTagContainer FDlCharacterPartList::CollectCombinedTags() const
{
    FGameplayTagContainer Result;

    for (const FDlAppliedCharacterPartEntry& Entry : Entries)
    {
        if (Entry.SpawnedComponent)
        {
            if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent))
            {
                TagInterface->GetOwnedGameplayTags(Result);
            }
        }
    }

    return Result;
}
