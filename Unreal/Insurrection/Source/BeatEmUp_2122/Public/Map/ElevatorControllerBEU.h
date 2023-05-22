// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorControllerBEU.generated.h"

class AEnemyCommander;

UCLASS()
class BEATEMUP_2122_API AElevatorControllerBEU : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorControllerBEU();


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Level Paramenters")
    float timeCL = 5.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Level Paramenters")
    FString mapName;

  UPROPERTY(EditAnywhere, Category = "Configuration")
    TSubclassOf<AEnemyCommander> enemyCommanderClass;

  FTimerHandle timerCL;

  bool levelStarted;

  AEnemyCommander* enemyCommander;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  void ChangeLevel();

};
