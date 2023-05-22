// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CombatNotifyState.h"
#include "Core/BaseCharacter.h"
#include "Player/CustomAnim.h"


void UCombatNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp,
  UAnimSequenceBase* Animation,
  float TotalDuration) {

  ABaseCharacter* Char = Cast<ABaseCharacter>(MeshComp->GetOwner());

  if (nullptr != Char) {

    Char->_bComboTime = true;
    Char->_onCombo = false;
    Char->_nextXInput = _nextXInput;
    Char->_nextYInput = _nextYInput;

    //If more than one input was used on the animation before, the combo should work, even if there is no input in this action.
	//Right now, this has been disabled as the max inputs the array can handle is 1 but maybe that changes again. 
    if (Char->inputArray.Num() > 0) {

        Char->_onCombo = true;
    
    }

  }

}

void UCombatNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp,
  UAnimSequenceBase* Animation) {

  ABaseCharacter* Char = Cast<ABaseCharacter>(MeshComp->GetOwner());

  if (nullptr != Char) {

    Char->_bComboTime = false;
    if (Char->_onCombo) {
      Char->_attackEnd = false;
	}
	else {
		//If no combo was executed in time we return these values to Default, so combo will start from scratch next time we attack.
		Char->_nextXInput = FName("Default");
		Char->_nextYInput = FName("Default");
	}
    
  }

}