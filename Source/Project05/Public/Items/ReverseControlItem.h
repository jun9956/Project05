#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "ReverseControlItem.generated.h"

UCLASS()
class PROJECT05_API AReverseControlItem : public ABaseItem
{
	GENERATED_BODY()
	
	
public:
	AReverseControlItem();
	
protected:
	// 디버브 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item|Debuff")
	float DebuffDuration;
	
	virtual void ActivateItem(AActor* Activate) override;
};
