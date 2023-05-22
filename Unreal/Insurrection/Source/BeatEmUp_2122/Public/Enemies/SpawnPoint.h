// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "Core/GamemodeBEU.h"
#include "SpawnPoint.generated.h"

class ABaseEnemy;

USTRUCT(BlueprintType)
struct FSimpleWave {

  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Type")
		TSubclassOf<ABaseEnemy> enemyType;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number of Enemies")
    int32 minEnemies;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number of Enemies")
    int32 maxEnemies;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Number of Enemies")
    int32 enemiesToSpawn;  

};

USTRUCT(BlueprintType)
struct FWaveMapValue {

	GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Waves")
		TArray<FSimpleWave> enemySimpleWaves;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Counter")
    int32 totalEnemiesToSpawn;

};

USTRUCT(BlueprintType)
struct FWave {

  GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Waves")
		TMap<Difficulty, FWaveMapValue > enemyMapWave;

};

UCLASS()
class BEATEMUP_2122_API ASpawnPoint : public AActor {

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

  int32 lastPointWhereEnemiesSpawnedFrom = -1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
    TArray<USceneComponent*> spawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Waves")
		TArray<FWave> complexWave;

  /** TIMER */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    FTimerHandle _masillaTimer;
  FTimerDelegate _masillaTimerDelegate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float _masillaTimerValue;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    FTimerHandle _masillaExplosiveTimer;
  FTimerDelegate _masillaExplosiveTimerDelegate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float _masillaExplosiveTimerValue;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
    FTimerHandle _agileTimer;
  FTimerDelegate _agileTimerDelegate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float _agileTimerValue;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
    FTimerHandle _agileRangedTimer;
  FTimerDelegate _agileRangedTimerDelegate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float _agileRangedTimerValue;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
    FTimerHandle _tankTimer;
  FTimerDelegate _tankTimerDelegate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float _tankTimerValue;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
    FTimerHandle _tankUnstoppableTimer;
  FTimerDelegate _tankUnstoppableTimerDelegate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float _tankUnstoppableTimerValue;
    
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
    FTimerHandle _delayTimer;
  FTimerDelegate _delayTimerDelegate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float delayTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
