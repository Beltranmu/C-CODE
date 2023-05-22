// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CommonBEU.h"
#include "SpawnTrigger.generated.h"

class UStaticMeshComponent;
class ASpawnPoint;
class ACameraBEU;
class AEdgePoint;
class ABlockingVolume;

UCLASS()
class BEATEMUP_2122_API ASpawnTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnTrigger();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invisible Walls")
		//TArray<ABlockingVolume*> invisibleWall;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Mesh")
    UStaticMeshComponent* triggerMesh;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Point List")
    TArray<ASpawnPoint*> spawnPointList;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle Side")
    TArray<AEdgePoint*> leftSidePoints;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle Side")
    TArray<AEdgePoint*> rightSidePoints;


	UFUNCTION()
    void PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
											 bool bFromSweep, const FHitResult& SweepResult);
	//CAMERA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FCameraLockConfig lockConfig;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type type) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
