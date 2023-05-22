// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/BaseProp.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/Engine.h"

// Sets default values
ABaseProp::ABaseProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = body;

}

// Called when the game starts or when spawned
void ABaseProp::BeginPlay()
{
	Super::BeginPlay();
	FString msg;
	switch(ObjectToSpawn){
	case 0: msg = "SPAWN NADA"; break;
	case 1: msg = "SPAWN APPLE"; break;
	case 2: msg = "SPAWN CHICKEN"; break;
	case 3: msg = "SPAWN MENU DEL KFC"; break;
	}
	
}

// Called every frame
void ABaseProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

