// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/RangedAgile.h"
#include "Enemies/Addons/Projectile.h"
#include "Player/TurboPlayer.h"
#include "Player/DestinationPoint.h"
#include "AIController.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARangedAgile::ARangedAgile() {

  enemyType = TypeOfEnemy_AgileModified;
  middlePointWeight = 80;

}

void ARangedAgile::BeginPlay() {

  Super::BeginPlay();

  timerNextLongAttackValue = 2.f;
  waitingForNextLongAttack = true;

}

void ARangedAgile::Tick(float DeltaTime) {

  Super::Tick(DeltaTime);

  if (castProjectile) {
    castProjectile = false;
    rangedAttack();
  }

}

void ARangedAgile::EndPlay(const EEndPlayReason::Type type) {
}

void ARangedAgile::specialThink() {

  if(chanceToAttack <= 0)
    chanceToAttack = 1;

  FVector myPosition = GetActorLocation();
  FVector pointPosition = _mainCharacter[playerIndex]->targetPoint[pointIndex]->GetComponentLocation() - FVector(0.f, 0.f, 90.f);

  bool inPosition = (myPosition - pointPosition).IsNearlyZero(50.f);
  bool frontPointFree = _mainCharacter[playerIndex]->targetPoint[0]->isFree;
  bool backPointFree = _mainCharacter[playerIndex]->targetPoint[1]->isFree;
  int randomNumber = rand() % 101 <= chanceToAttack;

  if (waitingForNextLongAttack && (pointIndex == 6 || pointIndex == 7) && randomNumber &&
     !frontPointFree && !backPointFree  && inPosition) {

    

    _state2 = kState_SpecialBehaviour;
    aiController->StopMovement();
    waitingForNextLongAttack = false;

    edgePointHasBeenAssigned = false;
    pointHasBeenAssigned = false;
    pointOnAttackAssigned = false;

    _mainCharacter[playerIndex]->targetPoint[pointIndex]->isFree = true;

    //GetWorldTimerManager().SetTimer(timerNextLongAttack, this, &ARangedAgile::ResetLongAttack, timerNextLongAttackValue, false);
    
  }

}

void ARangedAgile::specialBehavior() {
 
  PlayAnimMontage(projectileAnimation, 1.f);
  _state2 = kState_StandBy;
  
}

void ARangedAgile::ResetLongAttack() {

  waitingForNextLongAttack = true;
  //GetWorld()->GetTimerManager().SetTimer(_timerCheckDangerAreaDistance, this, &ABaseEnemy::EnterInDangerZone, _timerCheckDangerAreaDistanceValue, true);

}

void ARangedAgile::rangedAttack() {

  FVector v3Dir = (_mainCharacter[playerIndex]->GetActorLocation() - GetActorLocation()).GetSafeNormal();

  FTransform myTR;
  myTR.SetRotation(FQuat::Identity);
  myTR.SetLocation(GetActorLocation() + GetActorRightVector() * 100.f);

  AProjectile* tmpProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(BP_Projectile, myTR);

  tmpProjectile->projectileMoveComp->InitialSpeed = projectileSpeed;
  tmpProjectile->projectileMoveComp->Velocity = v3Dir;
  tmpProjectile->projectileMoveComp->ProjectileGravityScale = 0.f;

  tmpProjectile->FinishSpawning(myTR);

  GetWorldTimerManager().SetTimer(timerNextLongAttack, this, &ARangedAgile::ResetLongAttack, timerNextLongAttackValue, false);

}

void ARangedAgile::selectAttack() {

  if (rand() % 2 == 0) {
    _lowAttackAnimationMontage = _kickOne;
  }
  else {
    _lowAttackAnimationMontage = _kickTwo;
  }

}