// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CommonBEU.h"
#include "Interface/ThrowableInterface.h"
#include "Interface/IDamagable.h"
#include "ThrowableActor.generated.h"

class UStaticMeshComponent;
struct FTimerHandle;
class ATurboPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBarrelHit , int, enemyID, float, hpPercentage, int, points);
UCLASS()
class BEATEMUP_2122_API AThrowableActor : public AActor, public IThrowableInterface, public IIDamagable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyMesh")
		UStaticMeshComponent* myMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool CanBeDestroyed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool canDamagePlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float health = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float launchForce = 75000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float weight = 175.0f;
	//Will be normalized
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FVector throwAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool destroysOnCollisionWithTerrain = false;
	UPROPERTY(EditAnywhere, Category = "Stats")
		float timeToDestroy = 1.25f;
	FTimerHandle destroyTimer;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FAttackStats stats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool usePhysics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool needsRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FName boneName = "NONE";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FVector offset = FVector(0.0f,1.0f,0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float speedMultiplierWhenHeld = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
		bool isBeingHeld;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
		bool onAir;


  UPROPERTY(EditAnywhere, Category = "Player_ref")
    ATurboPlayer* ownerplayer;


  UPROPERTY(EditAnywhere, Category = "Player_ref")
		FOnBarrelHit hitEvent;

  UPROPERTY(EditAnywhere, Category = "ID of the player that pick you")
    uint32 playerPickID;

  //Bool rotated can be added so we can use at least the pick method in all the other classes.
  //So using "if" we can make it works for objects that need rotation like barrels and other that don't.
  //A FVector should be also added to see where the object should be placed when picking it, or a FName if sockets are used.



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void DestroyThrowableActor();

	UFUNCTION()
		virtual void Pick(AActor* self, ATurboPlayer* player) override;

	UFUNCTION()
		virtual void Throw(FVector dir) override;

	UFUNCTION()
		virtual void OverlapWithEnemy(AActor* self, AActor* other) override;

	UFUNCTION()
		virtual void OnHitBehaviour(AActor* self, AActor* other, FVector normalImpulse, const FHitResult& hit);

	UFUNCTION()
		virtual void TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter);

};
