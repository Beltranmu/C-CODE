// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/ThrowableActor.h"
#include "ExplosiveThrowable.generated.h"

struct FParticleData;

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API AExplosiveThrowable : public AThrowableActor
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float explosionRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool triggersExplosionTimerWhenHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float TimeToExplode = 1.5f;

	FTimerHandle explosionTimer; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool applySlow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float slowPercentage = 33.3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float slowDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		UMaterialInterface* materialExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		FParticleData explosionParticle;

	virtual void DestroyThrowableActor() override;

	virtual void OverlapWithEnemy(AActor* self, AActor* other) override;

	virtual void TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter) override;

};
