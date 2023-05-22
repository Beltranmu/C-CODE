// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TimeDilatationNotify.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UTimeDilatationNotify : public UAnimNotifyState
{
  GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", DisplayName = "ScaleSlowFactor")
    float t;


   virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
      UAnimSequenceBase* Animation, float TotalDuration) override;

  void NotifyTick(USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation, float FrameDeltaTime) override;

  virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation) override;

};
