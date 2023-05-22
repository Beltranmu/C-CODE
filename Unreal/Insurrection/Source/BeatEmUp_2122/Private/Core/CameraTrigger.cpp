// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CameraTrigger.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACameraTrigger::ACameraTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	myBody = CreateDefaultSubobject<UStaticMeshComponent>("My Body");
	myBody->SetupAttachment(RootComponent);


	myBody->bHiddenInGame = true;

}

// Called when the game starts or when spawned
void ACameraTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACameraTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

