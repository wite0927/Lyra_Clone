#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "DlReticleWidgetBase.generated.h"

/** forward declarations */
class UDlWeaponInstance;
class UDlInventoryItemInstance;

UCLASS(Abstract)
class UDlReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UDlReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(UDlWeaponInstance* InWeapon);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInitialized();

	/**
	 * WeaponInstance/InventoryInstance를 상태 추적용으로 캐싱 목적
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDlWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDlInventoryItemInstance> InventoryInstance;
};