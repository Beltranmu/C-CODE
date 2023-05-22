// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/ThrowableActor.h"
#include "Interface/IDamagable.h"
#include "ThrowablePickupSpawner.generated.h"

/**
 * 
 */

class APickup;

UCLASS()
class BEATEMUP_2122_API AThrowablePickupSpawner : public AActor, public IIDamagable
{
	GENERATED_BODY()
		
public:
	AThrowablePickupSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float health = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyMesh")
		UStaticMeshComponent* myMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool CanBeDestroyed;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp|Spawn")
    bool spawnPickUpActivated;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp|Spawn")
    TArray<TSubclassOf<APickup>> pickupsToSpawn;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp|Ratio")
    TArray<int32> pickupRatio;
  UPROPERTY(VisibleAnywhere, Category = "PickUp|NumberElements")
    uint32 numberOfElements;

	void BeginPlay();

	void DestroyThrowableActor();

	UFUNCTION()
		virtual void TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter);


};
