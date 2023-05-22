// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/BossRazor.h"
#include "Components/StaticMeshComponent.h"
#include "Player/TurboPlayer.h"

// Sets default values
ABossRazor::ABossRazor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	myMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABossRazor::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(destroyTimer, this, &ABossRazor::DestroyRazor, 5.5f, false);
	OnActorBeginOverlap.AddDynamic(this, &ABossRazor::OverlapBehaviour);
	
}

// Called every frame
void ABossRazor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Movement
	FVector newLocation = GetActorLocation();
	newLocation += movementData.dir * (movementData.speed * DeltaTime);
	movementData.dir -= movementData.decrementRate * DeltaTime;
	SetActorLocation(newLocation);

	//Rotation
	FRotator newRotation = GetActorRotation();
	newRotation.Yaw += movementData.rotationSpeed * DeltaTime;
	SetActorRotation(FQuat(newRotation));

}

void ABossRazor::DestroyRazor()
{

	Destroy();

}

void ABossRazor::OverlapBehaviour(AActor* self, AActor* other)
{

	ATurboPlayer* player = Cast<ATurboPlayer>(other);
	if (player) 
	{
		bool alreadyHit = false;
		for (int i = 0; i < playerHitArray.Num(); i++) {

			if (player == playerHitArray[i]) {
				alreadyHit = true;
			}
		
		}

		if (!alreadyHit) {

			FVector knockbackDir = (other->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			knockbackDir *= combatStats.attack_knockback;
			knockbackDir.Z = combatStats.yKnockBackForce;
			player->Execute_TakeAHit(player, knockbackDir, combatStats, false, this);

			playerHitArray.Add(player);

		}

	}

}

