// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SpawnPoint.h"
#include "Components/SceneComponent.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//enemiesToSpawn = -1;

	USceneComponent* tmpSceneComp = nullptr;

	tmpSceneComp = CreateDefaultSubobject<USceneComponent>("Spawn Point 0");
	spawnPoint.Add(tmpSceneComp);
  RootComponent = spawnPoint[0];

  tmpSceneComp = CreateDefaultSubobject<USceneComponent>("Spawn Point 1");
  spawnPoint.Add(tmpSceneComp);
  spawnPoint[1]->SetupAttachment(spawnPoint[0]);

  tmpSceneComp = CreateDefaultSubobject<USceneComponent>("Spawn Point 2");
  spawnPoint.Add(tmpSceneComp);
  spawnPoint[2]->SetupAttachment(spawnPoint[0]);

  tmpSceneComp = CreateDefaultSubobject<USceneComponent>("Spawn Point 3");
  spawnPoint.Add(tmpSceneComp);
  spawnPoint[3]->SetupAttachment(spawnPoint[0]);

  tmpSceneComp = CreateDefaultSubobject<USceneComponent>("Spawn Point 4");
  spawnPoint.Add(tmpSceneComp);
  spawnPoint[4]->SetupAttachment(spawnPoint[0]);
  
  // Timers
  _masillaTimerValue = 0.f;
  _agileTimerValue = 0.f;
  _tankTimerValue = 0.f;
  _masillaExplosiveTimerValue = 0.f;
  delayTime = 0.f;

}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

