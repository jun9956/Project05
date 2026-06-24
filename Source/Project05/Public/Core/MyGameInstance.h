#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class USoundBase;
class UAudioComponent;

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
	
	// BGM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
	USoundBase* MenuBGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
	USoundBase* GameBGM;

	UPROPERTY()
	UAudioComponent* BGMComponent;

	UPROPERTY()
	USoundBase* CurrentBGM;

	UFUNCTION(BlueprintCallable, Category = "BGM")
	void PlayBGM(USoundBase* NewBGM);

	UFUNCTION(BlueprintCallable, Category = "BGM")
	void StopBGM();
	
};
