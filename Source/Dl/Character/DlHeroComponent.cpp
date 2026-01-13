// Fill out your copyright notice in the Description page of Project Settings.


#include "DlHeroComponent.h"

UDlHeroComponent::UDlHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	//매 틱마다 계산 안하고 델리게이트 같은 호출 방식으로 처리 -> 효율적
}
