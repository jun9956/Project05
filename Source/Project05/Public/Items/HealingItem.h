#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class PROJECT05_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AHealingItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount;
	
	virtual void ActivateItem(AActor* Activator) override;
	
};
