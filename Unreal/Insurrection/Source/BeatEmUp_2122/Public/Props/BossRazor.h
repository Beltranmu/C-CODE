// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CommonBEU.h"
#include "BossRazor.generated.h"

class UStaticMeshComponent;
class ATurboPlayer;
struct FTimerHandle;

USTRUCT()
struct FRazorMovement{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FVector dir;
	UPROPERTY(EditAnywhere)
	float speed;
	UPROPERTY(EditAnywhere)
	FVector decrementRate;
	UPROPERTY(EditAnywhere)
	float rotationSpeed;
};

UCLASS()
class BEATEMUP_2122_API ABossRazor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossRazor();

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* myMesh;

	UPROPERTY(EditAnywhere, Category = "Config")
		FAttackStats combatStats;

	UPROPERTY(EditAnywhere, Category = "Config")
		FRazorMovement movementData;

	TArray<ATurboPlayer*> playerHitArray;

	FTimerHandle destroyTimer;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DestroyRazor();

	UFUNCTION()
		void OverlapBehaviour(AActor* self, AActor* other);

};
