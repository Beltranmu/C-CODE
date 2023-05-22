// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyCommander.h"
#include "Player/TurboPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Core/CameraBEU.h"
#include "EngineUtils.h"
#include "Enemies/SpawnPoint.h"
#include "Enemies/SpawnTrigger.h"
#include "Player/DestinationPoint.h"
#include "HUD/HUDGameLevel.h"
#include "Enemies/EdgePoint.h"
#include "Core/GameInstanceBEU.h"
#include "NavigationSystem.h"
#include "Engine/BlockingVolume.h"
#include "Core/GamemodeBEU.h"

// Sets default values
AEnemyCommander::AEnemyCommander()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get this from GameState
	numPlayers = 1;
		
	// Enemies Waves
	_currentWave = 0;

	firstTimeCheckEnemiesSurroundingPlayer = true;
	changeWave = false;
	maxWavesCurrentTrigger = 0;
  // !!!!!NOOOOOOO CAMBIAR¡¡¡¡¡
  jailPosition = FVector(600.0f, -160000.0f, -1850.0f);
  // !!!!!NOOOOOOO CAMBIAR¡¡¡¡¡
}

// Called when the game starts or when spawned
void AEnemyCommander::BeginPlay()
{
	Super::BeginPlay();
	

  UGameInstanceBEU* gi = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Get all players references
  if (mainCharacter.Num() == 0) {
    for (int i = 0; i < numPlayers; ++i) {
      ATurboPlayer* tmpChar = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), i)->GetCharacter());
      mainCharacter.Add(tmpChar);
			enemiesOnPlayer[i] = 0;
    }
  }

	_gameMode = Cast<AGamemodeBEU>(UGameplayStatics::GetGameMode(GetWorld()));

	// Creation of the pool
	numberEnemiesArray[0] = numberMasilla;
	numberEnemiesArray[1] = numberMasillaMod;
	numberEnemiesArray[2] = numberAgile;
	numberEnemiesArray[3] = numberAgileMod;
	numberEnemiesArray[4] = numberTank;
	numberEnemiesArray[5] = numberTankMod;
	const int numberEnemyTypes = 6;
	initEnemiesList[0] = 0;
	for(int i = 0; i < numberEnemyTypes; ++i){
			if (i > 0) {
				initEnemiesList[i] = numberEnemiesArray[i - 1] + initEnemiesList[i - 1];
			}
		for(int j = 0; j < numberEnemiesArray[i]; ++j){
			
      FTransform myTr;
      myTr.SetRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());
      // !!!!!NOOOOOOO CAMBIAR¡¡¡¡¡
        myTr.SetLocation(jailPosition);
      // !!!!!NOOOOOOO CAMBIAR¡¡¡¡¡
			ABaseEnemy* newEnemy = 
      GetWorld()->SpawnActor<ABaseEnemy>(bp_enemiesList[i], myTr);
			if(nullptr !=newEnemy){
        newEnemy->_onDeath.AddDynamic(this, &AEnemyCommander::DestroyEnemyFromArray);
        newEnemy->_getNewPoint.AddDynamic(this, &AEnemyCommander::SetNewLocation);
				newEnemy->updateUIEvent.AddDynamic(hud, &AHUDGameLevel::UpdateEnemiesBar);
				newEnemy->_onAttackEvent.AddDynamic(this, &AEnemyCommander::SetAttackPoint);


        int32 currentLevelID = gi->currentLevelID;
        if (currentLevelID < extraHealthperLevel.Num() && currentLevelID < extraDMGperLevel.Num()) {
          newEnemy->maxHealth += extraHealthperLevel[currentLevelID];
          newEnemy->extraDMGLevel = extraDMGperLevel[currentLevelID];
          newEnemy->health = newEnemy->maxHealth;
        }
				
				if (_gameMode->gameDifficulty == Difficulty::kDificulty_Hard) {

					newEnemy->health *= 2.f;

				}
        
				newEnemy->_mainCharacter.Add(mainCharacter[0]);
				if(mainCharacter.Num()>1){
					newEnemy->_mainCharacter.Add(mainCharacter[1]);
				}
        //newEnemy->_onDeath.AddDynamic(this, &AEnemyCommander::DestroyEnemyFromArray);
        //newEnemy->_onDeath.AddDynamic(this, &AEnemyCommander::DestroyEnemyFromArray);
				newEnemy->characterID = initEnemiesList[i] + j;
				newEnemy->SetActorTickEnabled(false);			
				newEnemy->PrimaryActorTick.SetTickFunctionEnable(false);
        enemiesListPool.Add(newEnemy);
        KillEnemy(initEnemiesList[i] + j);
			}
		}
	}
			
	//Camera
	AActor* returnedActor;
	returnedActor = UGameplayStatics::GetActorOfClass(GetWorld(), cameraClass);
	myCamera = Cast<ACameraBEU>(returnedActor);
	myCamera->_triggerKillEvent.AddDynamic(this, &AEnemyCommander::KillEnemy);

	// NavMesh
	navMesh = UNavigationSystemV1::GetCurrent(GetWorld());

	spawningEnemies = false;
	
}

