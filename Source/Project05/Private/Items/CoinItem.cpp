#include "Items/CoinItem.h"
#include "Engine/World.h"
#include "Core/MyGameState.h"
ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	// 액터가 있는지 확인 그 액터가 플레이어인지 확인 
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 점수 
		if (UWorld* World = GetWorld())
		{
			if (AMyGameState* GameState = World->GetGameState<AMyGameState>())
			{
				GameState->AddScore(PointValue);
				GameState -> OnCoinCollected();
			}
		}
		
		DestroyItem();
	}
}


