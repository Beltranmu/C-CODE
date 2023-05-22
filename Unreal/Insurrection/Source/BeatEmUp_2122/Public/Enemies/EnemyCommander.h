// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/BaseEnemy.h"
#include "EnemyCommander.generated.h"

class ATurboPlayer;
class ASpawnContainer;
class ASpawnTrigger;
class ACameraBEU;
class ASpawnPoint;
class AHUDGameLevel;
class UNavigationSystemV1;
class AGamemodeBEU;

UENUM()
enum EnemyType {
	EnemyType_Masilla = 0,
	EnemyType_Masilla_Mod = 1,
	EnemyType_Agile = 2,
	EnemyType_Agile_Mod = 3,
	EnemyType_Tank = 4,
	EnemyType_Tank_Mod = 5
};

UCLASS()
class BEATEMUP_2122_API AEnemyCommander : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AEnemyCommander();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Difficulty")
    AGamemodeBEU* _gameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		bool spawningEnemies = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data", DisplayName = "Extra damage for each level")
    TArray<float> extraDMGperLevel;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data", DisplayName = "Extra health for each level")
    TArray<float> extraHealthperLevel;

	/** ENEMIES */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		int32 totalEnemyNumber;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		int32 totalMasillaEnemyNumber;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		int32 totalAgileEnemyNumber;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		int32 totalTankEnemyNumber;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<ABaseEnemy*> enemiesList;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
    ASpawnTrigger* spawnTrigger; //La vida de pizzero te espera...

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		float approachOffset = 50.f;

	/** MAIN CHARACTER */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<ATurboPlayer*> mainCharacter;

  UPROPERTY(VisibleAnywhere, Category = "Players")
    int32 enemiesOnPlayer[2];

	/** CORE */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core")
    int32 numPlayers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hud")
		AHUDGameLevel* hud;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 _currentWave;//Javier y la IA?

  UPROPERTY(VisibleAnywhere, Category = "Wave")
    TArray<float> totalEnemiesPerWave;

  UPROPERTY(VisibleAnywhere, Category = "Wave")
    FTimerHandle _timerChangeWave;

  UPROPERTY(EditAnywhere, Category = "Wave")
    float _timerChangeWaveValue;

	bool changeWave;
	int32 maxWavesCurrentTrigger;

	UPROPERTY(VisibleAnywhere, Category = "NavMesh")
		UNavigationSystemV1* navMesh;

	bool firstTimeCheckEnemiesSurroundingPlayer;

  //CAMERA
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Config")
	  TSubclassOf<ACameraBEU> cameraClass;

  ACameraBEU* myCamera;

	// POOL **

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pool")
    FVector jailPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
		int32 numberMasilla;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
		int32 numberMasillaMod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
		int32 numberAgile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
		int32 numberAgileMod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
		int32 numberTank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
		int32 numberTankMod;

	UPROPERTY(VisibleAnywhere,  Category = "Pool")
		int32 numberEnemiesArray[6];

  UPROPERTY(VisibleAnywhere, Category = "Pool")
    int32 initEnemiesList[6];

  UPROPERTY(EditAnywhere, Category = "Pool")
    FVector testingPosition;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool|Enemies")
    TArray<TSubclassOf<ABaseEnemy>> bp_enemiesList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool|Array")
		TArray<ABaseEnemy*> enemiesListPool;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pool|Array")
    float totalEnemiesPerTrigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type type) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void DestroyEnemyFromArray(ABaseEnemy* self, int32 playerID);

  UFUNCTION()
    void GetSpawnTrigger(ASpawnTrigger* trigger);

  UFUNCTION()
    void startSpawnTimers(ASpawnPoint* spawnPoint);

	UFUNCTION()
		void SpawnEnemies(EnemyType type, ASpawnPoint* spawnPoint);

  UFUNCTION()
    void SetNewLocation(ABaseEnemy* enemy, int32 pointType, int32 playerIndexTarget = 0);

	UFUNCTION()
		void SetAttackPoint(ABaseEnemy* myEnemy);

  UFUNCTION()
    void CheckEnemiesSurroundingPlayer();

	// POOL **
  UFUNCTION()
    int GetFristAvailableEnemy(EnemyType enemyType);

	UFUNCTION()
		ABaseEnemy* SpawnEnemy(int32 enemyID, FVector position);

  UFUNCTION()
    void KillEnemy(int32 enemyID);

  UFUNCTION()
    void ChangeWave();
};
