#pragma once

#include "Containers/Array.h"
#include "Templates/SubclassOf.h"
#include "Dl/Inventory/DlInventoryItemDefinition.h"
#include "InventoryFragment_ReticleConfig.generated.h"

/** forward declaration */
class UDlReticleWidgetBase;

UCLASS()
class UDlInventoryFragment_ReticleConfig : public UDlInventoryItemFragment
{
	GENERATED_BODY()
public:
	/** 무기에 결합된 ReticleWidget 정보를 가지고 있는 Fragment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reticle)
	TArray<TSubclassOf<UDlReticleWidgetBase>> ReticleWidgets;
};