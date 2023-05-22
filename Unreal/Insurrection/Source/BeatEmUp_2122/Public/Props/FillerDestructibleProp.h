// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FillerDestructibleProp.generated.h"

struct FTimerHandle;

UCLASS()
class BEATEMUP_2122_API AFillerDestructibleProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFillerDestructibleProp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Config")
		class UStaticMeshComponent* myMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Config")
		class UBoxComponent* trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float damageStored;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float forceApplied = 7500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float blinkTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float dissapearRate = 3.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float minDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float maxDamage;


	float opacity = 1.0f;
	bool dissapear = false;
	FTimerHandle blinkTimer;
	FTimerHandle destroyTimer;
	UMaterialInstanceDynamic* fadeMat;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OverlapBehaviour(AActor* self, AActor* other);
	
	UFUNCTION()
	void Blink();

	UFUNCTION()
	void DestroyFillerProp();

};
