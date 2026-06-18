#pragma once

#include "CoreMinimal.h"
#include "Items/CoinItem.h"
#include "SmallCoinItem.generated.h"

UCLASS()
class PROJECT05_API ASmallCoinItem : public ACoinItem
{
	GENERATED_BODY()
	
public:
	ASmallCoinItem();
	
	virtual void ActivateItem(AActor* Activator) override;
};
