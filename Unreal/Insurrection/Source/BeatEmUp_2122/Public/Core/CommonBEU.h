// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommonBEU.generated.h"

/**
 * 
 */


class UNiagaraSystem;
class UNiagaraComponent;

UENUM()
enum FCollisionType {

    kCollision_SPHERE = 0,
    kCollision_BOX,
    kCollision_CAPSULE

};

USTRUCT(BlueprintType)
struct FParticleData
{
  GENERATED_BODY()

    FParticleData() {
    
    }


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", meta = (DisplayName = "Particle System"))
    UParticleSystem* PSTemplate = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", meta = (DisplayName = "Particle System Niagara"))
    UNiagaraSystem * PSTemplateNiagara = nullptr;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", meta = (DisplayName = "Particle System Niagara"))
   UNiagaraComponent* particleComponentNiagara = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", meta = (DisplayName = "Particle System"))
    UParticleSystemComponent* particleComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    FVector locationOffset = FVector::ZeroVector;
  // Rotation offset from socket
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    FRotator rotationOffset = FRotator::ZeroRotator;
  UPROPERTY(EditAnywhere, Category = "Particle")
    FVector scale = FVector(1.0f, 1.0f, 1.0f);

  // Should attach to the bone/socket
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    bool attached = true; 	//~ Does not follow coding standard due to redirection from BP

    // SocketName to attach to
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    FName socketName;

 
};


USTRUCT(BlueprintType)
struct FAttackStats
{
  GENERATED_BODY()

    FAttackStats() {
      upper_body = false;
      lower_body = false;
      ultimate = false;
      airKick = false;

      blockeable = true;


      attack_dmg = 0.0f;
      attack_knockback = 0.0f;
      yKnockBackForce = 0.0f;

      apply_hit = true;
      apply_knock_down = false;
      apply_stun = false;

      time_apply_hit = 0.33f;
      time_apply_knock_down = 2.33f;
      time_apply_stun = 1.33f;
    }

    

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Area")
    bool upper_body;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Area")
    bool lower_body;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Area")
    bool ultimate;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Area")
	  bool airKick;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Area")
    bool blockeable;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    float attack_dmg;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    FVector knockbackForce = FVector(0.0f, 0.0f, 0.0f);;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    float attack_knockback;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    float yKnockBackForce;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    bool apply_hit ;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    bool apply_knock_down ;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    bool apply_stun ;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    float time_apply_hit;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    float time_apply_knock_down;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack stats")
    float time_apply_stun;
  
  ///You will have to modify the resetAttackStats() on the BaseCharacter if you add a new stat
};


USTRUCT(BlueprintType)
struct FCharacterStatus {

    GENERATED_BODY()

    FCharacterStatus() {

        hit = false;
        stun = false;
        knockdown = false;
        alteredStatus = false;
        standUp = false;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
        bool hit;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
        bool stun;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
        bool knockdown;
    
    //This one is true if any of the status above is true. Used to see if we can attack or things like that
    //using only one boolean
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
        bool alteredStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
      bool standUp;



};

USTRUCT(BlueprintType)
struct FUIData
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
    float health; // range 0 - 1

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIData")
    int imageID; //from image array
};

USTRUCT(BlueprintType)
struct FCameraLockConfig
{
    GENERATED_BODY()

    FCameraLockConfig() {

        lock = true;
        changeOffset = false;
        changeRotation = false;
        lockInPlace = false;

        newRotation = FRotator(-15.0f, 0.0f, 0.0f);
        offset = 700.0f;
        cameraLockPoint = nullptr;


    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Config")
    bool lock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Config")
    bool changeOffset;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Config")
    bool changeRotation;
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Config")
    bool lockInPlace;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Config")
    float offset;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Config")
    FRotator newRotation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Config")
    AActor* cameraLockPoint;

};

UENUM()
enum InputType {
    kInput_X = 0,
    kInput_Y,
    kInput_Block
};

UCLASS()
class BEATEMUP_2122_API UCommonBEU : public UObject
{
	GENERATED_BODY()
	
};
