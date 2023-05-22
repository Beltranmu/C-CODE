// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AnimNotifyAttackEnd.h"
#include "Core/BaseCharacter.h"

void UAnimNotifyAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (character) {

		//Just in case this is checked before CombatNotifyEnds
		character->_bComboTime = false;


		if (character->_onCombo) {

			FName nextAttack;
			UAnimInstance* currentAnimInstance = character->GetMesh()->GetAnimInstance();
			UAnimMontage* currentMontage;

			//Input for next attack (X, Y)
			if(character->inputArray.Num() > 0){

			  switch (character->inputArray[0]) {
			  case kInput_X:
				  nextAttack = character->_nextXInput;
				  //next input would be a combo or are we switching from a punch to a kick?
				  /*if (nextAttack == FName("None")) {
					  //character->_punching = true;
					  //Would be easier to just delete the input and maybe better for dessign, have to ask
					  character->inputArray.Empty();
				  }*/
				  break;
			  case kInput_Y:
				  nextAttack = character->_nextYInput;
				  //next input would be a combo or are we switching from a punch to a kick?
				  /*if (nextAttack == FName("None")) {
					  //character->_punching = false;
					  character->inputArray.Empty();
				  }*/
				  break;
			  }

      }
			if (character->_punching) {
				currentMontage = character->_highAttackAnimationMontage;
			}
			else {
				currentMontage = character->_lowAttackAnimationMontage;
			}

			//If there is no follow up, we do the same as if there was no combo input
			if (nextAttack == FName("None")) {

				character->_attackEnd = true;
				character->_nextYInput = FName("Default");
				character->_nextXInput = FName("Default");
				character->inputArray.Empty();

			}
			else {

				if (currentMontage)
					character->PlayAnimMontage(currentMontage, 1.f, nextAttack);
        character->rotationAngle = character->tmprotationAngle;
          character->rotationVector = FRotator(0.0f, character->tmprotationAngle, 0.0f);
        //Rotate player and scale mesh
        character->SetActorRotation(FQuat(character->rotationVector));
        FVector newScale = character->tmprotationAngle == 0.0f ? FVector(1.0f, 1.0f, 1.0f) : FVector(-1.0f, 1.0f, 1.0f);
        FRotator newRotator = character->tmprotationAngle == 0.0f ? FRotator(0.0f, 90.0f, 0.0f) : FRotator(0.0f, -90.0f, 0.0f);        
        character->GetMesh()->SetRelativeRotation(newRotator);
        character->GetMesh()->SetRelativeScale3D(newScale);
			
			}


			character->_onCombo = false;
			if (character->inputArray.Num() > 0) {
			
				character->inputArray.RemoveAt(0);
				character->inputArray.Shrink();

			}
			
		}
		else {

			character->_attackEnd = true;
			character->_nextYInput = FName("Default");
			character->_nextXInput = FName("Default");
			if (character->inputArray.Num() > 0) {

				if (character->inputArray[0] == kInput_Block) {
					character->startBlock();
				}

			}
			character->inputArray.Empty();

		}
    character->throwingBarrel = false;
    character->holdingObjectAnimation = false;
	}
	
}