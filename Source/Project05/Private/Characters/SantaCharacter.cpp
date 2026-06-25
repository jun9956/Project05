#include "Characters/SantaCharacter.h"


#include "Camera/CameraComponent.h"
#include "Core/SantaPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h" // UEnhancedInputComponent, BindAction()
#include "InputActionValue.h" // FInputActionValue
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/MyGameState.h"
#include "Engine/Engine.h"


ASantaCharacter::ASantaCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	
	// SpringArm — RootComponent(캡슐)에 붙임
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;          // 캐릭터에서 400 만큼 떨어짐
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 40.0f, 70.0f));
	SpringArmComponent->bUsePawnControlRotation = true;    // 컨트롤러 회전을 따라감

	// Camera — SpringArm 끝에 붙임
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	// CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);// 바로 위에 코드랑 동일 
	CameraComponent->bUsePawnControlRotation = false;      // 카메라 자체는 회전 따라가지 X

	// 몸이 컨트롤러 yaw를 바로 따라가지 않음
	bUseControllerRotationYaw = true;
	
	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(GetMesh());
	OverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	//hp바
	OverHeadWidget->SetDrawSize(FVector2D(150.0f, 20.0f));
	OverHeadWidget->SetPivot(FVector2D(0.5f, 0.5f));
	
	// 이동/점프 설정
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	
	// 체력설정
	MaxHealth = 100.0f;
	Health = MaxHealth;
	
}


void ASantaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateMovementSpeed();
	
	UpdateOverheadHP();
}

void ASantaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 매 프레임 디버프 시간이 끝났는지 확인한다.
	// 시간이 끝난 디버프는 배열에서 제거된다.
	UpdateSlowDebuff();
}

void ASantaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASantaPlayerController* SantaPlayerController = Cast<ASantaPlayerController>(GetController()))
		{
			if (SantaPlayerController->MoveAction) EIC->BindAction(SantaPlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASantaCharacter::InputActionMove);
			if (SantaPlayerController->LookAction) EIC->BindAction(SantaPlayerController->LookAction, ETriggerEvent::Triggered, this, &ASantaCharacter::InputActionLook);
			if (SantaPlayerController->JumpAction) EIC->BindAction(SantaPlayerController->JumpAction, ETriggerEvent::Started,   this, &ASantaCharacter::InputActionJump);
			if (SantaPlayerController->JumpAction) EIC->BindAction(SantaPlayerController->JumpAction, ETriggerEvent::Completed, this, &ASantaCharacter::InputActionStopJump);
			if (SantaPlayerController->SprintAction) EIC->BindAction(SantaPlayerController->SprintAction, ETriggerEvent::Started, this, &ASantaCharacter::StartSprint);
			if (SantaPlayerController->SprintAction) EIC->BindAction(SantaPlayerController->SprintAction, ETriggerEvent::Completed, this, &ASantaCharacter::StopSprint);
		}
	}
}



void ASantaCharacter::InputActionMove(const FInputActionValue& Value)
{
	const FVector2D MoveVec = Value.Get<FVector2D>();
	if (Controller == nullptr)
	{
		return;
	}

	// 카메라 yaw 방향 기준으로 forward/right 계산
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);  // pitch/roll 0, yaw만

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveVec.Y);   // W/S → 앞뒤
	AddMovementInput(RightDirection, MoveVec.X);   // A/D → 좌우
}

void ASantaCharacter::InputActionLook(const FInputActionValue& Value)
{
	const FVector2D LookVec = Value.Get<FVector2D>();

	AddControllerYawInput(LookVec.X * LookSensitivity);
	AddControllerPitchInput(LookVec.Y * LookSensitivity);
}

void ASantaCharacter::InputActionJump(const FInputActionValue& Value)
{
	Jump();   // ACharacter가 기본 제공하는 점프 함수
}

void ASantaCharacter::InputActionStopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void ASantaCharacter::StartSprint()
{
	bIsSprinting = true;
	
	UpdateMovementSpeed();
}

