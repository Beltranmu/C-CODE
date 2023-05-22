// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/ElevatorControllerBEU.h"
#include "Enemies/EnemyCommander.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AElevatorControllerBEU::AElevatorControllerBEU()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AElevatorControllerBEU::BeginPlay()
{
	Super::BeginPlay();
	

  
}

// Called every frame
void AElevatorControllerBEU::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  if (!IsValid(enemyCommander)) {
    enemyCommander = Cast<AEnemyCommander>(UGameplayStatics::GetActorOfClass(GetWorld(), enemyCommanderClass));
  }

  if (enemyCommander->spawningEnemies && !levelStarted) {
    levelStarted = true;
  }

  if (levelStarted && !enemyCommander->spawningEnemies) {
    GetWorld()->GetTimerManager().SetTimer(timerCL, this, &AElevatorControllerBEU::ChangeLevel, timeCL, false);
    levelStarted = false;
  }

}

void AElevatorControllerBEU::ChangeLevel() {

  UGameplayStatics::OpenLevel(this, FName(mapName), false);

}