void AEnemyCommander::EndPlay(const EEndPlayReason::Type type){
	Super::EndPlay(type);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
void AEnemyCommander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCommander::DestroyEnemyFromArray(ABaseEnemy* self, int32 playerID) {

	//He muerto por un pj
	uint32 hitplayerID = self->hitByPlayerID;
	if(hitplayerID >= 0){
		mainCharacter[hitplayerID]->callToUIEvent(0, 0, self->score);
	}

	--enemiesOnPlayer[playerID];
	enemiesList.RemoveSingle(self);

	if(1 == enemiesList.Num())
		enemiesList[0]->isLastEnemyOfTheWave = true;

	if (0 == enemiesList.Num() && _currentWave == (maxWavesCurrentTrigger-1)){
		if (myCamera)
			if (myCamera->locked) {
				hud->ShowGoHint();
			}
			myCamera->UnlockCamera();

		/*if (0 != spawnTrigger->invisibleWall.Num()) {

			for (int i = 0; i < 2; ++i) {

				spawnTrigger->invisibleWall[i]->SetActorEnableCollision(false);

			}

		}*/
		spawningEnemies = false;
	}
	mainCharacter[self->playerIndex]->targetPoint[self->pointIndex]->isFree = true;
	KillEnemy(self->characterID);
}

void AEnemyCommander::GetSpawnTrigger(ASpawnTrigger* trigger){

	_currentWave = 0;
	spawningEnemies = true;

	if(enemiesList.Num() > 0) {
	
		for(int i = 0; i < mainCharacter.Num(); ++i) {

			for(int j = 0; j < mainCharacter[i]->targetPoint.Num(); ++j) {

				mainCharacter[i]->targetPoint[j]->isFree = true;

			}

		}

		for(int i = enemiesList.Num(); i > 0; --i) {
			
			KillEnemy(enemiesList[i-1]->characterID);
			enemiesList.RemoveSingle(enemiesList[i-1]);

		}

	}
	
	spawnTrigger = trigger;

  for (int i = 0; i < mainCharacter.Num(); ++i) {

    firstTimeCheckEnemiesSurroundingPlayer = true;

  }

	int numWaves = spawnTrigger->spawnPointList[0]->complexWave.Num();

  for (int i = 0; i < spawnTrigger->spawnPointList.Num(); ++i) {

		if(numWaves < spawnTrigger->spawnPointList[i]->complexWave.Num())
			numWaves = spawnTrigger->spawnPointList[i]->complexWave.Num();

	}

	maxWavesCurrentTrigger = numWaves;

	if(totalEnemiesPerWave.Num() != 0)
		totalEnemiesPerWave.Empty();

	totalEnemiesPerWave.Init(0, numWaves);

	for(int i = 0; i < spawnTrigger->spawnPointList.Num(); ++i){
		for(int j = 0; j < spawnTrigger->spawnPointList[i]->complexWave.Num(); ++j){
			for(auto& Elem : spawnTrigger->spawnPointList[i]->complexWave[j].enemyMapWave){
				switch (Elem.Key){

					case Difficulty::kDificulty_Easy:
						for(int k = 0; k < Elem.Value.enemySimpleWaves.Num(); ++k){							
							Elem.Value.enemySimpleWaves[k].enemiesToSpawn = FMath::RandRange(Elem.Value.enemySimpleWaves[k].minEnemies, Elem.Value.enemySimpleWaves[k].maxEnemies);
							Elem.Value.totalEnemiesToSpawn += Elem.Value.enemySimpleWaves[k].enemiesToSpawn;
						}
						totalEnemiesPerWave[j] += Elem.Value.totalEnemiesToSpawn;
						totalEnemiesPerTrigger += Elem.Value.totalEnemiesToSpawn;
						break;

          case Difficulty::kDificulty_Normal:
            for (int k = 0; k < Elem.Value.enemySimpleWaves.Num(); ++k) {
              Elem.Value.totalEnemiesToSpawn += Elem.Value.enemySimpleWaves[k].enemiesToSpawn;
            }
            break;

          case Difficulty::kDificulty_Hard:
            for (int k = 0; k < Elem.Value.enemySimpleWaves.Num(); ++k) {
              Elem.Value.enemySimpleWaves[k].enemiesToSpawn = FMath::RandRange(Elem.Value.enemySimpleWaves[k].minEnemies, Elem.Value.enemySimpleWaves[k].maxEnemies);
              Elem.Value.totalEnemiesToSpawn += Elem.Value.enemySimpleWaves[k].enemiesToSpawn;
            }
            totalEnemiesPerWave[j] += Elem.Value.totalEnemiesToSpawn;
            totalEnemiesPerTrigger += Elem.Value.totalEnemiesToSpawn;
            break;

				}
			}
		}

    if (spawnTrigger->spawnPointList[i]->delayTime != 0.f) {

      spawnTrigger->spawnPointList[i]->_delayTimerDelegate.BindUFunction(this, FName("startSpawnTimers"), spawnTrigger->spawnPointList[i]);
      GetWorldTimerManager().SetTimer(spawnTrigger->spawnPointList[i]->_delayTimer, spawnTrigger->spawnPointList[i]->_delayTimerDelegate, spawnTrigger->spawnPointList[i]->delayTime, false);

    }

	}

	if (myCamera)
		myCamera->_cameraEvent.Broadcast(spawnTrigger->lockConfig);

	if (spawnTrigger->lockConfig.lock) {
		/*if (0 != spawnTrigger->invisibleWall.Num()) {
	
			for (int i = 0; i < 2; ++i) {
	
				spawnTrigger->invisibleWall[i]->SetActorEnableCollision(true);
	
			}
	
		}*/
	}
	
}

void AEnemyCommander::startSpawnTimers(ASpawnPoint* spawnPoint) {

  spawnPoint->_masillaTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Masilla, spawnPoint);
  GetWorldTimerManager().SetTimer(spawnPoint->_masillaTimer, spawnPoint->_masillaTimerDelegate, spawnPoint->_masillaTimerValue, true);

	spawnPoint->_masillaExplosiveTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Masilla_Mod, spawnPoint);
  GetWorldTimerManager().SetTimer(spawnPoint->_masillaExplosiveTimer, spawnPoint->_masillaExplosiveTimerDelegate, spawnPoint->_masillaExplosiveTimerValue, true);
  
	spawnPoint->_agileTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Agile, spawnPoint);
  GetWorldTimerManager().SetTimer(spawnPoint->_agileTimer, spawnPoint->_agileTimerDelegate, spawnPoint->_agileTimerValue, true);

  spawnPoint->_agileRangedTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Agile_Mod, spawnPoint);
  GetWorldTimerManager().SetTimer(spawnPoint->_agileRangedTimer, spawnPoint->_agileRangedTimerDelegate, spawnPoint->_agileRangedTimerValue, true);

	spawnPoint->_tankTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Tank, spawnPoint);
  GetWorldTimerManager().SetTimer(spawnPoint->_tankTimer, spawnPoint->_tankTimerDelegate, spawnPoint->_tankTimerValue, true);

  spawnPoint->_tankUnstoppableTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Tank_Mod, spawnPoint);
  GetWorldTimerManager().SetTimer(spawnPoint->_tankUnstoppableTimer, spawnPoint->_tankUnstoppableTimerDelegate, spawnPoint->_tankUnstoppableTimerValue, true);

}

