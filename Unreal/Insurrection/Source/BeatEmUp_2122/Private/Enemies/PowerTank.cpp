// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/PowerTank.h"
#include "AIController.h"
#include "Player/DestinationPoint.h"

APowerTank::APowerTank() {
  _state2 = kState_GetLocation;
  enemyType = TypeOfEnemy_Tank;
  specialAttackActivated = false;
  unostopable = true;
}


void APowerTank::BeginPlay() {

  Super::BeginPlay();

  meshMaterial1 = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);
  GetMesh()->SetMaterial(1, meshMaterial1);
  complementHipMeshesMaterials.Add(meshMaterial1);

  meshMaterial2 = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(2), this);
  GetMesh()->SetMaterial(2, meshMaterial2);
  complementHipMeshesMaterials.Add(meshMaterial2);

}

void APowerTank::Tick(float deltaTime) {

  Super::Tick(deltaTime);

  FRotator newRotator = rotationAngle == 0.0f ? FRotator(0.0f, 90.0f, 0.0f) : FRotator(0.0f, -90.0f, 0.0f);
  GetMesh()->SetRelativeRotation(newRotator);

  StateMachine2(kInput_X);


  meshMaterial1->SetScalarParameterValue(FName("Fade"), fade);
  meshMaterial1->SetScalarParameterValue(FName("DeathFade"), dfade);
  meshMaterial2->SetScalarParameterValue(FName("Fade"), fade);
  meshMaterial2->SetScalarParameterValue(FName("DeathFade"), dfade);

}

void APowerTank::selectAttack() {

  int32 randomNumber = rand()%100;

  if (randomNumber < chanceFirstPunch) {
    _highAttackAnimationMontage = _fistOne;
  }
  else if (chanceFirstPunch <= randomNumber && randomNumber < chanceFirstPunch+chanceSecondPunch) {
    _highAttackAnimationMontage = _fistTwo;
  }else{
    _highAttackAnimationMontage = _fistThree;
  }

}