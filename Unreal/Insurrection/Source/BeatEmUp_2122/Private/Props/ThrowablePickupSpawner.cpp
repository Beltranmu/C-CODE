// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ThrowablePickupSpawner.h"
#include "Props/Pickup.h"

AThrowablePickupSpawner::AThrowablePickupSpawner() {

    PrimaryActorTick.bCanEverTick = true;


    myMesh = CreateDefaultSubobject<UStaticMeshComponent>("My Mesh");
    myMesh->SetupAttachment(RootComponent);
}

void AThrowablePickupSpawner::BeginPlay()
{
    Super::BeginPlay();

}
void AThrowablePickupSpawner::DestroyThrowableActor() {

	/*FActorSpawnParameters spawnParams;
	int32 i = FMath::RandRange(0, pickupsToSpawn.Num() - 1);

  FHitResult ground;
  FCollisionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
  FVector spawnPosition = GetActorLocation() + FVector(0.0f, 0.0f, 0.0f);
  TraceParams.AddIgnoredActor(this);
  if (GetWorld()->LineTraceSingleByChannel(ground, GetActorLocation(), GetActorLocation() - FVector(0.0f, 0.0f, 500.0f),
    ECC_GameTraceChannel18, TraceParams)) {
    spawnPosition.Z = ground.ImpactPoint.Z + 35.0f;
  }

  AActor* pickup = GetWorld()->SpawnActor<AActor>(pickupsToSpawn[i], spawnPosition, FRotator::ZeroRotator, spawnParams);
  if (IsValid(pickup)) { pickup->SetLifeSpan(15.0f); }*/



  numberOfElements = pickupsToSpawn.Num();
  uint32 i;
  int lower_limitP = 0;
  int randomN = FMath::RandRange(1, 100);
  for (i = 0; i < numberOfElements; ++i) {
    if (lower_limitP < randomN && randomN < pickupRatio[i] + lower_limitP) {
      FActorSpawnParameters spawnParams;
      spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
      FHitResult ground;
      FCollisionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
      TraceParams.AddIgnoredActor(this);
      FVector spawnPosition = GetActorLocation() + FVector(0.0f, 0.0f, 0.0f);
      spawnPosition += FVector(0.0f, 0.0f, 20.f);
      if (GetWorld()->LineTraceSingleByChannel(ground, GetActorLocation(), GetActorLocation() - FVector(0.0f, 0.0f, 500.0f),
        ECC_GameTraceChannel18, TraceParams)) {

        //Ricky eres el mejor, te quiero mucho, no se lo digas a javi, es un celoso :D
        spawnPosition.Z = ground.ImpactPoint.Z + 100.0f;
      }
      AActor* pickup = GetWorld()->SpawnActor<AActor>(pickupsToSpawn[i], spawnPosition, FRotator::ZeroRotator, spawnParams);
      if (IsValid(pickup)) { pickup->SetLifeSpan(15.0f); }
    }
    lower_limitP += pickupRatio[i];
  }







	Destroy();

}

void AThrowablePickupSpawner::TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter)
{

    if (CanBeDestroyed) {

        health -= attack_info.attack_dmg;
        if (health <= 0.0f) {

            //Avoiding recursive calls, explosive for example can try to damage himself which would cause recursivity and a stack overflow.
            myMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            //Call anything that must happen when destroyed
            DestroyThrowableActor();
        }

    }

}

