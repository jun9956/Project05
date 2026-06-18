#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class PROJECT05_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
	
	virtual void ActivateItem(AActor* Activator) override;
};