void AEnemyCommander::SpawnEnemies(EnemyType type, ASpawnPoint* spawnPoint) {

	ASpawnPoint* SP = spawnPoint;
	FWave Wave;
	FWaveMapValue WMV;
	FSimpleWave ESW;
	int32 enemiesToSpawn = 0;

	if(SP) {

		if(SP->complexWave.Num() > _currentWave) {

      Wave = SP->complexWave[_currentWave];

      WMV = Wave.enemyMapWave[Difficulty::kDificulty_Easy];

      if (WMV.enemySimpleWaves.Num() > type) {

        ESW = WMV.enemySimpleWaves[type];

        enemiesToSpawn = ESW.enemiesToSpawn;

      }else{ }

		}

	}

	//if(spawnTrigger->spawnPointList[i]->complexWave[_currentWave].enemyMapWave[Difficulty::kDificulty_Easy].enemySimpleWaves[type].enemiesToSpawn > 0){
	if(enemiesToSpawn>0) {

		FSimpleWave& tmpWave = spawnPoint->complexWave[_currentWave].enemyMapWave[Difficulty::kDificulty_Easy].enemySimpleWaves[type];
		ABaseEnemy* tmpEnemy = nullptr;
		FTransform myTr;
		FVector tmpDst;
    myTr.SetRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());				
    myTr.SetLocation(spawnPoint->spawnPoint[rand()%5]->GetComponentLocation());
		
		int32 tmpIndex = 0;

		do {
			tmpIndex = rand() % 5;
		} while(tmpIndex == spawnPoint->lastPointWhereEnemiesSpawnedFrom);

		spawnPoint->lastPointWhereEnemiesSpawnedFrom = tmpIndex;

		tmpDst = spawnPoint->spawnPoint[spawnPoint->lastPointWhereEnemiesSpawnedFrom]->GetComponentLocation();
		tmpEnemy = SpawnEnemy(GetFristAvailableEnemy(type), tmpDst);		
		GetWorld()->GetTimerManager().SetTimer(tmpEnemy->_timerStartMovingAfterSpawning, tmpEnemy, &ABaseEnemy::BecomeAlive, tmpEnemy->timerStartMovingAfterSpawningValue, false);		
		tmpEnemy->_state2 = ABaseEnemy::kState_StandBy;
    --tmpWave.enemiesToSpawn;
		tmpEnemy->isHitByFinisher = false;
		tmpEnemy->isLastEnemyOfTheWave = false;
    enemiesList.Add(tmpEnemy);
		--totalEnemiesPerTrigger;
		--totalEnemiesPerWave[_currentWave];

		changeWave = true;

    /*if (tmpEnemy) {


      enemiesList.Last()->_target = mainCharacter[0];
      enemiesList.Last()->targetID = 0;


      ++enemiesOnPlayer[0];

    }*/

    if (totalEnemiesPerWave[_currentWave] == 0) {

      for (int i = 0; i < mainCharacter.Num(); ++i) {

        GetWorldTimerManager().SetTimer(mainCharacter[i]->timerSendEnemiesTowardsPlayer, this, &AEnemyCommander::CheckEnemiesSurroundingPlayer, mainCharacter[i]->timerSendEnemiesTowardsPlayerValueFirstTime, false);

      }

    }

		
	}

}

