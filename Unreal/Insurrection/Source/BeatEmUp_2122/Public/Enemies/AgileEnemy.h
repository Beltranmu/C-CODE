// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/BaseEnemy.h"
#include "AgileEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API AAgileEnemy : public ABaseEnemy
{
	GENERATED_BODY()

	public:

		AAgileEnemy();

    FVector finalLocation = FVector::ZeroVector;
    FVector intermediateLocation = FVector::ZeroVector;
    FVector intermediateLocation2 = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
      UAnimMontage* _kickOne;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
      UAnimMontage* _kickTwo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DashAttack| AttackStats")
      float maxdashAttacDistance = 1000.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DashAttack| AttackStats")
      float mindashAttacDistance = 200.0f;;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DashAttack| AttackStats")
      float waitDashAttackTime = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DashAttack| AttackStats")
      float cooldownDashAttackTime = 10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DashAttack| AttackStats")
      float firstCooldownDashAttackTime = 3.0f;   
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
      bool dashAttackActivate = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Special Attack| AttackStats")
      bool dashAttackInProgres = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DashAttack| AttackStats", meta = (ClampMin = "1.0", ClampMax = "1000.0", UIMin = "1.0", UIMax = "1000.0"))
      int32 dashAttackProbability = 100.0f;

    FTimerHandle pausetimerDashAttack;
    FTimerHandle coolDownTimerDashAttack;
    FTimerHandle timerDashAttack;
    FTimerHandle timerDashRandomNumber;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Attack")
      float timerGetNewRandomPointDashAttack = 1.f;
    bool getRandomNumber = true;

    FTimerHandle pauseTimerDashAttack;
    FTimerHandle coolDownTimerPushAttack;

		virtual void BeginPlay() override;
		virtual void EndPlay(const EEndPlayReason::Type type) override;

		virtual void Tick(float DeltaTime) override;

    UFUNCTION()
      void startDashAttack();

    UFUNCTION()
      void resetDashAttack();

    UFUNCTION()
      void resetRandomNumber();

    UFUNCTION()
      void activateDashAttack();

    virtual void specialThink() override;
    virtual void specialBehavior() override;
    virtual void ResetEnemy() override;

    virtual void OnDeathImpulse() override;

    virtual void selectAttack() override;
	
};
