// Fill out your copyright notice in the Description page of Project Settings.


#include "DlExperienceManagerComponent.h"

void UDlExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnDlExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		//로딩이 됐으면 바로 실행
		Delegate.ExecuteIfBound(CurrentExperience);
	}
	else
	{
		//안됐으면 이 때 델리게이트에 추가
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}