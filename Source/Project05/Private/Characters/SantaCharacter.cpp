#include "Characters/SantaCharacter.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h" // UEnhancedInputComponent, BindAction()
#include "EnhancedInputSubsystems.h" // UEnhancedInputLocalPlayerSubsystem, AddMappingContext
#include "InputActionValue.h" // FInputActionValue
#include "GameFramework/PlayerController.h" // APlayerController
#include "Engine/LocalPlayer.h" // ULocalPlayer
#include "GameFramework/CharacterMovementComponent.h"


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

	// // 이동 방향으로 캐릭터가 회전
	// GetCharacterMovement()->bOrientRotationToMovement = true;
	// GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// 이동/점프 설정
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 420.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	// 체력설정
	MaxHealth = 100.0f;
	Health = MaxHealth;
	
}


void ASantaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void ASantaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction) EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASantaCharacter::InputActionMove);
		if (LookAction) EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASantaCharacter::InputActionLook);
		if (JumpAction) EIC->BindAction(JumpAction, ETriggerEvent::Started,   this, &ASantaCharacter::InputActionJump);
		if (JumpAction) EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASantaCharacter::InputActionStopJump);
		if (SprintAction) EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &ASantaCharacter::StartSprint);
		if (SprintAction) EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASantaCharacter::StopSprint);
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
	UE_LOG(LogTemp, Warning, TEXT("Health increased to %f"), Health);
}

float ASantaCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to %f"), Health);
	
	if (Health <= 0.0f)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

void ASantaCharacter::OnDeath()
{
	// 게임 종료 로직
}