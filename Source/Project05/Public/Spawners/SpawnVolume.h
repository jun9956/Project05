#pragma once

#include "CoreMinimal.h"
#include "Spawners/ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT05_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Spawning")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, category = "Spawning")
	AActor* SpawnRandomItem(int32 CurrentWaveNumber);
	
	FItemSpawnRow* GetRandomItem(int32 CurrentWaveNumber) const;
	
	FItemSpawnRow* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FVector GetRandomPointInVolume() const;
};
