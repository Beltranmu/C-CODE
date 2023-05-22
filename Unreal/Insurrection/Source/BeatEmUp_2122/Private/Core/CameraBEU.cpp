// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CameraBEU.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Enemies/EnemyCommander.h"
#include "Enemies/SpawnTrigger.h"
#include "Enemies/SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Player/TurboPlayer.h"
#include "Core/GameInstanceBEU.h"
#include "Engine/Engine.h"


// Sets default values
ACameraBEU::ACameraBEU()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	myCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	myArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");

	rightWall = CreateDefaultSubobject<UStaticMeshComponent>("RWall");
	leftWall = CreateDefaultSubobject<UStaticMeshComponent>("LWall");

	myArm->SetupAttachment(RootComponent);
	myCamera->SetupAttachment(myArm);
	locked = false;
	transition = false;

	rightWall->SetupAttachment(RootComponent);
	leftWall->SetupAttachment(RootComponent);

	myArm->SetWorldRotation(cameraRotation);
	myArm->bEnableCameraLag = true;
	myArm->CameraLagSpeed = cameraLag;
	rightWall->SetVisibility(false);
	leftWall->SetVisibility(false);
	leftWall->SetRelativeLocation(FVector(myCamera->GetRelativeLocation().X + cameraWallsOffset.X, myCamera->GetRelativeLocation().Y - cameraWallsOffset.Y, myCamera->GetRelativeLocation().Z + cameraWallsOffset.Z));
	rightWall->SetRelativeLocation(FVector(myCamera->GetRelativeLocation().X + cameraWallsOffset.X, myCamera->GetRelativeLocation().Y + cameraWallsOffset.Y, myCamera->GetRelativeLocation().Z + cameraWallsOffset.Z));
	

}

// Called when the game starts or when spawned
void ACameraBEU::BeginPlay()
{
	Super::BeginPlay();

	startPoint_ = GetActorLocation();

	player1 = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player2 = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1));


	myArm->TargetArmLength = defaultCameraOffset;
	cameraOffset = defaultCameraOffset;
	myArm->SetWorldRotation(defaultCameraRotation);

	leftWall->SetRelativeLocation(FVector(myCamera->GetRelativeLocation().X + cameraWallsOffset.X, myCamera->GetRelativeLocation().Y - cameraWallsOffset.Y, myCamera->GetRelativeLocation().Z + cameraWallsOffset.Z));
	rightWall->SetRelativeLocation(FVector(myCamera->GetRelativeLocation().X + cameraWallsOffset.X, myCamera->GetRelativeLocation().Y + cameraWallsOffset.Y, myCamera->GetRelativeLocation().Z + cameraWallsOffset.Z));
	CalcWallPosition();

	_cameraEvent.AddDynamic(this, &ACameraBEU::LockCamera);
	player1->cameraShakeEvent.AddDynamic(this, &ACameraBEU::ShakeCamera);
	if(nullptr != player2 && IsValid(player2))
		player2->cameraShakeEvent.AddDynamic(this, &ACameraBEU::ShakeCamera);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this);
	myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));
	myGI->camera = this;

	leftWall->OnComponentBeginOverlap.AddDynamic(this, &ACameraBEU::EnemyOverlap);
	rightWall->OnComponentBeginOverlap.AddDynamic(this, &ACameraBEU::EnemyOverlap);

	target_ = GetActorLocation();
	cameraRotation = defaultCameraRotation;

}

// Called every frame
void ACameraBEU::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Estoy hay que quitarlo de aqui cuando hagamos el local en plan bien :D
	player2 = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1));

	SmoothCameraMovement(DeltaTime);
	if (!locked) {

		FVector newPosition;

		if (!player2) {

			if (player1)
				newPosition = player1->GetActorLocation() + playerOffset;

		}
		else {
			//both players alive
			if (myGI->playerLife[0] > 0 && myGI->playerLife[1] > 0) {

				newPosition = player1->GetActorLocation() - player2->GetActorLocation();
				newPosition = player1->GetActorLocation() - newPosition / 2;

			}
			else {
				//only player 1 alive
				if (myGI->playerLife[0] > 0) {
					newPosition = player1->GetActorLocation() + playerOffset;

				}
				else {
					newPosition = player2->GetActorLocation() + playerOffset;

				}

			}


		}

		if (newPosition.X > maxCameraOffsetXAxis) {
			newPosition.X = maxCameraOffsetXAxis;
		}
		else {
			if (newPosition.X < minCameraOffsetXAxis)
				newPosition.X = minCameraOffsetXAxis;
		}

		//This is for now the best way to prevent players leaving camera bounds without movement restriction.
		if (!playersTooFar) {

			//this->SetActorLocation(newPosition);
			CalcWallPosition();
			target_ = newPosition;

			if (transition) {

				if (FVector::Dist(GetActorLocation(), target_) < 45.0f) {
			
					transition = false;
					scalingSmoothUnlockSpeed = 0.0f;
			
				}
				else {

					scalingSmoothUnlockSpeed += DeltaTime * 0.2f;
					SetActorLocation(FMath::Lerp(GetActorLocation(), target_, scalingSmoothUnlockSpeed));
				
				}

			}
			else {
				SetActorLocation(target_);
			}
		
		}

		LimitPlayerMovement(GetActorLocation());

	}
	else {

		SetActorLocation(FMath::Lerp(GetActorLocation(), target_, smoothRate * DeltaTime));

	}


}

