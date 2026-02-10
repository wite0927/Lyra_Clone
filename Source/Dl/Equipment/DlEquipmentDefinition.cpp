// Fill out your copyright notice in the Description page of Project Settings.

#include "DlEquipmentDefinition.h"
#include "DlEquipmentInstance.h"

UDlEquipmentDefinition::UDlEquipmentDefinition(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	//기본값 설정
	InstanceType = UDlEquipmentInstance::StaticClass();
}
