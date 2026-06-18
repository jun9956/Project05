#include "Traps/MagicTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AMagicTrap::AMagicTrap()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(SceneRoot);
	
	CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);
	
	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
	TrapMesh->SetupAttachment(SceneRoot);
	
	//충돌
	TrapMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


void AMagicTrap::BeginPlay()
{
	Super::BeginPlay();
	
	// 처음 위치를 저장한다.
    	// 랜덤 이동은 이 위치를 기준으로 계산된다.
    	StartLocation = GetActorLocation();
	
	// 시작 상태는 보이는 상태로 설정
	bIsVisible = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	// VisibleTime초 후 HideTrap 실행
	GetWorld()->GetTimerManager().SetTimer(
		MagicTimerHandle,       // 이 타이머를 관리할 핸들
		this,					// 현제 객체에서
		&AMagicTrap::HideTrap,    // 실행할 함수
		VisibleTime,			// 몇초뒤에 실행할지
		false			// 한번실행, true 반복실행
	);
}

void AMagicTrap::HideTrap()
{
	// 현재 상태를 숨김으로 변경
	bIsVisible = false;

	// 화면에서 보이지 않게 하고 충돌도 끈다.
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
	// 숨겨진 상태에서 랜덤 위치로 이동한다.
	if (bUseRandomMove)
	{
		MoveToRandomLocation();
	}


	// HiddenTime초 후 다시 ShowTrap 실행
	GetWorld()->GetTimerManager().SetTimer(
		MagicTimerHandle,
		this,
		&AMagicTrap::ShowTrap,
		HiddenTime,
		false
	);
}

void AMagicTrap::ShowTrap()
{
	// 현재 상태를 보임으로 변경
	bIsVisible = true;

	// 화면에 다시 보이게 하고 충돌도 켠다.
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	// VisibleTime초 후 다시 HideTrap 실행
	GetWorld()->GetTimerManager().SetTimer(
		MagicTimerHandle,
		this,
		&AMagicTrap::HideTrap,
		VisibleTime,
		false
	);
}

void AMagicTrap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(MagicTimerHandle);
}

// 랜덤 이동 함수 
void AMagicTrap::MoveToRandomLocation()
{
	float RandomX = FMath::RandRange(-MoveRangeX, MoveRangeX);
	float RandomY = FMath::RandRange(-MoveRangeY, MoveRangeY);

	FVector NewLocation = StartLocation + FVector(RandomX, RandomY, 0.0f);

	SetActorLocation(NewLocation);
}

// void AMagicTrap::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

