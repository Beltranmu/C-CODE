// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Addons/RangedAgileProjectileAnimNotify.h"
#include "Core/BaseCharacter.h"
#include "Enemies/RangedAgile.h"

void URangedAgileProjectileAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

  ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner());
  ARangedAgile* rangedAgile = nullptr;
  if (nullptr != character)
    rangedAgile = Cast<ARangedAgile>(character);

  if (nullptr != rangedAgile) {
    rangedAgile->castProjectile = true;
   
  }

}