void AEnemyCommander::SetNewLocation(ABaseEnemy* enemy, int32 pointType, int32 playerIndexTarget) {
	
  float minDistance = 10000000.f;
  float currentDistance = 0.f;
	float closestPointDistance = 1000000000.f;

	int iterator = 6;
	bool middlePointSaved = false;

	FVector tmpLocation = FVector::ZeroVector;

	bool pointIsReachable = false;
	FNavLocation navLocation;

	switch(pointType) {   

		case 0:
			// Gotta get a point from the edge
			
			if (spawnTrigger != nullptr) {

				for (int i = 0; i < spawnTrigger->leftSidePoints.Num(); ++i) {

					if(spawnTrigger->leftSidePoints[i]->isFree) {

						currentDistance = (enemy->GetActorLocation() - spawnTrigger->leftSidePoints[i]->GetActorLocation()).Size();

						if (currentDistance < minDistance) {
							minDistance = currentDistance;
							enemy->edgePointIndex = i;
							enemy->_targetLocation = spawnTrigger->leftSidePoints[i]->GetActorLocation();
						}

					}

				}
				
        if (enemy->pointOnAttackAssigned) {
          enemy->pointOnAttackAssigned = false;
          mainCharacter[enemy->playerIndex]->targetPoint[enemy->pointIndex]->isFree = true;
        }

				enemy->edgePointHasBeenAssigned = true;

				if (enemy->edgePointIndex >= spawnTrigger->leftSidePoints.Num() || -1 == enemy->edgePointIndex) {
					int32 randomPoint = rand()%spawnTrigger->leftSidePoints.Num();
					enemy->edgePointIndex = randomPoint;
					enemy->_targetLocation = spawnTrigger->leftSidePoints[enemy->edgePointIndex]->GetActorLocation();
				}

				spawnTrigger->leftSidePoints[enemy->edgePointIndex]->isFree = false;

			}

		break;
	
		case 1:
	
			// Must get closer to the player
			if(enemy->alive) {

				// Check closest point to go
				for(int i = 0; i < mainCharacter.Num(); ++i) {
						
					// Middle Points
					do {

						pointIsReachable = navMesh->ProjectPointToNavigation(mainCharacter[i]->targetPoint[iterator]->GetComponentLocation(), navLocation);

						if (mainCharacter[i]->targetPoint[iterator]->isFree && pointIsReachable) {

							enemy->pointIndex = iterator;
							enemy->playerIndex = i;

							if (rand()% 101 <= enemy->middlePointWeight) {

								enemy->pointHasBeenAssigned = true;
								middlePointSaved = false;
								mainCharacter[enemy->playerIndex]->targetPoint[enemy->pointIndex]->isFree = false;
			
							}
							else {
								
								enemy->pointHasBeenAssigned = false;
								middlePointSaved = true;

							}

						}

						++iterator;

					}while(iterator <= 7 && !enemy->pointHasBeenAssigned && !middlePointSaved);

					iterator = 6;

					// External Points	
					if(middlePointSaved || !enemy->pointHasBeenAssigned) {

						for(int j = 2; j < 6; ++j) {

							pointIsReachable = navMesh->ProjectPointToNavigation(mainCharacter[i]->targetPoint[j]->GetComponentLocation(), navLocation);

							if(mainCharacter[i]->targetPoint[j]->isFree && pointIsReachable) {

								if ((mainCharacter[i]->targetPoint[j]->GetComponentLocation() - enemy->GetActorLocation()).Size() <= closestPointDistance) {
									closestPointDistance = (mainCharacter[i]->targetPoint[j]->GetComponentLocation() - enemy->GetActorLocation()).Size();
									enemy->pointIndex = j;
									enemy->playerIndex = i;
									enemy->pointHasBeenAssigned = true;
								}

							}

						}

					}

				}

				mainCharacter[enemy->playerIndex]->targetPoint[enemy->pointIndex]->isFree = false;
				
				// Raycast to the ground
        //FHitResult ground;
        //FCollisionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
        //TraceParams.AddIgnoredActor(enemy);
				//
				//float zLocation = 0.f;
				tmpLocation = mainCharacter[enemy->playerIndex]->targetPoint[enemy->pointIndex]->GetComponentLocation();

        //if (GetWorld()->LineTraceSingleByChannel(ground, tmpLocation, tmpLocation - FVector(0.0f, 0.0f, 500.0f),
				//	ECC_GameTraceChannel5, TraceParams)) {
				//
        //  zLocation = ground.GetActor()->GetActorLocation().Z;
        //}
				
				enemy->pointHasBeenAssigned = true;
				enemy->_targetLocation = tmpLocation;//FVector(tmpLocation.X, tmpLocation.Y, zLocation);
				enemy->edgePointHasBeenAssigned = false;

			}

		break;

		case 2:

			enemy->_targetLocation = navMesh->GetRandomReachablePointInRadius(GetWorld(), spawnTrigger->leftSidePoints[enemy->edgePointIndex]->GetActorLocation(), enemy->rangeRandomPoint);

		break;

    case 3:

      enemy->_targetLocation = navMesh->GetRandomReachablePointInRadius(GetWorld(), mainCharacter[enemy->playerIndex]->targetPoint[enemy->pointIndex]->GetComponentLocation(), enemy->rangeRandomPoint*0.5f);
	
			// Raycast to the ground
			//esult ground;
			//isionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
			//Params.AddIgnoredActor(enemy);
			//
			// zLocation = 0.f;
			tmpLocation = enemy->_targetLocation;

			//if (GetWorld()->LineTraceSingleByChannel(ground, tmpLocation + FVector(0.0f, 0.0f, 500.0f), tmpLocation - FVector(0.0f, 0.0f, 1500.0f),
			//	ECC_GameTraceChannel5, TraceParams)) {
			//
			//	zLocation = ground.GetActor()->GetActorLocation().Z;
			//}

			enemy->_targetLocation = tmpLocation;//FVector(tmpLocation.X, tmpLocation.Y, zLocation);

    break;
		
		case 4:
			
			// Must get closer to the player
			if (enemy->alive) {

				int i = playerIndexTarget;

				// Middle Points
				do {

					if (mainCharacter[i]->targetPoint[iterator]->isFree) {

						enemy->pointIndex = iterator;
						enemy->playerIndex = i;

						if (rand() % 101 <= enemy->middlePointWeight) {

							enemy->pointHasBeenAssigned = true;
							middlePointSaved = false;
							mainCharacter[enemy->playerIndex]->targetPoint[enemy->pointIndex]->isFree = false;

						}
						else {

							enemy->pointHasBeenAssigned = false;
							middlePointSaved = true;

						}

					}

					++iterator;

				} while (iterator <= 7 && !enemy->pointHasBeenAssigned && !middlePointSaved);

				// External Points	
				if (middlePointSaved || !enemy->pointHasBeenAssigned) {

					for (int j = 2; j < 6; ++j) {

						if (mainCharacter[i]->targetPoint[j]->isFree) {

							if ((mainCharacter[i]->targetPoint[j]->GetComponentLocation() - enemy->GetActorLocation()).Size() <= closestPointDistance) {
								closestPointDistance = (mainCharacter[i]->targetPoint[j]->GetComponentLocation() - enemy->GetActorLocation()).Size();
								enemy->pointIndex = j;
								enemy->playerIndex = i;
								enemy->pointHasBeenAssigned = true;
							}

						}

					}

				}

				mainCharacter[enemy->playerIndex]->targetPoint[enemy->pointIndex]->isFree = false;

				// Raycast to the ground
				//FHitResult ground;
				//FCollisionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
				//TraceParams.AddIgnoredActor(enemy);
				//
				//float zLocation = 0.f;
				tmpLocation = mainCharacter[enemy->playerIndex]->targetPoint[enemy->pointIndex]->GetComponentLocation();

				//if (GetWorld()->LineTraceSingleByChannel(ground, tmpLocation, tmpLocation - FVector(0.0f, 0.0f, 500.0f),
				//	ECC_GameTraceChannel5, TraceParams)) {
				//
				//  zLocation = ground.GetActor()->GetActorLocation().Z;
				//}

				enemy->pointHasBeenAssigned = true;
				enemy->_targetLocation = tmpLocation;//FVector(tmpLocation.X, tmpLocation.Y, zLocation);
				enemy->edgePointHasBeenAssigned = false;

			}

			break;

	}

}

