// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/BossMissile.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/IDamagable.h"
#include "Interface/EnemyDataUI.h"
#include "Core/BaseCharacter.h"
#include "Core/BaseEnemy.h"
#include "Core/GameInstanceBEU.h"
#include "Enemies/FinalBossCharacter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABossMissile::ABossMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Comp");
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	mesh->SetupAttachment(RootComponent);

	trail = CreateDefaultSubobject<UParticleSystemComponent>("Trail");
	trail->SetupAttachment(mesh);

}

// Called when the game starts or when spawned
void ABossMissile::BeginPlay()
{
	Super::BeginPlay();
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called every frame
void ABossMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector pos = GetActorLocation();
	pos.Z -= DeltaTime + fallSpeed;
	SetActorLocation(pos);

	if (pos.Z <= limitZAxisHeight) {

		UGameInstanceBEU* myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));
		myGI->camera->ShakeCamera(10.0f);
		//Explosion
		TArray<FHitResult> hits;
		FVector explosionLocation = GetActorLocation();
		explosionLocation.Z += explosionZOffset;

		FCollisionShape colSphere = FCollisionShape::MakeSphere(explosionRadius);
		UParticleSystem* SystemTemplate = explosionParticle.PSTemplate;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(FQuat::Identity);
		SpawnTransform.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
		explosionParticle.particleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SystemTemplate,
			SpawnTransform,
			true, EPSCPoolMethod::AutoRelease, true);

		if (GetWorld()->SweepMultiByChannel(hits, explosionLocation, GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel13, colSphere)) {

			for (auto& hitIterator : hits) {

				AActor* other = hitIterator.GetActor();
				AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(other);
				if (other && !boss) {

					if (other->GetClass()->ImplementsInterface(UIDamagable::StaticClass())) {

						ABaseEnemy* enemy = Cast<ABaseEnemy>(other);
						if (damagesEnemies || !enemy) {

							FVector knockbackDir = (other->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
							knockbackDir *= combatStats.attack_knockback;
							knockbackDir.Z = combatStats.yKnockBackForce;
							IIDamagable::Execute_TakeAHit(other, knockbackDir, combatStats, false, this);

						}

					}

				}

			}

		}
		Destroy();
	}

}

