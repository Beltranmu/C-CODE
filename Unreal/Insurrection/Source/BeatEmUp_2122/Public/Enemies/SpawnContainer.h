// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnContainer.generated.h"

class ASpawnPoint;

UCLASS()
class BEATEMUP_2122_API ASpawnContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnContainer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Point List")
		TArray<ASpawnPoint*> spawnPointList;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
