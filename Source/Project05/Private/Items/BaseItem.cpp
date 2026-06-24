#include "Items/BaseItem.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ABaseItem::ABaseItem()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(RootComponent);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	
	// 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

// 플레이어가 이 아이템의 범위에 들러왔을때 호출
void ABaseItem::OnItemOverlap(
	    UPrimitiveComponent* OverlappedComp,     // 오버랩이 발생한 자기자신(아이템콜리전)
		AActor* OtherActor,						 // 콜리전에 부딪힌 액터(플레이어)
		UPrimitiveComponent* OtherComp,          // 캡슐컴포넌트
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		ActivateItem(OtherActor);
	}
}
// 플레이어가 이 아이템의 범위을 벗어났을 때 호출
void ABaseItem::OnItemEndOverlap(
	    UPrimitiveComponent* OverlappedComp,    
		AActor* OtherActor,						 
		UPrimitiveComponent* OtherComp,          
		int32 OtherBodyIndex)
{
	
}
// 아이템이 사용되었을때 호출
void ABaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;
	
	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			false);
	}
	
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation());
	}
	
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			2.0f,
			false);
	}
}
// 이 아이템의 유형(이름 타입)을 반환
FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}
