#include "Core/MyGameState.h"

#include "Core/MyGameInstance.h"
#include "Core/SantaPlayerController.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Spawners/SpawnVolume.h"
#include "Items/CoinItem.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

AMyGameState::AMyGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 6000.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	
	// 웨이브
	CurrentWaveIndex = 0;
	bWaveActive = false;
	
	Waves.Add(FWaveData{30.0f, 15});
	Waves.Add(FWaveData{25.0f, 20});
	Waves.Add(FWaveData{600.0f, 30});
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	
	FString CurrentMapName = GetWorld()->GetMapName();

	if (CurrentMapName.Contains(TEXT("MenuLevel")))
	{
		return;
	}
	
	StartLevel();
	
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMyGameState::UpdateHUD,
		0.1f,
		true
		);
}

// 타이머 정리
void AMyGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(LevelTimerHandle);
		World->GetTimerManager().ClearTimer(HUDUpdateTimerHandle);
		World->GetTimerManager().ClearAllTimersForObject(this);
	}

	Super::EndPlay(EndPlayReason);
}

int32 AMyGameState::GetScore() const
{
	return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToScore(Amount);
		}
	}
	
}


void AMyGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASantaPlayerController* SantaPlayerController = Cast<ASantaPlayerController>(PlayerController))
		{
			SantaPlayerController->ShowGameHUD();
		}
	}
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
		}
	}
	
	// 웨이브
	CurrentWaveIndex = 0;
	bWaveActive = false;
	
	StartWave();
}

//코인 먹기
void AMyGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	
	UE_LOG(LogTemp, Warning,TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount);
	
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}

// 레벨끝내기
void AMyGameState::EndLevel()
{
	bWaveActive = false;
	
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	
	// 코인정리
	ClearCurrentWaveCoins();
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* SpartaGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
			
			if (CurrentLevelIndex >= MaxLevels)
			{
				OnGameOver();
				return;
			}
			
			if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
			{
				UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
			}
			else
			{
				OnGameOver();
			}
		}
	}
}

void AMyGameState::OnGameOver()
{
	bWaveActive = false;
	
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	
	ClearCurrentWaveCoins();
	// Ui, 재시작
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASantaPlayerController* SantaPlayerController = Cast<ASantaPlayerController>(PlayerController))
		{
			SantaPlayerController->SetPause(true);
			SantaPlayerController->ShowGameOverMenu();
		}
	}
	
}

// 텍스트설정
void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASantaPlayerController* SantaPlayerController = Cast<ASantaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SantaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Time")))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Score")))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
						if (MyGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
						}
					}
				}
				
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Level")))
				{
					
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
				
				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Wave")))
				{
					WaveText->SetText(FText::FromString(
						FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)
					));
				}
			}
		}
	}
}

// 웨이브
void AMyGameState::StartWave()
{
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		EndLevel();
		return;
	}
	
	bWaveActive = true;
	
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentWaveCoins.Empty();
	
	const FWaveData& CurrentWave = Waves[CurrentWaveIndex];
	
	FString WaveMessage = FString::Printf(
		TEXT("Level %d - Wave %d 시작!"),
		CurrentLevelIndex + 1,
		CurrentWaveIndex + 1);
	
	UE_LOG(LogTemp, Warning, TEXT("Wave: %s"), *WaveMessage);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Yellow,
			WaveMessage);
	}
	
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	for (int32 i = 0; i < CurrentWave.ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, FoundVolumes.Num() - 1);
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[RandomIndex]);

			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();

				if (ACoinItem* CoinItem = Cast<ACoinItem>(SpawnedActor))
				{
					SpawnedCoinCount++;
					CurrentWaveCoins.Add(CoinItem);
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AMyGameState::OnWaveTimeUp,
		CurrentWave.WaveDuration,
		false
	);
}

void AMyGameState::OnWaveTimeUp()
{
	// 시간 초과 또는 웨이브 종료 시 남은 코인 제거
	ClearCurrentWaveCoins();
	
	EndWave();
}

void AMyGameState::EndWave()
{
	if (!bWaveActive)
	{
		return;
	}

	bWaveActive = false;
	
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	
	CurrentWaveIndex++;

	if (CurrentWaveIndex >= Waves.Num())
	{
		EndLevel();
	}
	else
	{
		StartWave();
	}
}

void AMyGameState::ClearCurrentWaveCoins()
{
	for (TWeakObjectPtr<AActor> CoinPtr : CurrentWaveCoins)
	{
		AActor* CoinActor = CoinPtr.Get();

		if (IsValid(CoinActor))
		{
			CoinActor->Destroy();
		}
	}

	CurrentWaveCoins.Empty();
}

//레벨에서 40개의 아이템 소환
// 코인 아이템들을 다 먹었을 경우에만 바로 다음 레벨
// 30초의 시간이 주어짐. 타임오버가 되도 바로 다음 레벨
// 체력이 0이되면, 바로 즉시 게임 오버
// 게임 종료 이후 -> 재시작

//GameInstance -> 게임 전체에서 지속적으로 유지되는 싱글톤객체-> 레벨전환후 데이터 유지할때
