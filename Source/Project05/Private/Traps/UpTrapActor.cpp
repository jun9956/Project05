#include "Traps/UpTrapActor.h"
#include "Components/BoxComponent.h"

AUpTrapActor::AUpTrapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 빈 SceneComponent를 생성하고 Actor의 RootComponent로 설정
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	// 충돌 박스 생성
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(SceneRoot);

	// 충돌 박스 크기 설정
	CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	// 충돌 활성화
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 움직이는 함정이므로 WorldDynamic으로 설정
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);

	// 기본적으로 모든 채널을 막음
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);

	
	// StaticMechComponent 생성(TrapMesh)
	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
	
	// 이 Atcor의 루트 컴포넌트를 TrapMesh로 설정
	TrapMesh->SetupAttachment(SceneRoot);
	
}

// Called when the game starts or when spawned
void AUpTrapActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 현재 레벨에 배치된 위치를 시작위치로 
	StartLocation = GetActorLocation();
	
	//시작할때 아래에 숨긴 상태일 경우 
	if (bStartHidden)
	{
		// 시작위치에서 z축으로 maxRange만큼 아래 위치 계산
		FVector HiddenLocation = StartLocation;
		HiddenLocation.Z -= MaxRange;
		
		// 함정을 아래로 위지
		SetActorLocation(HiddenLocation);
		
		// 숨어있는 상태이므로 이동거리 0
		CurrentDistance = 0.0f;
		
		//처음에는 위로 올라감
		bMovingUp = true;
	}
	else
	{
		// 숨기지 않고 배치한 위치에서 시작, 올라온 상태
		CurrentDistance = MaxRange;
		
		// 다음 이동을 아래로 
		bMovingUp = false;
	}
	
}

// Called every frame
void AUpTrapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 현재 대기중이라면 이동하지 않ㄷ고 시간만 계산
	if (bWaiting)
	{
		// deltatime을 더해서 흐른 시간 누적
		WaitTimer += DeltaTime;
		
		// 누적 시간이 waittime 보다 크거나 같을떄 대기 종료
		if (WaitTimer >= WaitTime)
		{
			// 타이머 초기화
			WaitTimer = 0.0f;
			
			//대기 상태 해제
			bWaiting = false;
			
			// 이동 방향 반전 
			bMovingUp = !bMovingUp;
		}
		
		// 대기중에는 실행 x
		return;
	}
	// 이번 프레임에 이동할 거리 
	float Movestep = MoveSpeed * DeltaTime;
	
	// 위로 이동 중일때 
	if (bMovingUp)
	{
		// 현재 이동 거리 증가
		CurrentDistance += Movestep;
		
		// 최대 이동거리 보다 커지면 MaxRange로 제한
		if (CurrentDistance >= MaxRange)
		{
			CurrentDistance = MaxRange;
			
			// 끝에 도착했으면 대기
			bWaiting = true;
		}
		
	}
	// 아래 이동 
	else
	{
		//현제 이동 거리를 감소
		CurrentDistance -= Movestep;
		
		// 0보다 자ㄱ아지면 완전히 아래에 숨은 상태로 제한
		if (CurrentDistance <= 0.0f)
		{
			CurrentDistance = 0.0f;
			
			bWaiting = true;
		}
	}
	
	//새 위치 계산 
	FVector NewLocation = StartLocation;
	
	// 시작할때 숨긴경우 
	// StartLocation보다 MaxRange 아래에서 시작해서 CurrentDistance만큼 올라옴
	// CurrentDistance가 0이면 아래에 숨어 있고,
	// CurrentDistance가 MaxRange면 StartLocation까지 올라옴
	if (bStartHidden)
	{
		NewLocation.Z = NewLocation.Z - MaxRange + CurrentDistance;
	}
	// 숨기지 않은 경우 
	// 배치된 위치를 기준으로 위아래 이동
	else
	{
		NewLocation.Z = NewLocation.Z + CurrentDistance;
		
	}
	
	// 계산된 위치로 Actor를 이동
	SetActorLocation(NewLocation);
}

