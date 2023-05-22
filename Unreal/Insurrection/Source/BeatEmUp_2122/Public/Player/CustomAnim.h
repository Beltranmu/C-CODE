// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Core/CommonBEU.h"
#include "CustomAnim.generated.h"

/**
 * 
 */
class ABaseCharacter;
class ATurboPlayer;
class AFinalBossCharacter;
UCLASS()
class BEATEMUP_2122_API UCustomAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:

    UFUNCTION()
    FName GetNextAttack(int input);

  virtual void NativeBeginPlay() override;
  virtual void NativeUpdateAnimation(float DeltaSeconds) override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
    FName _nextXInput;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
    FName _nextYInput;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    float _speed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    float _health;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool _isJumping;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool _isBlocking;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool _isAirKicking;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool _isDahing;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool _isHoldingObject;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool _isThrowingBarrel;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool _isFlying;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    FCharacterStatus _characterAtributtes;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Attributes")
    bool _isDeath;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Attributes")
    int _hitType;

  

  /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool _charginAttack;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Attributes")
    bool released_charged_attack;*/

  UPROPERTY()
    AActor* myActor;

  UPROPERTY()
    ABaseCharacter* myCharacter;

  UPROPERTY()
    AFinalBossCharacter* boss;

  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "owner")
/*    ATurboPlayer* turboplayer;*/

};
