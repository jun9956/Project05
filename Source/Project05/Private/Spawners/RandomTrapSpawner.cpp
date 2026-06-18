#include "Spawners/RandomTrapSpawner.h"


ARandomTrapSpawner::ARandomTrapSpawner()
{
 	
	PrimaryActorTick.bCanEverTick = false;

}


void ARandomTrapSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnRandomTraps();
	
}

void ARandomTrapSpawner::SpawnRandomTraps()
{
	// 배열에 아무 함정 클래스도 등록하지 않았으면 생성하지 않는다.
	if (TrapClasses.Num() == 0)
	{
		return;
	}

	for (int32 i = 0; i < SpawnCount; i++)
	{
		// TrapClasses 배열에서 랜덤 인덱스 선택
		int32 RandomIndex = FMath::RandRange(0, TrapClasses.Num() - 1);

		// 선택된 함정 클래스 가져오기
		TSubclassOf<AActor> SelectedTrapClass = TrapClasses[RandomIndex];

		// 혹시 배열 칸이 비어 있으면 이번 반복은 건너뛴다.
		if (!SelectedTrapClass)
		{
			continue;
		}

		// 스포너 위치 기준 랜덤 X, Y 좌표 생성
		float RandomX = FMath::RandRange(-SpawnRangeX, SpawnRangeX);
		float RandomY = FMath::RandRange(-SpawnRangeY, SpawnRangeY);

		// 최종 스폰 위치 계산
		FVector SpawnLocation = GetActorLocation() + FVector(RandomX, RandomY, SpawnHeight);

		// Z축 기준 랜덤 회전
		FRotator SpawnRotation = FRotator(
			0.0f,
			FMath::RandRange(0.0f, 360.0f),
			0.0f
		);

		// 선택된 함정을 월드에 생성
		GetWorld()->SpawnActor<AActor>(
			SelectedTrapClass,
			SpawnLocation,
			SpawnRotation
		);
	}
		
	
}

















// void ARandomTrapSpawner::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

