#include "Items/ReverseControlItem.h"
#include "Characters/SantaCharacter.h"

AReverseControlItem::AReverseControlItem()
{
	ItemType = "ReverseControl";
	DebuffDuration = 5.0f;
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	// BaseItem의 공통 기능 실행
	Super::ActivateItem(Activator);

	// 플레이어 태그가 있는 액터만 조작 반전 적용
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASantaCharacter* PlayerCharacter = Cast<ASantaCharacter>(Activator))
		{
			// 플레이어에게 조작 반전 디버프 적용
			PlayerCharacter->ApplyReverseControlDebuff(DebuffDuration);
		}

		// 아이템 사용 후 제거
		DestroyItem();
	}
}
