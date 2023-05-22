// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SpawnTrigger.h"
#include "Components/StaticMeshComponent.h"
#include "Player/TurboPlayer.h"
#include "Engine/BlockingVolume.h"

// Sets default values
ASpawnTrigger::ASpawnTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	triggerMesh = CreateDefaultSubobject<UStaticMeshComponent>("Trigger Mesh");

	RootComponent = triggerMesh;

}


// Called when the game starts or when spawned
void ASpawnTrigger::BeginPlay()
{
	Super::BeginPlay();

	triggerMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpawnTrigger::PlayerOverlap);

	/*if (0 != invisibleWall.Num()) {
	
		for (int i = 0; i < 2; ++i) {
	
			if (invisibleWall[i] != nullptr) {

				invisibleWall[i]->SetActorEnableCollision(false);
			
			}
			else {
				invisibleWall.Empty();
			}
	
		}
	
	}*/
	
}

void ASpawnTrigger::EndPlay(const EEndPlayReason::Type type){

	Super::EndPlay(type);
	if(triggerMesh->OnComponentBeginOverlap.IsBound()){
		triggerMesh->OnComponentBeginOverlap.Remove(this, "PlayerOverlap");
	}
}

// Called every frame
void ASpawnTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnTrigger::PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	ATurboPlayer* tmpPlayer = nullptr;
	tmpPlayer = Cast<ATurboPlayer>(OtherActor);
	triggerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	if(tmpPlayer){

		tmpPlayer->triggerEvent.Broadcast(this);

	}


	//Destroy(true);

}