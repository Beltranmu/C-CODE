// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/BaseEnemy.h"
#include "MasillaEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API AMasillaEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:

	AMasillaEnemy();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _fistOne;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _fistTwo;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    bool explosionBehaviour;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    float explositonDirection = 3;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    float explosionRadius = 90.0f;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    float explosionForce = 900.0f;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    float explosionDMG;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    float timeOfExplosion = 2.0f;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    FTimerHandle timerExplosion;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    FVector offsetExplosion  ;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    FTimerHandle timerChangeColorExplosion;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    float timeChangeColor;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    FLinearColor explosionColor;
  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Behaviour|Explosion")
    short colorExplosion;

    //Death Particle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles", meta = (DisplayName = "Particle System | Explosion"))
      FParticleData PSTemplateWarningExplsion1;
    //Death Particle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles", meta = (DisplayName = "Particle System | Explosion"))
      FParticleData PSTemplateWarningExplsion2;
    



	virtual void BeginPlay() override;
  void EndPlay(const EEndPlayReason::Type type) override;

	virtual void Tick(float DeltaTime) override;
  /** INTERFACE */
  virtual void TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter) override;

  virtual void OnDeathImpulse() override;


  void ResetEnemy() override;

  void setNewStyle() override;

  UFUNCTION()
    void Explode();

  UFUNCTION()
    void ChangeColorExplosion();

  virtual void selectAttack() override;

};
