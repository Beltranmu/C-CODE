// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ResetStateNotify.h"
#include "Core/BaseCharacter.h"

void UResetStateNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

  ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner());

  if (IsValid(character)) {
    character->resetStateStats();
  }
}