void AEnemyCommander::SetAttackPoint(ABaseEnemy* myEnemy) {

	FVector tmpLocation = FVector::ZeroVector;
	float zLocation = 0.f;

  // Raycast to the ground
  FHitResult ground;
  FCollisionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
  TraceParams.AddIgnoredActor(myEnemy);
	
  if ((myEnemy->pointIndex == 2 || myEnemy->pointIndex == 3 || myEnemy->pointIndex == 6) &&
    mainCharacter[myEnemy->playerIndex]->targetPoint[0]->isFree) {

		mainCharacter[myEnemy->playerIndex]->targetPoint[myEnemy->pointIndex]->isFree = true;

		tmpLocation = mainCharacter[myEnemy->playerIndex]->targetPoint[0]->GetComponentLocation();
		
    //if (GetWorld()->LineTraceSingleByChannel(ground, tmpLocation + FVector(0.0f, 0.0f, 500.0f), tmpLocation - FVector(0.0f, 0.0f, 1500.0f),
		//	ECC_GameTraceChannel5, TraceParams)) {
		//
    //  zLocation = ground.GetActor()->GetActorLocation().Z;
    //}

		myEnemy->_targetLocation = tmpLocation;//FVector(tmpLocation.X, tmpLocation.Y, zLocation);

		if (mainCharacter[myEnemy->playerIndex]->GetActorLocation().X < myEnemy->GetActorLocation().X) {

			myEnemy->_targetLocation.X -= approachOffset;

		} else if(mainCharacter[myEnemy->playerIndex]->GetActorLocation().X > myEnemy->GetActorLocation().X) {

			myEnemy->_targetLocation.X += approachOffset * 1.5f;

		}

		myEnemy->pointIndex = 0;
		mainCharacter[myEnemy->playerIndex]->targetPoint[0]->isFree = false;
    myEnemy->pointOnAttackAssigned = true;    
    return;

  }

  if ((myEnemy->pointIndex == 2 || myEnemy->pointIndex == 3 || myEnemy->pointIndex == 6) &&
    !mainCharacter[myEnemy->playerIndex]->targetPoint[0]->isFree &&
    mainCharacter[myEnemy->playerIndex]->targetPoint[1]->isFree) {

		mainCharacter[myEnemy->playerIndex]->targetPoint[myEnemy->pointIndex]->isFree = true;

    tmpLocation = mainCharacter[myEnemy->playerIndex]->targetPoint[1]->GetComponentLocation();

	//if (GetWorld()->LineTraceSingleByChannel(ground, tmpLocation + FVector(0.0f, 0.0f, 500.0f), tmpLocation - FVector(0.0f, 0.0f, 1500.0f),
	//	ECC_GameTraceChannel5, TraceParams)) {
	//
  //  zLocation = ground.GetActor()->GetActorLocation().Z;
  //}

    myEnemy->_targetLocation = tmpLocation;//FVector(tmpLocation.X, tmpLocation.Y, zLocation);

    if (mainCharacter[myEnemy->playerIndex]->GetActorLocation().X < myEnemy->GetActorLocation().X) {

      myEnemy->_targetLocation.X -= approachOffset;

    } else if (mainCharacter[myEnemy->playerIndex]->GetActorLocation().X > myEnemy->GetActorLocation().X) {

      myEnemy->_targetLocation.X += approachOffset * 1.5f;

    }

		mainCharacter[myEnemy->playerIndex]->targetPoint[1]->isFree = false;
		myEnemy->pointIndex = 1;
		myEnemy->pointOnAttackAssigned = true;
    return;

  }

  if ((myEnemy->pointIndex == 4 || myEnemy->pointIndex == 5 || myEnemy->pointIndex == 7) &&
    mainCharacter[myEnemy->playerIndex]->targetPoint[1]->isFree) {

		mainCharacter[myEnemy->playerIndex]->targetPoint[myEnemy->pointIndex]->isFree = true;

    tmpLocation = mainCharacter[myEnemy->playerIndex]->targetPoint[1]->GetComponentLocation();

		//if (GetWorld()->LineTraceSingleByChannel(ground, tmpLocation + FVector(0.0f, 0.0f, 500.0f), tmpLocation - FVector(0.0f, 0.0f, 1500.0f),
		//	ECC_GameTraceChannel5, TraceParams)) {
		//
    //  zLocation = ground.GetActor()->GetActorLocation().Z;
    //}

    myEnemy->_targetLocation = tmpLocation;//FVector(tmpLocation.X, tmpLocation.Y, zLocation);

    if (mainCharacter[myEnemy->playerIndex]->GetActorLocation().X < myEnemy->GetActorLocation().X) {

      myEnemy->_targetLocation.X -= approachOffset;

    } else if (mainCharacter[myEnemy->playerIndex]->GetActorLocation().X > myEnemy->GetActorLocation().X) {

      myEnemy->_targetLocation.X += approachOffset * 1.5f;

    }

		mainCharacter[myEnemy->playerIndex]->targetPoint[1]->isFree = false;
		myEnemy->pointIndex = 1;
		myEnemy->pointOnAttackAssigned = true;
    return;

  }

  if ((myEnemy->pointIndex == 4 || myEnemy->pointIndex == 5 || myEnemy->pointIndex == 7) &&
    !mainCharacter[myEnemy->playerIndex]->targetPoint[1]->isFree &&
    mainCharacter[myEnemy->playerIndex]->targetPoint[0]->isFree) {

		mainCharacter[myEnemy->playerIndex]->targetPoint[myEnemy->pointIndex]->isFree = true;

    tmpLocation = mainCharacter[myEnemy->playerIndex]->targetPoint[0]->GetComponentLocation();

		//if (GetWorld()->LineTraceSingleByChannel(ground, tmpLocation + FVector(0.0f, 0.0f, 500.0f), tmpLocation - FVector(0.0f, 0.0f, 1500.0f),
		//	ECC_GameTraceChannel5, TraceParams)) {
		//
    //  zLocation = ground.GetActor()->GetActorLocation().Z;
    //}

    myEnemy->_targetLocation = tmpLocation;//FVector(tmpLocation.X, tmpLocation.Y, zLocation);

    if (mainCharacter[myEnemy->playerIndex]->GetActorLocation().X < myEnemy->GetActorLocation().X) {

      myEnemy->_targetLocation.X -= approachOffset;

    } else if (mainCharacter[myEnemy->playerIndex]->GetActorLocation().X > myEnemy->GetActorLocation().X) {

      myEnemy->_targetLocation.X += approachOffset * 1.5f;

    }

		mainCharacter[myEnemy->playerIndex]->targetPoint[0]->isFree = false;
		myEnemy->pointIndex = 0;
		myEnemy->pointOnAttackAssigned = true;
    return;

  }

	myEnemy->pointOnAttackAssigned = false;

}

