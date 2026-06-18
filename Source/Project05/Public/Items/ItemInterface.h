#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT05_API IItemInterface
{
	GENERATED_BODY()

public:
	
	// 지뢰, 힐링, 코인
	// 힐링, 코인 -> 즉시 발동형 -> 오버랩
	// 지뢰 -> 범위 내에 오버랩 -> 발동이 5초뒤 폭발 -> 오버랩 -> 데미지
	
	// 플레이어가 이 아이템의 범위에 들러왔을때 호출
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,      // 오버랩이 발생한 자기자신(아이템콜리전)
		AActor* OtherActor,						 // 콜리전에 부딪힌 액터(플레이어)
		UPrimitiveComponent* OtherComp,          // 캡슐컴포넌트
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	// 플레이어가 이 아이템의 범위을 벗어났을 때 호출
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	// 아이템이 사용되었을때 호출
	virtual void ActivateItem(AActor* Activator) = 0;
	// 이 아이템의 유형(이름 타입)을 반환
	virtual FName GetItemType() const = 0;
	
};
