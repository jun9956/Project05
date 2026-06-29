#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "SlowingItem.generated.h"

UCLASS()
class PROJECT05_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ASlowingItem();
	
protected:
	
	// 슬로우 지속시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item|DeBuff")
	float DebuffDuration;
	
	virtual void ActivateItem(AActor* Activator) override;
	
};
