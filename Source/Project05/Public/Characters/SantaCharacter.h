#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SantaCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class PROJECT05_API ASantaCharacter : public ACharacter
{
	GENERATED_BODY()
	
	//스크린
	//월드
	

public:
	ASantaCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverHeadWidget;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	
	// 이동 속도 감소 디버프 적용
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void ApplySlowDebuff(float Duration);
	
	// UFUNCTION(BlueprintPure, Category = "Debuff")
	// FText GetDebuffStatusText() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float LookSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	// 디버프
	// 현재 달리고있는지를 저장
	bool bIsSprinting = false;

	// 디버프 중첩당 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float SlowMultiplierPerStack = 0.5f;

	// 최저 속도 고정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float MinSlowSpeedMultiplier = 0.25f;

	// 슬로우디버프 끝나는 시간 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	TArray<float> SlowEndTimes;

protected:
	void InputActionMove(const struct FInputActionValue& Value);
	void InputActionLook(const struct FInputActionValue& Value);
	void InputActionJump(const struct FInputActionValue& Value);
	void InputActionStopJump(const struct FInputActionValue& Value);

	void StartSprint();
	void StopSprint();

	void OnDeath();
	void UpdateOverheadHP();
	
	// 짖버프 업데이트
	void UpdateSlowDebuff();
	//현재상테에 맞게 속도 계산
	void UpdateMovementSpeed();
	// 디버프 배열에서 가장 오래된 시간 구하기
	float GetMaxRemainingTime(const TArray<float>& EndTimes) const;
};
