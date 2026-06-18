#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomTrapSpawner.generated.h"

UCLASS()
class PROJECT05_API ARandomTrapSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARandomTrapSpawner();

protected:
	
	virtual void BeginPlay() override;
	
	// 스폰할 함정 클래스 배열
	// 블루프린트에서 추가가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<TSubclassOf<AActor>> TrapClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int SpawnCount = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnRangeX = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnRangeY = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnHeight = 0.0f;
	
	// 랜덤 함정 생성 함수
	void SpawnRandomTraps();
	
	
	
	
// public:	
// 	
// 	virtual void Tick(float DeltaTime) override;

};
