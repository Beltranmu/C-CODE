// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AgileEnemy.h"
#include "AIController.h"
#include "Engine/Engine.h"
#include "Player/TurboPlayer.h"
#include "Player/DestinationPoint.h"
#include "NavigationSystem.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AAgileEnemy::AAgileEnemy(){
  _state2 = kState_GoToEdge;

  enemyType = TypeOfEnemy_Agile;
}

void AAgileEnemy::BeginPlay(){

  Super::BeginPlay();
  
  
}

void AAgileEnemy::EndPlay(const EEndPlayReason::Type type)
{
  Super::EndPlay(type);
}

void AAgileEnemy::Tick(float DeltaTime){

  Super::Tick(DeltaTime);

  StateMachine2(kInput_Y);


}

void AAgileEnemy::specialThink(){
  if(dashAttackActivate && getRandomNumber){

    float dist = (GetLocationTarget() - GetActorLocation()).Size();
    bool dashAttack = dist <= maxdashAttacDistance;
    dashAttack = dashAttack && mindashAttacDistance <= dist;
    int n = FMath::RandRange(0, 1000);
    dashAttack = dashAttack && n < dashAttackProbability;
    if(!dashAttack) {
      GetWorld()->GetTimerManager().SetTimer(timerDashRandomNumber, this, &AAgileEnemy::resetRandomNumber, timerGetNewRandomPointDashAttack, false);
      getRandomNumber = false;
    }
    dashAttack = dashAttack && _state2 != kState_ChargingAttack && _state2 == kState_Chase2;
    dashAttack = dashAttack && !dashAttackInProgres;


    // MRU
    // xf = x0 + v * t
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

    FVector locationOffset = (finalLocation - GetLocationTarget())/2.f;
    intermediateLocation = initialLocation + dir * dashDuration * 0.33f;
    intermediateLocation2 = initialLocation + dir * dashDuration * 0.66f;

    UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    FNavLocation navLocation;

    bool pointIsInNavMesh = navSystem->ProjectPointToNavigation(GetLocationTarget(), navLocation);
    pointIsInNavMesh = pointIsInNavMesh && navSystem->ProjectPointToNavigation(finalLocation, navLocation);
    pointIsInNavMesh = pointIsInNavMesh && navSystem->ProjectPointToNavigation(intermediateLocation, navLocation);
    pointIsInNavMesh = pointIsInNavMesh && navSystem->ProjectPointToNavigation(intermediateLocation2, navLocation);
    
    if (dashAttack && pointIsInNavMesh) {
      _state2 = kState_ChargingAttack;
      dashAttackInProgres = true;
      dashAttackActivate = false;
      aiController->StopMovement();
      GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
      GetWorld()->GetTimerManager().SetTimer(pausetimerDashAttack, this, &AAgileEnemy::startDashAttack, waitDashAttackTime, false);
      GetWorld()->GetTimerManager().SetTimer(timerDashAttack, this, &AAgileEnemy::resetDashAttack, waitDashAttackTime + dashDuration, false);
      _charactersHit.Reset();
      PlayAnimMontage(_powerAttackAnimtionMontage, 1.0f, FName("Default"));
    }
  }
}


void AAgileEnemy::startDashAttack() {

  _state2 = kState_SpecialBehaviour;
  FVector dashAttackDirection = (GetLocationTarget() - GetActorLocation()).GetSafeNormal();

  _mainCharacter[playerIndex]->targetPoint[pointIndex]->isFree = true;
  dashAttackInProgres = true;
  Dash(dashAttackDirection.GetSafeNormal());  

  PlayAnimMontage(_powerAttackAnimtionMontage, 2.0f, FName("Repeat"));

}


void AAgileEnemy::specialBehavior(){




}

void AAgileEnemy::resetDashAttack(){

  GetCharacterMovement()->MaxWalkSpeed = 475.f;

  GetWorld()->GetTimerManager().SetTimer(coolDownTimerPushAttack, this, &AAgileEnemy::activateDashAttack, cooldownDashAttackTime, false);
  PlayAnimMontage(_powerAttackAnimtionMontage, 2.0f, FName("End"));

  _state2 = kState_StandBy;

  GetWorld()->GetTimerManager().ClearTimer(pauseTimerDashAttack);
  GetWorld()->GetTimerManager().ClearTimer(timerDashAttack);
  dashAttackInProgres = false;

  edgePointHasBeenAssigned = false;
  pointHasBeenAssigned = false;
  pointOnAttackAssigned = false;
  //_mainCharacter[playerIndex]->targetPoint[pointIndex]->isFree = true;
  GetWorld()->GetTimerManager().SetTimer(_timerCheckDangerAreaDistance, this, &ABaseEnemy::EnterInDangerZone, _timerCheckDangerAreaDistanceValue, true);

  resetAttack = true;

}

void AAgileEnemy::resetRandomNumber() { getRandomNumber = true;}

void AAgileEnemy::ResetEnemy() {
  Super::ResetEnemy();
  dashAttackActivate = false;
  GetWorld()->GetTimerManager().SetTimer(coolDownTimerDashAttack, this, &AAgileEnemy::activateDashAttack, firstCooldownDashAttackTime, false);
}

void AAgileEnemy::OnDeathImpulse() {

  Super::OnDeathImpulse();
    
  /*deathImpulse.Y *= GetActorRightVector().Y * -1;

  LaunchCharacter(deathImpulse, true, true);*/

}


void AAgileEnemy::activateDashAttack() {

  dashAttackActivate = true;

}

void AAgileEnemy::selectAttack() {

  if (rand() % 2 == 0) {
    _lowAttackAnimationMontage = _kickOne;
  }
  else {
    _lowAttackAnimationMontage = _kickTwo;
  }

}