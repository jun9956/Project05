#include "Core/MyGameMode.h"

#include "Core/MyGameState.h"
#include "Core/SantaPlayerController.h"
#include "Characters/SantaCharacter.h"


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ASantaCharacter::StaticClass();
	PlayerControllerClass = ASantaPlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}
