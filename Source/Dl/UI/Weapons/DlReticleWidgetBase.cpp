#include "DlReticleWidgetBase.h"
#include "Dl/Weapons/DlWeaponInstance.h"
#include "Dl/Inventory/DlInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(DlReticleWidgetBase)

UDlReticleWidgetBase::UDlReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDlReticleWidgetBase::InitializeFromWeapon(UDlWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;
	if (WeaponInstance)
	{
		InventoryInstance = Cast<UDlInventoryItemInstance>(WeaponInstance->GetInstigator());
	}
	OnWeaponInitialized();
}