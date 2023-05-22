// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ExplosiveThrowable.h"
#include "Interface/EnemyDataUI.h"
#include "Interface/IDamagable.h"
#include "Core/BaseCharacter.h"
#include "Player/TurboPlayer.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Core/CommonBEU.h"


void AExplosiveThrowable::DestroyThrowableActor()
{

	TArray<FHitResult> hits;

	FCollisionShape colSphere = FCollisionShape::MakeSphere(explosionRadius);

	if (GetWorld()->SweepMultiByChannel(hits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel13, colSphere)) {

		for (auto& hitIterator : hits) {

			AActor* other = hitIterator.GetActor();
			if (other) {

        if (other->GetClass()->ImplementsInterface(UEnemyDataUI::StaticClass())) {
          int32 enemy_type = 0;
          float hp_percentage = 0.0f;
          int32 points = 0;
          IEnemyDataUI::Execute_CatchUIDataEnemy(other, enemy_type, hp_percentage, points, playerPickID);
          hitEvent.Broadcast(enemy_type, hp_percentage, points);

        }

				if (other->GetClass()->ImplementsInterface(UIDamagable::StaticClass())) {

					ATurboPlayer* player = Cast<ATurboPlayer>(other);
					if (canDamagePlayer || !player) {

						FVector knockbackDir = (other->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
						knockbackDir *= stats.attack_knockback;
						knockbackDir.Z = stats.yKnockBackForce;
						IIDamagable::Execute_TakeAHit(other, knockbackDir, stats, false, this);
				
						//For explosives that apply slow
						if (applySlow) {

							ABaseCharacter* character = Cast<ABaseCharacter>(hitIterator.GetActor());
							if (character) {

								character->ApplySlow(slowPercentage/100.0f, slowDuration);

							}


						}
				

						if (other->GetClass()->ImplementsInterface(UEnemyDataUI::StaticClass())) {
							int32 enemy_type = 0;
							float hp_percentage = 0.0f;
							int32 points = 0;
							IEnemyDataUI::Execute_CatchUIDataEnemy(other, enemy_type, hp_percentage, points, playerPickID);
							hitEvent.Broadcast(enemy_type, hp_percentage, points);

						}
						
					}
				

				}

			}

		}

	}

	UParticleSystem* SystemTemplate = explosionParticle.PSTemplate;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetActorLocation());
	SpawnTransform.SetRotation(FQuat::Identity);
	SpawnTransform.SetScale3D(FVector(1.0f,1.0f,1.0f));
	explosionParticle.particleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SystemTemplate,
		SpawnTransform,
		true, EPSCPoolMethod::AutoRelease, true);

	Destroy();

}

void AExplosiveThrowable::OverlapWithEnemy(AActor* self, AActor* other) {

	if (onAir) {

		if (other->GetClass()->ImplementsInterface(UIDamagable::StaticClass())) {

			ATurboPlayer* player = Cast<ATurboPlayer>(other);
			if (canDamagePlayer || !player) {

				DestroyThrowableActor();

			}
		}

	}

}

void AExplosiveThrowable::TakeAHit_Implementation(FVector knockback_force, FAttackStats & attack_info, bool hitFromBehind, AActor* hiter)
{
	if (health > 0) {

		Super::TakeAHit_Implementation(knockback_force, attack_info, hitFromBehind, hiter);
		if (!GetWorldTimerManager().IsTimerActive(explosionTimer) && triggersExplosionTimerWhenHit) {

			GetWorldTimerManager().SetTimer(explosionTimer, this, &AExplosiveThrowable::DestroyThrowableActor, TimeToExplode, false);
			myMesh->SetMaterial(0, materialExplosion);

		}

	}

}
