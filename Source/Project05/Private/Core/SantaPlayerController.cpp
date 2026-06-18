#include "Core/SantaPlayerController.h"

#include "Core/MyGameState.h"
#include "Blueprint/UserWidget.h"

ASantaPlayerController::ASantaPlayerController()

	:HUDWidgetClass(nullptr)
{
	
}

void ASantaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this,HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
	
	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}
}

UUserWidget* ASantaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}
