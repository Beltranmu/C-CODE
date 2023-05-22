// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Pickup.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("MyMesh");
	mesh->SetupAttachment(RootComponent);

	mesh->SetSimulatePhysics(true);
	//lockRotation was not working so I had to lock all of the axis manually
	mesh->BodyInstance.bLockXRotation = true;
	mesh->BodyInstance.bLockYRotation = true;
	mesh->BodyInstance.bLockZRotation = true;
	mesh->BodyInstance.bLockXTranslation = true;
	mesh->BodyInstance.bLockYTranslation = true;


}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::ShowInteractButton()
{
}

