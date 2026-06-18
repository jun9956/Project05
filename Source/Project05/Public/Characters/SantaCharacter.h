#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SantaCharacter.generated.h"

UCLASS()
class PROJECT05_API ASantaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASantaCharacter();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 데미지 
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override; 
	
public:
	// 3인칭 카메라 막대
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class USpringArmComponent* SpringArmComponent;

	// 카메라 본체
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class UCameraComponent* CameraComponent;
	
public:
	// Input 에셋들 — BP에서 할당
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* JumpAction;
	
	// 스프린트 입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SprintAction;
	
	// 체력 설정
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	
	// 체력 회복
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

	
protected:
	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 600.0f;

	// 달리기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float LookSensitivity = 0.5f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
protected:
	// 실제 입력 처리 함수들
	void InputActionMove(const struct FInputActionValue& Value);
	void InputActionLook(const struct FInputActionValue& Value);
	void InputActionJump(const struct FInputActionValue& Value);
	
	// 점프 키를 뗐을 때
	void InputActionStopJump(const struct FInputActionValue& Value);

	// 스프린트
	void StartSprint();
	void StopSprint();
	
	// 죽음
	void OnDeath();
	
	
};
