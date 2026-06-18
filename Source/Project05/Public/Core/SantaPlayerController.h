#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SantaPlayerController.generated.h"

UCLASS()
class PROJECT05_API ASantaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	ASantaPlayerController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
};
