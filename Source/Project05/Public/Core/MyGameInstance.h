#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class PROJECT05_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
	
};
