// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/TankEnemy.h"
#include "AIController.h"
#include "Player/TurboPlayer.h"
#include "Player/DestinationPoint.h"
#include "NavigationSystem.h"

ATankEnemy::ATankEnemy() {
  _state2 = kState_GetLocation;
  enemyType = TypeOfEnemy_Tank;
  unostopable = true;
}

void ATankEnemy::BeginPlay() {

  Super::BeginPlay();
  meshMaterial1 = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);
  GetMesh()->SetMaterial(1, meshMaterial1);
  complementHipMeshesMaterials.Add(meshMaterial1);

  meshMaterial2 = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(2), this);
  GetMesh()->SetMaterial(2, meshMaterial2);
  complementHipMeshesMaterials.Add(meshMaterial2);

  //pushAttackActivate = true;
}

void ATankEnemy::Tick(float DeltaTime) {

  Super::Tick(DeltaTime);

  FRotator newRotator = rotationAngle == 0.0f ? FRotator(0.0f, 90.0f, 0.0f) : FRotator(0.0f, -90.0f, 0.0f);
  GetMesh()->SetRelativeRotation(newRotator);

  StateMachine2(kInput_X);
  




  meshMaterial1->SetScalarParameterValue(FName("Fade"), fade);
  meshMaterial1->SetScalarParameterValue(FName("DeathFade"), dfade);
  meshMaterial2->SetScalarParameterValue(FName("Fade"), fade);
  meshMaterial2->SetScalarParameterValue(FName("DeathFade"), dfade);


}


void ATankEnemy::specialThink(){
  if(pushAttackActivate){

    float dist = (GetLocationTarget() - GetActorLocation()).Size();
    bool pushAttack = dist >= pushAttackTriggerDistance;
    int n = FMath::RandRange(0, 100);
    pushAttack = pushAttack && n < pushAttackprobability;
    pushAttack = pushAttack && _state2 != kState_ChargingAttack && _state2 == kState_Chase2;
    pushAttack = pushAttack && !pushAttackInProgres;
    
    if (!pushAttack) {
      GetWorld()->GetTimerManager().SetTimer(timerDashRandomNumber, this, &ATankEnemy::resetRandomNumber, timerGetNewRandomPointDashAttack, false);
      getRandomNumber = false;
    }

    FVector dashAttackDirection = (GetLocationTarget() - GetActorLocation()).GetSafeNormal();
    FVector dir = dashAttackDirection;

    dir.Z = 0.0f;

    if (dir.IsNearlyZero()) {
      dir = this->GetActorForwardVector();

      //Forward vector is rotated, which is really fun so i have to make this.

      float aux = dir.X;
      dir.X = dir.Y;
      dir.Y = aux;

    }

    dir.Normalize();
    dir *= dashSpeed;

    FVector initialLocation = GetActorLocation();
    finalLocation = initialLocation + dir * dashDuration;

    FVector locationOffset = (finalLocation - GetLocationTarget()) / 2.f;
    intermediateLocation = initialLocation + dir * dashDuration * 0.33f;
    intermediateLocation2 = initialLocation + dir * dashDuration * 0.66f;

    UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    FNavLocation navLocation;

    bool pointIsInNavMesh = navSystem->ProjectPointToNavigation(GetLocationTarget(), navLocation);
    pointIsInNavMesh = pointIsInNavMesh && navSystem->ProjectPointToNavigation(finalLocation, navLocation);
    pointIsInNavMesh = pointIsInNavMesh && navSystem->ProjectPointToNavigation(intermediateLocation, navLocation);
    pointIsInNavMesh = pointIsInNavMesh && navSystem->ProjectPointToNavigation(intermediateLocation2, navLocation);

    if (pushAttack) {
      _state2 = kState_ChargingAttack;
      pushAttackActivate = false;
      aiController->StopMovement();
      GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
      GetWorld()->GetTimerManager().SetTimer(pauseTimerPushAttack, this, &ATankEnemy::StartPushAttack, waitPushAttackTime, false);
      GetWorld()->GetTimerManager().SetTimer(timerPushAttack, this, &ATankEnemy::resetPushAttack, waitPushAttackTime + dashDuration, false);
      _charactersHit.Reset();
      PlayAnimMontage(_powerAttackAnimtionMontage, 1.0f, FName("Default"));
    }
  }
}

void ATankEnemy::StartPushAttack(){

  _mainCharacter[playerIndex]->targetPoint[pointIndex]->isFree = true;
  pushAttackInProgres = true;
  _state2 = kState_SpecialBehaviour;
  FVector dashAttackDirection = (GetLocationTarget() - GetActorLocation()).GetSafeNormal();
  Dash(dashAttackDirection.GetSafeNormal());
  PlayAnimMontage(_powerAttackAnimtionMontage, 2.0f, FName("Repeat"));

}

void ATankEnemy::specialBehavior(){


  //COLLISIONs

 
}

void ATankEnemy::ResetEnemy(){
  Super::ResetEnemy();
  pushAttackActivate = false;
  GetWorld()->GetTimerManager().SetTimer(coolDownTimerPushAttack, this, &ATankEnemy::activatePushAttack, firstPushAttackCoolDown, false);
}

void ATankEnemy::activatePushAttack() {

  pushAttackActivate = true;

}

void ATankEnemy::resetPushAttack(){

  GetWorld()->GetTimerManager().SetTimer(coolDownTimerPushAttack, this, &ATankEnemy::activatePushAttack, coolDown, false);

  _state2 = kState_StandBy;

  PlayAnimMontage(_powerAttackAnimtionMontage, 2.0f, FName("End"));

  GetWorld()->GetTimerManager().ClearTimer(pauseTimerPushAttack);
  GetWorld()->GetTimerManager().ClearTimer(timerPushAttack);
  pushAttackInProgres = false;
  edgePointHasBeenAssigned = false;
  pointHasBeenAssigned = false;
  pointOnAttackAssigned = false;
  resetAttack = true;
  GetWorld()->GetTimerManager().SetTimer(_timerCheckDangerAreaDistance, this, &ABaseEnemy::EnterInDangerZone, _timerCheckDangerAreaDistanceValue, true);
}

void ATankEnemy::resetRandomNumber() { getRandomNumber = true; }

void ATankEnemy::HitDamagable(AActor* otherActor) {


  if (alive) {
    if (pushAttackInProgres) {

      float actorExplosionDirection = 45;
      float explosionPower = (1.0f / (otherActor->GetActorLocation() - this->GetActorLocation()).Size()) * explosionforce * 100;
      if ((otherActor->GetActorLocation().Y - this->GetActorLocation().Y) < 0.0f) {
        actorExplosionDirection = 180 - 45;
      }
      float angle = FMath::DegreesToRadians(actorExplosionDirection);
      FVector explosionVector = FVector(0.0f, explosionPower * FMath::Cos(angle), explosionPower * FMath::Sin(angle));


      //HERE FOR MORE ACTION FOR THE EXPLOSION
      attackStats.attack_dmg = dmg;
      //attackStats.attack_knockback = explosionForce;
      ABaseEnemy* hitEnemy = Cast<ABaseEnemy>(otherActor);
      if (hitEnemy) {
        hitEnemy->hitByPlayerID = hitByPlayerID;
      }
      IIDamagable::Execute_TakeAHit(otherActor, explosionVector, attackStats, false, this);
    }else{
      Super::HitDamagable(otherActor);
    }
  }
}

void ATankEnemy::selectAttack() {

  if (rand() % 2 == 0) {
    _highAttackAnimationMontage = _fistOne; 
  }
  else {
    _highAttackAnimationMontage = _fistTwo;
  }

}