void AEnemyCommander::CheckEnemiesSurroundingPlayer() {

	bool targetPointsAreFree[2];
	targetPointsAreFree[0] = true;
	targetPointsAreFree[1] = true;

	if(firstTimeCheckEnemiesSurroundingPlayer) {
		firstTimeCheckEnemiesSurroundingPlayer = false;
		for(int i = 0; i < mainCharacter.Num(); ++i) {
			GetWorldTimerManager().SetTimer(mainCharacter[i]->timerSendEnemiesTowardsPlayer, this, &AEnemyCommander::CheckEnemiesSurroundingPlayer, mainCharacter[i]->timerSendEnemiesTowardsPlayerValueRepeatedly, true);
		}
	}

	for(int i = 0; i < mainCharacter.Num(); ++i) {

		for(int j = 0; j < mainCharacter[i]->targetPoint.Num(); ++j) {

			targetPointsAreFree[i] = targetPointsAreFree[i] && mainCharacter[i]->targetPoint[j]->isFree;

		}

	}

	if(mainCharacter.Num() > 1) {

		int numEnemies = enemiesList.Num();
		
		// Both players are free
		if (targetPointsAreFree[0] && targetPointsAreFree[1]) {
			
			// Divive enemies into both characters if numEnemies > 1
			if (numEnemies > 1) {

				for (int e = 0; e < numEnemies / 2; ++e) {

					SetNewLocation(enemiesList[e], 1);
					enemiesList[e]->_state2 = ABaseEnemy::kState_Chase2;
					GetWorldTimerManager().ClearTimer(enemiesList[e]->_timerPauseRandomPoint);

				}

				targetPointsAreFree[0] = false;

				for (int e = numEnemies / 2; e < numEnemies; ++e) {

					SetNewLocation(enemiesList[e], 4, 1);
					enemiesList[e]->_state2 = ABaseEnemy::kState_Chase2;
					GetWorldTimerManager().ClearTimer(enemiesList[e]->_timerPauseRandomPoint);

				}

				targetPointsAreFree[1] = false;

			}
			else {

				int randomPlayer = rand()%2;

				for (int e = 0; e < numEnemies; ++e) {

					SetNewLocation(enemiesList[e], 4, randomPlayer);
					enemiesList[e]->_state2 = ABaseEnemy::kState_Chase2;
					GetWorldTimerManager().ClearTimer(enemiesList[e]->_timerPauseRandomPoint);

				}
				
				// Add case SetNewLocation passing an index of the MainCharacter
				targetPointsAreFree[randomPlayer] = false;

			}

		}	// Player 0 free - Player 1 has enemies
		else if (targetPointsAreFree[0] && !targetPointsAreFree[1]) {

			for (int e = 0; e < numEnemies; ++e) {

				SetNewLocation(enemiesList[e], 4, 0);
				enemiesList[e]->_state2 = ABaseEnemy::kState_Chase2;
				GetWorldTimerManager().ClearTimer(enemiesList[e]->_timerPauseRandomPoint);

			}

			// Add case SetNewLocation passing an index of the MainCharacter
			targetPointsAreFree[0] = false;

		}	// Player 0 has enemies - Player 1 free
		else if (!targetPointsAreFree[0] && targetPointsAreFree[1]) {

			for (int e = 0; e < numEnemies; ++e) {

				SetNewLocation(enemiesList[e], 4, 1);
				enemiesList[e]->_state2 = ABaseEnemy::kState_Chase2;
				GetWorldTimerManager().ClearTimer(enemiesList[e]->_timerPauseRandomPoint);

			}

			// Add case SetNewLocation passing an index of the MainCharacter
			targetPointsAreFree[1] = false;

		}

	}	// Single Player
	else {

		if (targetPointsAreFree[0]) {

			for (int e = 0; e < enemiesList.Num(); ++e) {

				SetNewLocation(enemiesList[e], 1);
				enemiesList[e]->_state2 = ABaseEnemy::kState_Chase2;
				GetWorldTimerManager().ClearTimer(enemiesList[e]->_timerPauseRandomPoint);

			}

			targetPointsAreFree[0] = false;

		}

	}

}