void ASantaCharacter::StopSprint()
{
	bIsSprinting = false;
	
	UpdateMovementSpeed();
}


float ASantaCharacter::GetHealth() const
{
	return Health;
}

void ASantaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

float ASantaCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();
	
	if (Health <= 0.0f)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

void ASantaCharacter::OnDeath()
{
	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->OnGameOver();
	}
}

void ASantaCharacter::UpdateOverheadHP()
{
	if (!OverHeadWidget) return;
	
	UUserWidget* OverHeadWidgetInstance = OverHeadWidget->GetUserWidgetObject();
	if (!OverHeadWidgetInstance) return;
	
	// 체력 비율 계산
	float HealthPercent = 0.0f;

	if (MaxHealth > 0.0f)
	{
		HealthPercent = Health / MaxHealth;
	}

	HealthPercent = FMath::Clamp(HealthPercent, 0.0f, 1.0f);

	// Progress Bar 업데이트
	if (UProgressBar* HPBar = Cast<UProgressBar>(OverHeadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHPBar"))))
	{
		HPBar->SetPercent(HealthPercent);
	}
	// if (UTextBlock* HPText = Cast<UTextBlock>(OverHeadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	// {
	// 	HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	// }
	// 	
}

void ASantaCharacter::ApplySlowDebuff(float Duration)
{
	if (Duration <= 0.0f || GetWorld() == nullptr)
	{
		return;
	}
	
	// 현재 시간 + 지속 시갖 = Slow가 끝나는 시간
	const float EndTime = GetWorld()->GetTimeSeconds() + Duration;

	// 배열에 끝나는 시간 추가
	SlowEndTimes.Add(EndTime);
	
	// slow가 적용되었으므로 이동 속도 다시 업데이트
	UpdateMovementSpeed();
	
	// 테스트용 화면 메시지
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Cyan,
			FString::Printf(TEXT("Slow 시작! 중첩: x%d"), SlowEndTimes.Num())
		);
	}
}

void ASantaCharacter::UpdateSlowDebuff()
{
	if (GetWorld() == nullptr)
	{
		return;
	}
	
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	
	// 슬로우 중첩 갯수 저장
	const int32 PreviousSlowCount = SlowEndTimes.Num();
	
	// 시간이 끝나고 슬로우 디버프 제거
	// 배열은 뒤부터 지워야 인덱스가 안꼬임
	for (int32 i = SlowEndTimes.Num() - 1; i >= 0; i--)
	{
		if (SlowEndTimes[i] <= CurrentTime)
		{
			SlowEndTimes.RemoveAt(i);
		}
	}
	
	// 슬로우 중첩수가 바뀌었다면 속도 다시 계산
	if (PreviousSlowCount != SlowEndTimes.Num())
	{
		UpdateMovementSpeed();
		
		if (PreviousSlowCount > 0 && SlowEndTimes.Num() <=0)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					2.0f,
					FColor::Green,
					TEXT("Slow 종료!")
				);
			}
		}
	}
}

void ASantaCharacter::UpdateMovementSpeed()
{
	// 기본 걷기 속도
	float CurrentBaseSpeed = WalkSpeed;
	
	// 달리는 중이면 달리기 속도 기준으로 사용
	if (bIsSprinting)
	{
		CurrentBaseSpeed = SprintSpeed;
	}
	
	//기본 배율
	float FinalSpeedMultiplier = 1.0f;
	
	// 슬로우가 하나라도 있으면 중첩수에 따라 속도 감소
	if (SlowEndTimes.Num() > 0)
	{
		// 1중첩 0.5 2중첩 0.25 
		FinalSpeedMultiplier = FMath::Pow(SlowMultiplierPerStack, SlowEndTimes.Num());
		
		// 최소 속도 제한
		FinalSpeedMultiplier = FMath::Clamp(
			FinalSpeedMultiplier,
			MinSlowSpeedMultiplier,
			1.0f);
	}
	
	// 최종 이동 속도 적용
	GetCharacterMovement()->MaxWalkSpeed = CurrentBaseSpeed * FinalSpeedMultiplier;
}