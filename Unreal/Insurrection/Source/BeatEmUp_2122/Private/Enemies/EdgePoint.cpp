// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EdgePoint.h"

// Sets default values
AEdgePoint::AEdgePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isFree = true;

}

// Called when the game starts or when spawned
void AEdgePoint::BeginPlay()
{
	Super::BeginPlay();

  FHitResult ground;
  FCollisionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
  TraceParams.AddIgnoredActor(this);

  if (GetWorld()->LineTraceSingleByChannel(ground, GetActorLocation(), GetActorLocation() - FVector(0.0f, 0.0f, 500.0f),
    ECC_GameTraceChannel2, TraceParams)) {

    float x = GetActorLocation().X;
    float y = GetActorLocation().Y;
    float z = ground.GetActor()->GetActorLocation().Z;

    //Ricky eres el mejor, te quiero mucho, no se lo digas a javi, es un celoso :D
    SetActorLocation(FVector(x, y, z));
  }
	
}

// Called every frame
void AEdgePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

