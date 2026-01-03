// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "DlExperienceManagerComponent.generated.h"

class UDlExperienceDefinition;

enum class EDlExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDlExperiencedLoaded, const UDlExperienceDefinition*);

/**
 * 말 그대로, 해당 컴포넌트는 gamestate를 오너로 가지면서, experience의 상태 정보를 갖고 있는 component다.
 * 뿐만 아니라, manager라는 단어가 포함되어 있듯이, experience 로딩 상태 업데이트 및 이벤트를 관리
 */
UCLASS()
class DL_API UDlExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	bool IsExperienceLoaded() const { return (LoadState == EDlExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	void CallOrRegister_OnExperienceLoaded(const FOnDlExperiencedLoaded::FDelegate& Delegate);
public:
	UPROPERTY()
	TObjectPtr<UDlExperienceDefinition> CurrentExperience;

	/*Experience의 로딩 상태를 모니터링*/
	EDlExperienceLoadState LoadState = EDlExperienceLoadState::Unloaded;
	/*Experience 로딩 완료된 후, Boradcasting Delegate*/
	FOnDlExperiencedLoaded OnExperienceLoaded;
};
