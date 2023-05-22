// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ToggleLightNotify.h"
#include "Player/TurboPlayer.h"
#include "Components/PointLightComponent.h"

void UToggleLightNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

  ATurboPlayer* player = Cast<ATurboPlayer>(MeshComp->GetOwner());

  if (player) {

    int value = lightType.GetValue();
    
    switch(value) {

      case 0:
        player->_powerfulAttackPointLight->ToggleVisibility();
        player->_powerfulAttackPointLight->SetIntensity(0.f);
      break;

      case 1:
        player->_ultimateHeadPointLight->ToggleVisibility();
        //player->_ultimateHeadPointLight->SetIntensity(0.f);
      break;

      case 2:
        player->_ultimateRootPointLight->ToggleVisibility();
        //player->_ultimateRootPointLight->SetIntensity(0.f);
      break;

    }

  }

}