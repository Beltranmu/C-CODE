// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CustomAnim.h"

#include "Engine/Engine.h"
#include "Math/Vector.h"
#include "Core/BaseCharacter.h"
#include "Enemies/FinalBossCharacter.h"
/*#include "Player/TurboPlayer.h"*/
FName UCustomAnim::GetNextAttack(int input) {

  switch (input) {

  case 0:
    return _nextXInput;
    break;

  case 1:
    return _nextYInput;
    break;

  }

  return NAME_None;

}

void UCustomAnim::NativeBeginPlay() {

  Super::NativeBeginPlay();
}

void UCustomAnim::NativeUpdateAnimation(float DeltaSeconds) {

  Super::NativeUpdateAnimation(DeltaSeconds);

  if (nullptr == myCharacter) {
    myActor = GetOwningActor();
    myCharacter = Cast<ABaseCharacter>(myActor);
    boss = Cast<AFinalBossCharacter>(myActor);
    //turboplayer = Cast<ATurboPlayer>(myActor);
    
  }

  if (nullptr != myCharacter) {
    _health = myCharacter->health;
    _isJumping = !myCharacter->canJumpChar;
    _isBlocking = myCharacter->blockIsActive;
    _isAirKicking = myCharacter->_airAttackOnce;
    _isDahing = myCharacter->dashing;
    _isHoldingObject = myCharacter->holdingObjectAnimation;
    _isThrowingBarrel = myCharacter->throwingBarrel;
    _isDeath = myCharacter->death;
    _hitType = rand()%2;
    


    //_hit = myCharacter->attackStats.apply_hit;
    _characterAtributtes = myCharacter->state;
  }

  if (nullptr != boss) {
      _isFlying = boss->flying;
  }

  if (nullptr != myActor)
    _speed = myActor->GetVelocity().Size();

}