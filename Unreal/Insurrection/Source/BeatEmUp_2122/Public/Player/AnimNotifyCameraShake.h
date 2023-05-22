// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifyCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UAnimNotifyCameraShake : public UAnimNotify
{
	GENERATED_BODY()

  public:

    UPROPERTY(EditAnywhere, Category = "Camera Shake")
      float shakeIntensity;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
