// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/PointArrayActor.h"

// Sets default values
APointArrayActor::APointArrayActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APointArrayActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APointArrayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

