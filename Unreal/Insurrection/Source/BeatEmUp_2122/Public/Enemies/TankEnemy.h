// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/BaseEnemy.h"
#include "TankEnemy.generated.h"

/**
 * 
 */

UCLASS()
class BEATEMUP_2122_API ATankEnemy : public ABaseEnemy
{
  GENERATED_BODY()

public:

  ATankEnemy();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _fistOne;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _fistTwo;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| Time")
    float pushAttackTime = 2.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| Time")
    float waitPushAttackTime = 1.0f;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| Time")
    bool pushAttackInProgres = false;;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    float dmg = 35.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    float explosionforce = 450.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    float pushAttackTriggerDistance = 100.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    float coolDown = 10.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    float firstPushAttackCoolDown = 3.0f;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
    bool pushAttackActivate = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats", meta = (ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "100.0"))
    int32 pushAttackprobability = 50.0f;



  FTimerHandle timerPushAttack;
  FTimerHandle pauseTimerPushAttack;
  FTimerHandle coolDownTimerPushAttack;
  FTimerHandle timerDashRandomNumber;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack| Direction")
  FVector pushAttackDirection;

  FVector finalLocation = FVector::ZeroVector;
  FVector intermediateLocation = FVector::ZeroVector;
  FVector intermediateLocation2 = FVector::ZeroVector;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Attack")
    float timerGetNewRandomPointDashAttack = 1.f;
  bool getRandomNumber = true;

  virtual void BeginPlay() override;

  virtual void Tick(float DeltaTime) override;

  virtual void specialThink() override;
  virtual void specialBehavior() override;
  virtual void ResetEnemy() override;

  virtual void HitDamagable(AActor* otherActor) override;

  UFUNCTION()
    void resetPushAttack(); 

  UFUNCTION()
    void activatePushAttack();
    
  UFUNCTION()
    void StartPushAttack();

  UFUNCTION()
    void resetRandomNumber();

  void selectAttack() override;

  
};