void ACameraBEU::LockCamera(FCameraLockConfig config) {

	if (config.lock) {

		locked = true;
		leftWall->SetCollisionProfileName("ArenaWalls");
		rightWall->SetCollisionProfileName("ArenaWalls");

		//checks if has to lock in place or lock point actor is null
		if (!config.lockInPlace && config.cameraLockPoint) {

			target_ = config.cameraLockPoint->GetActorLocation();

		}

		//In multiplayer the player that does not trigger the spawner can end up out of the combat area
		if (player1 && player2) {
			if (player1->alive && player2->alive) {

				//Check who is further
				if (FVector::Dist(GetActorLocation(), player1->GetActorLocation()) > 
					FVector::Dist(GetActorLocation(), player2->GetActorLocation())) {

					if (FVector::Dist(GetActorLocation(), player1->GetActorLocation()) > minDistanceToTeleportPlayer) {
						player1->SetActorLocation(target_);
					}

				}
				else {
					
					if (FVector::Dist(GetActorLocation(), player2->GetActorLocation()) > minDistanceToTeleportPlayer) {
						player2->SetActorLocation(target_);
					}

				}

			}

			player1->blockMovementLeft = false;
			player1->blockMovementRight = false;
			player2->blockMovementLeft = false;
			player2->blockMovementRight = false;

		}

	}

	if (config.changeOffset) {

		cameraOffset = config.offset;
		transition = true;

		//if change offset is set but lock isn't, the offset of the camera will change permanently. 
		if (!config.lock) {
			defaultCameraOffset = config.offset;
		}

	}

	if (config.changeRotation) {

		cameraRotation = config.newRotation;
		transition = true;

		//if change rotation is set but lock isn't, the rotation of the camera will change permanently. 
		if (!config.lock) {
			defaultCameraRotation = config.newRotation;
		}

	}

	CalcWallPosition();

}

void ACameraBEU::UnlockCamera() {

	//multiplayer check
	if (player1 && player2) {

		if(myGI->playerLife[0] > 0 && myGI->playerLife[1] > 0){

			FVector cameraLockPoint = GetActorLocation();
			FVector player1Pos = player1->GetActorLocation();
			FVector player2Pos = player2->GetActorLocation();
			player1Pos.Z += 25.0f;
			player2Pos.Z += 25.0f;
		
			//Teleport player if too far
			if (FVector::Dist(player1Pos, cameraLockPoint) > maxDistanceWhenUnlocked) {
				//left
				if (player1Pos.Y < cameraLockPoint.Y) {
					player1Pos = cameraLockPoint;
					player1Pos.Y -= maxDistanceWhenUnlocked;
					player1->SetActorLocation(player1Pos);
				}
				else {
					player1Pos = cameraLockPoint;
					player1Pos.Y += maxDistanceWhenUnlocked;
					player1->SetActorLocation(player1Pos);
				}
			}
			//Same for player 2, I know, repeating this hurts
			if (FVector::Dist(player2Pos, cameraLockPoint) > maxDistanceWhenUnlocked) {
				//left
				if (player2Pos.Y < cameraLockPoint.Y) {
					player2Pos = cameraLockPoint;
					player2Pos.Y -= maxDistanceWhenUnlocked;
					player1->SetActorLocation(player2Pos);
				}
				else {
					player2Pos = cameraLockPoint;
					player2Pos.Y += maxDistanceWhenUnlocked;
					player2->SetActorLocation(player2Pos);
				}
			}
	
		}
	}
	locked = false;
	myArm->TargetArmLength = defaultCameraOffset;
	CalcWallPosition();
	transition = true;
	leftWall->SetCollisionProfileName("CameraLimitBehaviour");
	rightWall->SetCollisionProfileName("CameraLimitBehaviour");

	//Reset offset & rotation for future changes
	cameraOffset = defaultCameraOffset;
	cameraRotation = defaultCameraRotation;


}

void ACameraBEU::CalcWallPosition()
{

	leftWall->SetRelativeLocation(FVector(myCamera->GetRelativeLocation().X + cameraWallsOffset.X, myCamera->GetRelativeLocation().Y - cameraWallsOffset.Y, myCamera->GetRelativeLocation().Z + cameraWallsOffset.Z));
	rightWall->SetRelativeLocation(FVector(myCamera->GetRelativeLocation().X + cameraWallsOffset.X, myCamera->GetRelativeLocation().Y + cameraWallsOffset.Y, myCamera->GetRelativeLocation().Z + cameraWallsOffset.Z));
	rightWall->SetUsingAbsoluteRotation(true);
	rightWall->SetWorldRotation(FRotator(0.0f, myCamera->FieldOfView / 2.0f, 0.0f));
	leftWall->SetUsingAbsoluteRotation(true);
	leftWall->SetWorldRotation(FRotator(0.0f, -(myCamera->FieldOfView / 2.0f), 0.0f));

}

