// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotifyCameraShake.h"
#include "Core/GameInstanceBEU.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
  if (IsValid(MeshComp->GetOwner())) {
    UGameInstanceBEU* myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(MeshComp->GetOwner()->GetWorld()));

    if (nullptr != myGI) {

      myGI->camera->ShakeCamera(shakeIntensity);

    }
  }

}
