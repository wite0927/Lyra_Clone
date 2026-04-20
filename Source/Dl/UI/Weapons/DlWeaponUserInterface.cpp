// Fill out your copyright notice in the Description page of Project Settings.

#include "DlWeaponUserInterface.h"
#include "Dl/Equipment/DlEquipmentManagerComponent.h"
#include "Dl/Weapons/DlWeaponInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(DlWeaponUserInterface)

UDlWeaponUserInterface::UDlWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDlWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Pawn을 가져오고
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		// EquipmentManaterComponent를 활용하여, WeaponInstance를 가져오자
		if (UDlEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UDlEquipmentManagerComponent>())
		{
			if (UDlWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UDlWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					// 새로 업데이트해주고, OnWeaponChanged 호출 진행
					UDlWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}