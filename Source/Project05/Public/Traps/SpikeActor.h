// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpikeActor.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT05_API ASpikeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* SpikeMesh;
	
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
	FRotator RotationSpeed = FRotator(0.0f, 0.0f, 360.0f);
	
	//충돌 테스트 
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionBox;

};
