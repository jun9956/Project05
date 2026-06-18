#include "Characters/ProjectPawn.h"
#include "Core/ProjectPlayerController.h"
#include "EnhancedInputComponent.h"
#include "VectorUtil.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/ArrowComponent.h"

AProjectPawn::AProjectPawn()
{
 
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	
	CapsuleComp->InitCapsuleSize(50.0f, 50.0f);
	//충돌 테스트
	// CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
 //    CapsuleComp->SetCollisionObjectType(ECC_Pawn);
 //    CapsuleComp->SetCollisionResponseToAllChannels(ECR_Block);
 //    CapsuleComp->SetGenerateOverlapEvents(true);
	
	CapsuleComp->SetSimulatePhysics(false);
	
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	SkeletalMeshComp->SetSimulatePhysics(false);
	//충돌 테스트
	// SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 90.0f, 0.0f));
	SpringArmComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	
	currentPitch = 0.0f;
	
	SpringArmComp->bUsePawnControlRotation = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	CameraComp->bUsePawnControlRotation = false;
	
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(CapsuleComp);
	ArrowComp->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	ArrowComp->SetRelativeRotation(FRotator::ZeroRotator);
	
}

void AProjectPawn::BeginPlay()
{
	Super::BeginPlay();
	
}


void AProjectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//키바인딩
void AProjectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AProjectPlayerController* PlayerController = Cast<AProjectPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AProjectPawn::Move
					);
			}
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&AProjectPawn::StopMove
					);
			}
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Canceled,
					this,
					&AProjectPawn::StopMove
					);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AProjectPawn::StartJump
					);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AProjectPawn::StopJump
					);
			}
			
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AProjectPawn::Look
					);
			}
			
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AProjectPawn::StartSprint
					);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AProjectPawn::StopSprint
					);
			}
		}
	}
	

}
//이동함수
void AProjectPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	
	 const FVector2D MoveInput = value.Get<FVector2D>();
	
	bIsMoving = !MoveInput.IsNearlyZero();
	AnimMoveSpeed = bIsMoving ? MoveSpeed : 0.0f;

	// 움직이지 않으면 여기서 종료
	if (!bIsMoving)
	{
		return;
	}
	
	//프레림 독립성 
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	//앞뒤
	FVector ForwardMove = FVector::ForwardVector * MoveInput.X;
	//좌우
	FVector RightMove = FVector::RightVector * MoveInput.Y;
	//앞뒤 좌우이동을 더해서 최종 이동 방향
	FVector MoveDirection = ForwardMove + RightMove;
	//실제 이동량 계산
	const FVector DeltaMove = MoveDirection * MoveSpeed * DeltaTime;
	//직접 이동시킴
	AddActorLocalOffset(DeltaMove, true);
	//충돌 테스트 
	// FHitResult Hit;
	// AddActorLocalOffset(DeltaMove, true, &Hit);
	//
	// if (Hit.bBlockingHit)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Blocked by: %s"), *Hit.GetActor()->GetName());
	// }
	
}

void AProjectPawn::StopMove()
{
	bIsMoving = false;
	AnimMoveSpeed = 0.0f;
}

void AProjectPawn::StartJump(const FInputActionValue& value)
{
	
}

void AProjectPawn::StopJump(const FInputActionValue& value)
{
	
}

void AProjectPawn::Look(const FInputActionValue& value)
{
	// IA_Look에서 입력값 가져옴
	const FVector2D LookInput = value.Get<FVector2D>();
	
	//프레임 독립성을 위한 델타타임
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	//마우스 x값으로 yaw 회전량 계산
	float YawValue = LookInput.X * LookSpeed * DeltaTime;
	//마우스 y값으로 pitch 회전량 계산
	float PitchValue = -LookInput.Y * LookSpeed * DeltaTime;
	
	AddActorLocalRotation(FRotator(0.0f, YawValue, 0.0f));
	
	//현제 카메라 pitch값 누적
	currentPitch += PitchValue;
	//위/아래 너무 꺽이지 않도록
	currentPitch = FMath::Clamp(currentPitch, MinPitch, MaxPitch);
	//
	SpringArmComp->SetRelativeRotation(FRotator(currentPitch, 0.0f, 0.0f));
}

void AProjectPawn::StartSprint(const FInputActionValue& value)
{
	
}

void AProjectPawn::StopSprint(const FInputActionValue& value)
{
	
}


