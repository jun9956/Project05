#include "Core/ProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"

AProjectPlayerController::AProjectPlayerController()
	: InputMappingContext(nullptr),
      MoveAction(nullptr),
      JumpAction(nullptr),
      LookAction(nullptr),
      SprintAction(nullptr)
{
	
}

void AProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
