#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	
	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// 하드 레퍼런스
	// 클래스가 항상 메모리에 로드된 상테에서 바로 접근       //소프트는 클래스의 경로만 유지
	TSubclassOf<AActor> ItemClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance = 0.0f;
};
