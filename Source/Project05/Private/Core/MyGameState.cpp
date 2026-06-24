#include "Core/MyGameState.h"

#include "Core/MyGameInstance.h"
#include "Core/SantaPlayerController.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Spawners/SpawnVolume.h"
#include "Items/CoinItem.h"
#include "Blueprint/UserWidget.h"

AMyGameState::AMyGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 6000.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	
	StartLevel();
	
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMyGameState::UpdateHUD,
		0.1f,
		true
		);
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
	
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	
	const int32 ItemToSpawn = 40;
	
	// 코인갯수 
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++; 
				}
			}
		}
	}
	
	// 시간설정
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AMyGameState::OnLevelTimeUp,
		LevelDuration,
		false
		);
}

// 시간 끝나면 레벨끝내기
void AMyGameState::OnLevelTimeUp()
{
	EndLevel();
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
		EndLevel();
	}
}

// 레벨끝내기
void AMyGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	
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
	// Ui, 재시작
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASantaPlayerController* SantaPlayerController = Cast<ASantaPlayerController>(PlayerController))
		{
			SantaPlayerController->SetPause(true);
			SantaPlayerController->ShowMainMenu(true);
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
			}
		}
	}
}

//레벨에서 40개의 아이템 소환
// 코인 아이템들을 다 먹었을 경우에만 바로 다음 레벨
// 30초의 시간이 주어짐. 타임오버가 되도 바로 다음 레벨
// 체력이 0이되면, 바로 즉시 게임 오버
// 게임 종료 이후 -> 재시작

//GameInstance -> 게임 전체에서 지속적으로 유지되는 싱글톤객체-> 레벨전환후 데이터 유지할때
