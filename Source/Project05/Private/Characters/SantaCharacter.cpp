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


ASantaCharacter::ASantaCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
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
	
	UpdateOverheadHP();
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
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ASantaCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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

