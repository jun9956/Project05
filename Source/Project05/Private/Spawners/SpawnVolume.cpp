#include "Spawners/SpawnVolume.h"
#include "components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
	
	ItemDataTable = nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem(int32 CurrentWaveNumber)
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem(CurrentWaveNumber))
	{
		if (UClass* ActualClass = SelectedRow-> ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	
	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem(int32 CurrentWaveNumber) const
{
	if (!ItemDataTable) return nullptr;
	
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);
	
	if (AllRows.IsEmpty()) return nullptr;
	
	TArray<FItemSpawnRow*> AvailableRows;

	for (FItemSpawnRow* Row : AllRows)
	{
		if (Row && Row->UnlockWave <= CurrentWaveNumber && Row->SpawnChance > 0.0f)
		{
			AvailableRows.Add(Row);
		}
	}

	if (AvailableRows.IsEmpty()) return nullptr;
	
	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AvailableRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}
	
	if (TotalChance <= 0.0f) return nullptr;
	
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	
	for (FItemSpawnRow* Row : AvailableRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}
	
	return nullptr;
	
	// 0.0 ~ 1.0
	// 랜덤값 0.7이면
	
	// item A 0.5
	// item B 0.8
	// item C 1.0
	// b가 뽑힘
	
	// 빨간공 5개 (50%)
	// 파란공 3개 (30%)
	// 초록공 2개 (20%)
	// 전체 10개 
	// 0~1 까지의 랜덤 숫자 뽑음  ex)0.3 이면 빨강 0.6이면 파랑 0.9면 초록
	// 누적확률 방식의 랜덤뽑기
	
}


// 랜덤위치
FVector ASpawnVolume::GetRandomPointInVolume() const
{
	// 중심 -> 끝 까지의 거리
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	// 중심 값
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}


AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;
	
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
		);
	
	return SpawnedActor;
}
