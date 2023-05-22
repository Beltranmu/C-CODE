// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CommonBEU.h"
#include "HitNotifyState.generated.h"

/**
 * 
 */
class ABaseCharacter;
class AEnemyCommander;

UCLASS()
class BEATEMUP_2122_API UHitNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    bool attachedToHitLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", meta = (DisplayName = "Particle Hit"))
      FParticleData hitParticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", meta = (DisplayName = "Particle Spark"))
      FParticleData sparkParticleUp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", meta = (DisplayName = "Particle Spark"))
      FParticleData sparkParticleDown;


    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Time", meta = (DisplayName = "Activate Time Dilatation on Hit"))
      bool t;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Slow Motion")
      FTimerHandle timerRestartNormalTimeDilatation;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Time")
      float restartTimeDilatationValue = 1.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
      bool finisher = false;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Time", meta = (DisplayName = "Time Scale Slow Factor"))
      float scaleFactor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (ExposeOnSpawn = true))
      USoundBase* Sound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (ExposeOnSpawn = true))
      float VolumeMultiplier = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (ExposeOnSpawn = true))
      float PitchMultiplier = 1.0f;

    


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Stats")
		FAttackStats attackStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Stats")
        TEnumAsByte<FCollisionType> collisionType;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Stats")
    FVector attackPosition;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Stats")
      FRotator hitboxRotation;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Stats")
    float radius;

  UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Attack Stats")
    FVector boxSize;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Stats")
      FVector capsuleSize;

  bool alreadyHit ;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Stats")
    bool isChargedAttack = false;



private:

	ABaseCharacter* character;

  FVector attackLocation;
  FVector hitLocation;
  FVector characterRight;
  bool timeDilatation;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, float TotalDuration) override;

	void NotifyTick(USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation) override;

};
