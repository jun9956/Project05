#include "SlowingItem.h"
#include "Characters/SantaCharacter.h"

ASlowingItem::ASlowingItem()
{
	ItemType = "Slowing";
	DebuffDuration = 5.0f;
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASantaCharacter* PlayerCharacter = Cast<ASantaCharacter>(Activator))
		{
			PlayerCharacter->ApplySlowDebuff(DebuffDuration);
		}
	}
	
	DestroyItem();
}
