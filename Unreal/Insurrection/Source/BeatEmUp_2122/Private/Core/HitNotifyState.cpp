// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HitNotifyState.h"
#include "Engine/Engine.h"
#include "Core/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Interface/IDamagable.h"
#include "Interface/EnemyDataUI.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GamemodeBEU.h"
#include "Particles/ParticleSystem.h"
#include "Core/BaseEnemy.h"

void UHitNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp,
  UAnimSequenceBase* Animation, float TotalDuration) {
  /**/
  Super::NotifyBegin(MeshComp, Animation, TotalDuration);
  character = Cast<ABaseCharacter>(MeshComp->GetOwner());
  ABaseEnemy* _enemy = Cast<ABaseEnemy>(character);

  if (nullptr != character) {
    AGamemodeBEU* gmBEU = Cast<AGamemodeBEU>(UGameplayStatics::GetGameMode(character->GetWorld()));
    character->attackStats = attackStats;
    characterRight = character->GetActorRightVector();

    
    timeDilatation = t;

    if(isChargedAttack)
      attackPosition = FVector(0.f, 35.f + character->deltaY * boxSize.Y, 30.f);

    //if (nullptr != _enemy) {
    //
    //  if (gmBEU->gameDifficulty == Difficulty::kDificulty_Normal) {
    //
    //    attackStats.attack_dmg *= 1.5f;
    //
    //  }
    //  else if (gmBEU->gameDifficulty == Difficulty::kDificulty_Hard) {
    //
    //    attackStats.attack_dmg *= 2.f;
    //
    //  }
    //
    //}

  }

  hitLocation = FVector(0.0f, 0.0f, 0.0f);
  
}

void UHitNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp,
  UAnimSequenceBase* Animation){
  /**/
  Super::NotifyEnd(MeshComp, Animation);

  if (IsValid(character) && character->IsValidLowLevel()) {

    character->resetAttackStats();

    character->_charactersHit.Empty();
    character->_bonesHit.Reset();

  }
  alreadyHit = false;
}

void UHitNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime){
  Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

  
  if(IsValid(character) && character->IsValidLowLevel()){
    if (IsValid(character->GetWorld())) {

      attackLocation = character->GetActorLocation() + (attackPosition * characterRight);
      attackLocation.Z += attackPosition.Z;

      FCollisionQueryParams collparams(FName(TEXT("UpperSphere")), false);
      collparams.AddIgnoredActor(character);
      TArray<FOverlapResult> hitted_actors;

      FCollisionShape collisionShape;
      switch (collisionType) {

      case kCollision_SPHERE:
        collisionShape = FCollisionShape::MakeSphere(radius);
        break;
      case kCollision_BOX:
        if(isChargedAttack)
          collisionShape = FCollisionShape::MakeBox(FVector(boxSize.X, character->deltaY * boxSize.Y, boxSize.Z));
        else
          collisionShape = FCollisionShape::MakeBox(boxSize);
        break;
      case kCollision_CAPSULE:
        collisionShape = FCollisionShape::MakeCapsule(capsuleSize);
        break;

      }
            
      hitLocation = attackLocation;

      FVector startLocation = attackLocation - characterRight * radius;
      FVector endLocation = startLocation + characterRight * 2.0f * radius;
      FCollisionQueryParams collparamsLine(FName(TEXT("ImpulseBoneLine")), false);
      collparamsLine.AddIgnoredActor(character);
      TArray <FHitResult> hitResult;
      const int kNreps = 3;
      for (int r = 0; r < kNreps; ++r) {

        startLocation = attackLocation + (r - kNreps) * FVector(5.0f, 0.0f, 0.0f);
        endLocation = startLocation + characterRight * radius;

        if (character->GetWorld()->LineTraceMultiByChannel(hitResult, startLocation, endLocation,
          character->collisionAttackPreset, collparamsLine) && !alreadyHit) {

          for (int i = 0; i < hitResult.Num(); ++i) {
            if (INDEX_NONE == character->_bonesHit.Find(hitResult[i].BoneName)) {

              hitLocation = hitResult[i].ImpactPoint + characterRight * 15;

            }
          }
        }
      }

      if (character->GetWorld()->OverlapMultiByChannel(hitted_actors,
        attackLocation, FQuat(hitboxRotation), character->collisionAttackPreset, collisionShape, collparams)) {
        for (int i = 0; i < hitted_actors.Num(); ++i) {
          if (INDEX_NONE == character->_charactersHit.Find(hitted_actors[i].GetActor())) {

            AActor* OtherActor = hitted_actors[i].GetActor();
            if (OtherActor) {
              float hitHeigh = attackLocation.Z - OtherActor->GetActorLocation().Z + 10;
              bool hitedUp = hitHeigh >= 0;

              character->SpawnParticleSystem(hitParticle, attachedToHitLocation, hitLocation);

              sparkParticleUp.rotationOffset.Yaw = characterRight.Y == 1.0f ? 180.0f : 0.0f;
              sparkParticleDown.rotationOffset.Yaw = characterRight.Y == 1.0f ? 180.0f : 0.0f;

              character->SpawnParticleSystem(sparkParticleUp, attachedToHitLocation, hitLocation - characterRight * 10);
              character->SpawnParticleSystem(sparkParticleDown, attachedToHitLocation, hitLocation - characterRight * 10);

              UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), Sound, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);

                

              character->_charactersHit.Add(OtherActor);
              if (OtherActor) {

                if (OtherActor->GetClass()->ImplementsInterface(UEnemyDataUI::StaticClass())) {
                  character->HitUIUpdate(OtherActor);
                }
                if (OtherActor->GetClass()->ImplementsInterface(UIDamagable::StaticClass())) {
                  character->HitDamagable(OtherActor);
                }
              }
              
              ABaseEnemy* tmpEnemy = Cast<ABaseEnemy>(OtherActor);

              if (nullptr != tmpEnemy && tmpEnemy->health <= 0 &&
                  timeDilatation && finisher && tmpEnemy->isLastEnemyOfTheWave) {
                timeDilatation = false;
                UGameplayStatics::SetGlobalTimeDilation(tmpEnemy->GetWorld(), scaleFactor);
                tmpEnemy->GetWorldTimerManager().SetTimer(timerRestartNormalTimeDilatation, tmpEnemy, &ABaseEnemy::ResetTimeDilatation, scaleFactor * restartTimeDilatationValue, false);
              }

            }
          }
        }
      }
    }
  }
}


