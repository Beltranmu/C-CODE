// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TimeDilatationNotify.h"
#include "Kismet/GameplayStatics.h"


void UTimeDilatationNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration){

  if (IsValid(MeshComp)) {
    UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), t);
  }
}

void UTimeDilatationNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void UTimeDilatationNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
  if (IsValid(MeshComp)) {
    UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 1);
  }
}