void ACameraBEU::ShakeCamera(float shakeForce)
{

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(shakeClass, shakeForce);

}

void ACameraBEU::SmoothCameraMovement(float deltaTime) {

		//For temporal transition
	if (locked) {

		myArm->TargetArmLength = FMath::Lerp(myArm->TargetArmLength, cameraOffset, smoothRate * deltaTime);
		myArm->SetWorldRotation(FMath::Lerp(myArm->GetComponentRotation(), cameraRotation, smoothRate * deltaTime));

	}//Permanent transitions
	else {

		myArm->TargetArmLength = FMath::Lerp(myArm->TargetArmLength, defaultCameraOffset, smoothRate * deltaTime);
		myArm->SetWorldRotation(FMath::Lerp(myArm->GetComponentRotation(), defaultCameraRotation, smoothRate * deltaTime));

	}

}

void ACameraBEU::LimitPlayerMovement(FVector cameraTarget)
{

	FVector currentPlayer1Pos;
	FVector currentPlayer2Pos;

	if (player1) {
		currentPlayer1Pos = player1->GetActorLocation();
		//Calc max Y offset
		limitYOffsetP1 = myCamera->GetComponentLocation().X - currentPlayer1Pos.X;
		//2.1 instead
		limitYOffsetP1 *= FMath::Tan((myCamera->FieldOfView * 3.141592f / 180.0f) / 2.2f);
		limitYOffsetP1 = FMath::Abs<float>(limitYOffsetP1);

		if (currentPlayer1Pos.Y - cameraTarget.Y < -limitYOffsetP1) {
			player1->blockMovementLeft = true;
		}
		else {

			player1->blockMovementLeft = false;

			if (currentPlayer1Pos.Y - cameraTarget.Y > limitYOffsetP1) {
				player1->blockMovementRight = true;
			}
			else {
				player1->blockMovementRight = false;
			}
		}

	}
	if (player2) {
		currentPlayer2Pos = player2->GetActorLocation();
		limitYOffsetP2 = myCamera->GetComponentLocation().X - currentPlayer2Pos.X;
		//2.1 instead
		limitYOffsetP2 *= FMath::Tan((myCamera->FieldOfView * 3.141592f / 180.0f) / 2.2f);
		limitYOffsetP2 = FMath::Abs<float>(limitYOffsetP2);

		if (currentPlayer2Pos.Y - cameraTarget.Y < -limitYOffsetP2) {
			player2->blockMovementLeft = true;
		}
		else {

			player2->blockMovementLeft = false;

			if (currentPlayer2Pos.Y - cameraTarget.Y > limitYOffsetP2) {
				player2->blockMovementRight = true;
			}
			else {
				player2->blockMovementRight = false;
			}
		}

	}

	//Case both players try leaving the area at the same time on different X position
	if (player1 && player2) {

		if (myGI->playerLife[0] > 0 && myGI->playerLife[1] > 0) {


			if ((player1->blockMovementLeft || player1->blockMovementRight) && (player2->blockMovementLeft || player2->blockMovementRight) 
				|| FVector::Dist(player1->GetActorLocation(),player2->GetActorLocation()) > maxDistanceBetweenPlayersUnlocked){

				playersTooFar = true;

			}
			else {

				playersTooFar = false;

			}
			/*if (FMath::Abs<float>(currentPlayer1Pos.Y - currentPlayer2Pos.Y) + 50.0f > limitYOffsetP1 + limitYOffsetP2) {
			
				//player1 at left
				/*if (currentPlayer1Pos.Y < currentPlayer2Pos.Y) {
					player1->blockMovementLeft = true;
					player2->blockMovementRight = true;
				}
				else {
					player1->blockMovementRight = true;
					player2->blockMovementLeft = true;
				}*/

				/*playersTooFar = true;

			}
			else {

				playersTooFar = false;

			}*/

		}

	}

}

void ACameraBEU::PlayerDeath(int playerID) {}

void ACameraBEU::EnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ABaseEnemy* tmpEnemy = Cast<ABaseEnemy>(OtherActor);
	
	if (tmpEnemy) {

		if(tmpEnemy->state.alteredStatus) {
			_triggerKillEvent.Broadcast(tmpEnemy->characterID);
			tmpEnemy->_onDeath.Broadcast(tmpEnemy, tmpEnemy->playerIndex);
		}

	}

}

void ACameraBEU::EndPlay(EEndPlayReason::Type EndReason) {
	Super::EndPlay(EndReason);

	if (_cameraEvent.IsBound()) {

		_cameraEvent.RemoveAll(this);
	}
}



