// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Addons/Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/TurboPlayer.h"
#include "Interface/EnemyDataUI.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	myMesh = CreateDefaultSubobject<UStaticMeshComponent>("My Mesh");

	myMesh->SetupAttachment(RootComponent);

	projectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Comp");

}

void AProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	ATurboPlayer* _player = Cast<ATurboPlayer>(OtherActor);

	if(nullptr != _player) {

    if (OtherActor) {

      if (OtherActor->GetClass()->ImplementsInterface(UEnemyDataUI::StaticClass())) {
        _player->HitUIUpdate(OtherActor);
      }

      FAttackStats as;
      as.attack_dmg = damage;

      IIDamagable::Execute_TakeAHit(OtherActor, FVector::ZeroVector, as, false, this);

    }

    Destroy();

	}

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	myMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnActorOverlap);

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

