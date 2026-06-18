#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicTrap.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT05_API AMagicTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMagicTrap();

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TrapMesh;
	
	// 함정이 보이는 시간
	// 이 시간이 지나면 사라진다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Trap")
	float VisibleTime = 3.0f;

	// 함정이 사라져 있는 시간
	// 이 시간이 지나면 다시 나타난다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Trap")
	float HiddenTime = 2.0f;

	// 현재 함정이 보이는 상태인지 저장
	bool bIsVisible = true;

	// 사라짐/나타남 상태 전환에 사용할 타이머 핸들
	FTimerHandle MagicTimerHandle;

	// 함정을 사라지게 하는 함수
	void HideTrap();

	// 함정을 다시 나타나게 하는 함수
	void ShowTrap();
	
	// 랜덤 이동을 사용할지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Trap|Move")
    bool bUseRandomMove = true;
    
    // 이동할 수 있는 X축 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Trap|Move")
    float MoveRangeX = 500.0f;
    
    // 이동할 수 있는 Y축 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Trap|Move")
    float MoveRangeY = 500.0f;
    
    // 처음 시작 위치 저장
    FVector StartLocation;
    
    // 랜덤 위치로 이동하는 함수
    void MoveToRandomLocation();
	
	//충돌
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionBox;
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
// public:	
// 	
// 	virtual void Tick(float DeltaTime) override;

};