int AEnemyCommander::GetFristAvailableEnemy(EnemyType enemyType){

	for(int i = initEnemiesList[enemyType]; i < (initEnemiesList[enemyType] + numberEnemiesArray [enemyType]); ++i){
		if(enemiesListPool[i]->death){
			return i;
		}
	}
	return -1;
}

ABaseEnemy* AEnemyCommander::SpawnEnemy(int32 enemyID, FVector position) {
	// "SPAWN"
	if ((enemyID >= 0) && (enemyID <= enemiesListPool.Num()) ) {


    enemiesListPool[enemyID]->death = false;
    enemiesListPool[enemyID]->isSpawning = true;
    enemiesListPool[enemyID]->setNewStyle(); 
    enemiesListPool[enemyID]->GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyD"));
		enemiesListPool[enemyID]->SetActorEnableCollision(true);
		enemiesListPool[enemyID]->SetActorTickEnabled(true);
		enemiesListPool[enemyID]->SetActorHiddenInGame(false);

    //enemiesListPool[enemyID]->ResetEnemy();
		enemiesListPool[enemyID]->SetActorLocation(position);
    //SetNew Style()
 
	}

	return enemiesListPool[enemyID];

}

void AEnemyCommander::KillEnemy(int32 enemyID){
	// "KILL"
	if ((enemyID >= 0) && (enemyID <= enemiesListPool.Num()) ) {
		
		//UWorld* tmpWorld = enemiesListPool[enemyID]->GetWorld();
		//UGameplayStatics::SetGlobalTimeDilation(tmpWorld, 1);

		enemiesListPool[enemyID]->alive = false;
		enemiesListPool[enemyID]->death = true;

		enemiesListPool[enemyID]->SetActorEnableCollision(false);
		enemiesListPool[enemyID]->SetActorTickEnabled(false);
		enemiesListPool[enemyID]->PrimaryActorTick.SetTickFunctionEnable(false);
		enemiesListPool[enemyID]->SetActorHiddenInGame(true);
    //enemiesListPool[enemyID]->

		enemiesListPool[enemyID]->pointHasBeenAssigned = false;
		enemiesListPool[enemyID]->edgePointHasBeenAssigned = false;
		enemiesListPool[enemyID]->pointOnAttackAssigned = false;
		enemiesListPool[enemyID]->pointIndex = 0;
		enemiesListPool[enemyID]->playerIndex = 0;
    enemiesListPool[enemyID]->SetActorLocation(jailPosition);

		GetWorldTimerManager().ClearAllTimersForObject(enemiesListPool[enemyID]);

		if(totalEnemiesPerWave.Num() != 0) {

      if ((maxWavesCurrentTrigger-1) > _currentWave && totalEnemiesPerWave[_currentWave] == 0 && enemiesList.Num() == 0 && changeWave) {

				changeWave = false;

        /*for (int i = 0; i < spawnTrigger->spawnPointList.Num(); ++i) {

          GetWorldTimerManager().ClearAllTimersForObject(spawnTrigger->spawnPointList[i]);

        }*/

        GetWorldTimerManager().SetTimer(_timerChangeWave, this, &AEnemyCommander::ChangeWave, _timerChangeWaveValue, false);

				for (int i = 0; i < mainCharacter.Num(); ++i) {

					GetWorldTimerManager().ClearTimer(mainCharacter[i]->timerSendEnemiesTowardsPlayer);

				}
        
      }

		}

		// Disable Character Movement
	}
}

