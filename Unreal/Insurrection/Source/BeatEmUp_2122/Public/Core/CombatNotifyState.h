// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CombatNotifyState.generated.h"

/**
 * 
 */
class UCustomAnim;
UCLASS()
class BEATEMUP_2122_API UCombatNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()


public:

    //Sets combo time so the player can make inputs to determine next attacks, if there are inputs in the array
    //from the last animation it uses them even if no input is made in this animation.
   virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
      UAnimSequenceBase* Animation,
      float TotalDuration) override;

  virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation) override;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Anim Instance")
    UCustomAnim* _animInstance;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
    FName _nextXInput;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
    FName _nextYInput;
	
};
