#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/ItemInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class PROJECT05_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	
	ABaseItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|components")
	USceneComponent *Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|components")
	USphereComponent *Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|components")
	UStaticMeshComponent *StaticMesh;
	
	
	// 플레이어가 이 아이템의 범위에 들러왔을때 호출
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,      // 오버랩이 발생한 자기자신(아이템콜리전)
		AActor* OtherActor,						 // 콜리전에 부딪힌 액터(플레이어)
		UPrimitiveComponent* OtherComp,			// 캡슐컴포넌트
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	// 플레이어가 이 아이템의 범위을 벗어났을 때 호출
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	// 아이템이 사용되었을때 호출
	virtual void ActivateItem(AActor* Activator) override;
	// 이 아이템의 유형(이름 타입)을 반환
	virtual FName GetItemType() const override;
	
	virtual void DestroyItem();
	 
};
