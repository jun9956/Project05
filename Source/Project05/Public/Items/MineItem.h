#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class PROJECT05_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();
	UPROPERTY()
	USphereComponent* ExplosionCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* ExplosionSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item")
	int32 ExplosionDamage;
	
	bool bHasExploded;
	FTimerHandle ExplosionTimerHandle;
	
	virtual void ActivateItem(AActor* Activator) override;
	
	void Explode();
};
