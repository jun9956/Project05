#include "Items/HealingItem.h"
#include "Characters/SantaCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 힐 
		if (ASantaCharacter* PlayerCharacter = Cast<ASantaCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		
		DestroyItem();
	}
}
 