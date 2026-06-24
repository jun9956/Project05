#include "Core/MyGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


UMyGameInstance::UMyGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
	
	MenuBGM = nullptr;
	GameBGM = nullptr;
	BGMComponent = nullptr;
	CurrentBGM = nullptr;
}

void UMyGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}

void UMyGameInstance::PlayBGM(USoundBase* NewBGM)
{
	if (!NewBGM)
	{
		return;
	}

	// 같은 BGM이 이미 재생 중이면 다시 재생하지 않음
	if (CurrentBGM == NewBGM && BGMComponent && BGMComponent->IsPlaying())
	{
		return;
	}

	// 기존 BGM 정지
	if (BGMComponent)
	{
		BGMComponent->Stop();
		BGMComponent = nullptr;
	}

	CurrentBGM = NewBGM;

	BGMComponent = UGameplayStatics::SpawnSound2D(
		this,
		NewBGM,
		1.0f,		// Volume
		1.0f,		// Pitch
		0.0f,		// Start Time
		nullptr,
		true,		// 레벨 전환 후에도 유지
		false		// 자동 삭제 안 함
	);
}

void UMyGameInstance::StopBGM()
{
	if (BGMComponent)
	{
		BGMComponent->Stop();
		BGMComponent = nullptr;
	}

	CurrentBGM = nullptr;
}

