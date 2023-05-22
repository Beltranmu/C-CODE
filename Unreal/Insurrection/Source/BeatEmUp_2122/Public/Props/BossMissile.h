// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CommonBEU.h"
#include "BossMissile.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class BEATEMUP_2122_API ABossMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossMissile();

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "ParticleComp")
		UParticleSystemComponent* trail;

	UPROPERTY(EditAnywhere, Category = "Config")
		FAttackStats combatStats;

	UPROPERTY(EditAnywhere, Category = "Config")
		bool damagesEnemies = true;

	UPROPERTY(EditAnywhere, Category = "Config")
		float fallSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Config")
		float explosionRadius;

	UPROPERTY(EditAnywhere, Category = "Config")
		float explosionZOffset = 20.0f;

	//As these are spawned above a point defined in a vector array, instead
	//of using collisions we will just explode them when they reach the Z value of the target
	UPROPERTY(VisibleAnywhere, Category = "Config")
		float limitZAxisHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|Particles")
		FParticleData explosionParticle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
