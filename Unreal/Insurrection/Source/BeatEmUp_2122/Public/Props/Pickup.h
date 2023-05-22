// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class UStaticMeshComponent;

UENUM()
enum PickupType{
	kType_NONE = 0,
	kType_HPSMALL,
	kType_HPBIG,
	kType_AOE,
	kType_1UP,
	kType_DMGBOOST,
	kType_INVINCIBILITY,
	kType_MONEY
};

UCLASS()
class APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Type")
	TEnumAsByte<PickupType> type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		float hpRecoveryPercentage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		float dmgBoost;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		float dmgBoostDuration;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		float invencibleDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		int moneyAmmount = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		float moneyMultiplierVariance = 0.2f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ShowInteractButton();

};
