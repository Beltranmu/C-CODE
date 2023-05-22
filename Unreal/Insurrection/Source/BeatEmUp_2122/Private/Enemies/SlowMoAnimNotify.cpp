// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SlowMoAnimNotify.h"
#include "Core/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void USlowMoAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

  ABaseEnemy* tmpEnemy = Cast<ABaseEnemy>(MeshComp->GetOwner());

  if(IsValid(tmpEnemy) && tmpEnemy->IsValidLowLevel()) {

    if (IsValid(tmpEnemy->GetWorld())) {

      if (tmpEnemy->isHitByFinisher && tmpEnemy->isLastEnemyOfTheWave) {
        UWorld* tmpWorld = tmpEnemy->GetWorld();
        //UGameplayStatics::SetGlobalTimeDilation(tmpWorld, tmpEnemy->scaleFactor);  --> Reset GlobalTimeDilatation in EnemyCommander      
      }

    }

  }

}