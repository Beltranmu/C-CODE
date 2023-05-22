// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/BaseEnemy.h"
#include "PowerTank.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API APowerTank : public ABaseEnemy
{
	GENERATED_BODY()

public :
  APowerTank();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _powerAttackMontage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _fistOne;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _fistTwo;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _fistThree;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Punches")
    int32 chanceFirstPunch = 40;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Punches")
    int32 chanceSecondPunch = 40;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| Time")
    float pushAttackTime = 2.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    float pushAttackTriggerDistance = 100.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    float coolDown = 10.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    float firstSpecialAttackCoolDown = 3.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    bool specialAttackActivated = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats", meta = (ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "100.0"))
    int32 pushAttackprobability = 20.0f;

  FTimerHandle timerCooldown;


  virtual void BeginPlay() override;

  virtual void Tick(float DeltaTime) override;

  void selectAttack() override;

	
};
