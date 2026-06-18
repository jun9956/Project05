#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpTrapActor.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT05_API AUpTrapActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AUpTrapActor();

protected:
	
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	
private:
	// Actor의 기준점 역할을 하는 루트 컴포넌트
	// 실제 Mesh가 아니라 위치/회전/스케일의 기준점
	 UPROPERTY(VisibleAnywhere, Category = "Components")
	 USceneComponent* SceneRoot;

	// 함정 외형 StaticMeshComponent 
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* TrapMesh;
	
	// 함정의 시작 위치 저장
	UPROPERTY(VisibleAnywhere, Category = "Trap" )
	FVector StartLocation;
	
	// 함정이 올라오고 내려가는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap MoveMent", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 300.0f;
	
	// 함정이 이동할 최대 거리
	// ex)300이면 시작위치에서 300아래에서 숨어있다가 300만큼 올라옴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap MoveMent", meta = (AllowPrivateAccess = "true"))
	float MaxRange = 300.0f;
	
	// 위, 아래 끝에 도착했을때 잠깐 멈추는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap MoveMent", meta = (AllowPrivateAccess = "true"))
	float WaitTime = 1.0f;
	
	// 게임 시작시 숨길건지 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap MoveMent", meta = (AllowPrivateAccess = "true"))
	bool bStartHidden = true;
	
	// 위로 이도중인지 확인하는 변수 
	bool bMovingUp = true;
	
	// 현재 대기중인지 확인하는 변수
	bool bWaiting = false;
	
	// 현재 얼마나 이동했는지 저장하는변수 
	float CurrentDistance = 0.0f;
	
	// 대기 시간 누적 타이머
	float WaitTimer = 0.0f;
	
	// 충돌을 담당하는 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionBox;
	
	
	
};