void AEnemyCommander::ChangeWave() {

	++_currentWave;

	for(int i = 0; i < spawnTrigger->spawnPointList.Num(); ++i) {

		ASpawnPoint* spawnPoint = spawnTrigger->spawnPointList[i];

    spawnPoint->_masillaTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Masilla, spawnPoint);
    GetWorldTimerManager().SetTimer(spawnPoint->_masillaTimer, spawnPoint->_masillaTimerDelegate, spawnPoint->_masillaTimerValue, true);

    spawnPoint->_masillaExplosiveTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Masilla_Mod, spawnPoint);
    GetWorldTimerManager().SetTimer(spawnPoint->_masillaExplosiveTimer, spawnPoint->_masillaExplosiveTimerDelegate, spawnPoint->_masillaExplosiveTimerValue, true);

    spawnPoint->_agileTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Agile, spawnPoint);
    GetWorldTimerManager().SetTimer(spawnPoint->_agileTimer, spawnPoint->_agileTimerDelegate, spawnPoint->_agileTimerValue, true);

    spawnPoint->_agileRangedTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Agile_Mod, spawnPoint);
    GetWorldTimerManager().SetTimer(spawnPoint->_agileRangedTimer, spawnPoint->_agileRangedTimerDelegate, spawnPoint->_agileRangedTimerValue, true);

    spawnPoint->_tankTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Tank, spawnPoint);
    GetWorldTimerManager().SetTimer(spawnPoint->_tankTimer, spawnPoint->_tankTimerDelegate, spawnPoint->_tankTimerValue, true);

    spawnPoint->_tankUnstoppableTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), EnemyType_Tank_Mod, spawnPoint);
    GetWorldTimerManager().SetTimer(spawnPoint->_tankUnstoppableTimer, spawnPoint->_tankUnstoppableTimerDelegate, spawnPoint->_tankUnstoppableTimerValue, true);

	}
}
