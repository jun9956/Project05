// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/SpikeActor.h"
#include "Components/BoxComponent.h"
#include "DataWrappers/ChaosVDParticleDataWrapper.h"

// Sets default values
ASpikeActor::ASpikeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(SceneRoot);

	CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);

	
	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	SpikeMesh ->SetupAttachment(SceneRoot);
	// 충돌
	SpikeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ASpikeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpikeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FRotator DeltaRotation = RotationSpeed * DeltaTime;
	
	AddActorLocalRotation(DeltaRotation);

}

