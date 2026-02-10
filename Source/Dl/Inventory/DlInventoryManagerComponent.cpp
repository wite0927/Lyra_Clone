// Fill out your copyright notice in the Description page of Project Settings.

#include "DlInventoryManagerComponent.h"

UDlInventoryManagerComponent::UDlInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}
