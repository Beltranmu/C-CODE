// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/AgileEnemy.h"
#include "Enemies/Addons/Projectile.h"
#include "RangedAgile.generated.h"

/**
 *
 */
UCLASS()
class BEATEMUP_2122_API ARangedAgile : public AAgileEnemy
{
	GENERATED_BODY()

public:

	ARangedAgile();

	UPROPERTY(EditAnywhere, Category = "Long-Range Attack")
		int32 chanceToAttack;

  UPROPERTY(VisibleAnywhere, Category = "Long-Range Attack")
    FTimerHandle timerNextLongAttack;

  UPROPERTY(EditAnywhere, Category = "Long-Range Attack")
    float timerNextLongAttackValue;

  UPROPERTY(VisibleAnywhere, Category = "Long-Range Attack")
    bool waitingForNextLongAttack;

  UPROPERTY(EditAnywhere, Category = "Long-Range Attack")
    float projectileSpeed;

  UPROPERTY(EditAnywhere, Category = "Long-Range Attack")
    TSubclassOf<AProjectile> BP_Projectile;

  UPROPERTY(EditAnywhere, Category = "Long-Range Attack")
    UAnimMontage* projectileAnimation;

  bool castProjectile = false;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void EndPlay(const EEndPlayReason::Type type) override;

	void specialThink() override;
	void specialBehavior() override;
	void ResetLongAttack();

  void rangedAttack();

  void selectAttack() override;

};
