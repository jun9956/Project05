#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;

struct FInputActionValue;

//class UInputMappingContext;
//class UInputAction;


UCLASS()
class PROJECT05_API AProjectPawn : public APawn
{
	GENERATED_BODY()

public:
	
	AProjectPawn();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComp;
	
protected:
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopMove();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveSpeed = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float LookSpeed = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinPitch = -60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxPitch = 30.0f;
	
	float currentPitch = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float AnimMoveSpeed = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsMoving = false;
};
