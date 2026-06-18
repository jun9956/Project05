#include "Core/MyGameMode.h"

#include "Core/MyGameState.h"
#include "Core/SantaPlayerController.h"
#include "Characters/ProjectPawn.h"


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AProjectPawn::StaticClass();
	PlayerControllerClass = ASantaPlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}
