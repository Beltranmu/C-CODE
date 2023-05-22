// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonBEU.h"
#include "CameraBEU.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class ATurboPlayer;
class AEnemyCommander;
class ABaseEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockCameraEvent, FCameraLockConfig, config);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKillEnemy, int32, enemyID);

UCLASS()
class BEATEMUP_2122_API ACameraBEU : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraBEU();

	FLockCameraEvent _cameraEvent;
	FKillEnemy _triggerKillEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		USpringArmComponent* myArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		UCameraComponent* myCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		UStaticMeshComponent* rightWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		UStaticMeshComponent* leftWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		ATurboPlayer* player1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		ATurboPlayer* player2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		TSubclassOf<UCameraShakeBase> shakeClass;

	UPROPERTY(VisibleAnywhere, Category = "Config")
		float cameraOffset = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Config")
		float defaultCameraOffset = 1250.0f;

	UPROPERTY(EditAnywhere, Category = "Config")
		float cameraLag = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Config")
		bool locked;

	UPROPERTY(EditAnywhere, Category = "Config")
		bool playersTooFar;
	
	UPROPERTY(EditAnywhere, Category = "Config")
		float minDistanceToTeleportPlayer = 450.0f;

	UPROPERTY(VisibleAnywhere, Category = "Config")
		FRotator cameraRotation;

	UPROPERTY(VisibleAnywhere, Category = "Config")
		bool transition;

	UPROPERTY(EditAnywhere, Category = "Config")
		FRotator defaultCameraRotation;

	UPROPERTY(EditAnywhere, Category = "Config")
		FVector playerOffset;

	UPROPERTY(EditAnywhere, Category = "Config")
		FVector cameraWallsOffset = { 0.0f,55.0f,0.0f };

	UPROPERTY(EditAnywhere, Category = "Config")
		float minCameraOffsetXAxis = -500.0f;

	UPROPERTY(EditAnywhere, Category = "Config")
		float maxCameraOffsetXAxis = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		float limitYOffsetP1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		float limitYOffsetP2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float limitFovMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Smooth Movement")
		float smoothOffset = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Smooth Movement")
		float smoothRate = 1.2f;

	float scalingSmoothUnlockSpeed = 0.0f;

	UPROPERTY(EditAnyWhere, Category = "Target")
		FVector target_;

	UPROPERTY(EditAnyWhere, Category = "Target")
		FVector startPoint_;

	class UGameInstanceBEU* myGI;

	UPROPERTY(EditAnywhere, Category = "Config")
		float maxDistanceBetweenPlayersUnlocked = 650.0f;

	UPROPERTY(EditAnywhere, Category = "Config")
		float maxDistanceWhenUnlocked = 550.0f;

	//UPROPERTY(Category = "Camera Pan Detection")
	//	bool isNearPlayer = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EndPlay(const EEndPlayReason::Type EndReason);


	//Locks the camera in the given coordinates with the given offset (armlength)
	UFUNCTION()
		void LockCamera(FCameraLockConfig config);

	//Unlocks the camera, so it targets the player(s) again.
	UFUNCTION()
		void UnlockCamera();

	UFUNCTION()
		void CalcWallPosition();

	UFUNCTION()
		void ShakeCamera(float shakeForce);

	UFUNCTION()
		void SmoothCameraMovement(float deltaTime);

	UFUNCTION()
		void LimitPlayerMovement(FVector cameraTarget);

	//Given the id of the player dead (0 lives), makes that the camera stops following that player
	UFUNCTION()
		void PlayerDeath(int playerID);

	UFUNCTION()
		void EnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
