// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/MyDestructibleProp.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "DestructibleMesh.h"
#include "TimerManager.h"
#include "Materials/MaterialInterface.h"

// Sets default values
AMyDestructibleProp::AMyDestructibleProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");

	destructibleComp = CreateDefaultSubobject<UDestructibleComponent>("Destructible Component");
	destructibleComp->SetupAttachment(RootComponent);
	destructibleComp->SetNotifyRigidBodyCollision(true);

	trigger = CreateDefaultSubobject<UBoxComponent>("Trigger / Collider");
	trigger->SetupAttachment(destructibleComp);

 
}


// Called when the game starts or when spawned
void AMyDestructibleProp::BeginPlay()
{
	Super::BeginPlay();
	damageStored = 0;
	UMaterialInterface* mat = destructibleComp->GetMaterial(0);
	fadeMat = UMaterialInstanceDynamic::Create(mat, NULL);
	destructibleComp->SetMaterial(0, fadeMat);
  /*->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
  FVector scale = trigger->GetRelativeScale3D();
  trigger->SetRelativeScale3D(FVector(scale.X, scale.Y, 6.0f));*/
}

// Called every frame
void AMyDestructibleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (dissapear) {

		opacity = FMath::Lerp(opacity, 0.0f, dissapearRate* DeltaTime);
		fadeMat->SetScalarParameterValue(TEXT("Opacity"), opacity);

	}

}

void AMyDestructibleProp::TakeAHit_Implementation(FVector knockback_force, FAttackStats & attack_info, bool hitFromBehind, AActor* hiter)
{

	float damageApplied = attack_info.attack_dmg;
	const FVector hiterPos = hiter->GetActorLocation();
	const FVector destructiblePos = GetActorLocation();
	if (damageApplied < minDamage) {
		damageApplied = minDamage;
	}
	else{
		if(damageApplied > maxDamage){
			damageApplied = maxDamage;
		}
	}
	FVector dir = FVector::LeftVector;
	if (hiterPos.Y < destructiblePos.Y) {
		dir = FVector::RightVector;
	}
	dir.Z = 0.5f;
	dir *= 10;
	FVector impactPoint;
	destructibleComp->GetClosestPointOnCollision(hiter->GetActorLocation(), impactPoint);
	impactPoint *= 0.5;
	FVector newPoint = destructibleComp->GetRelativeTransform().TransformPosition(impactPoint);
	destructibleComp->ApplyDamage(damageApplied, destructibleComp->GetComponentLocation() + impactPoint, dir, forceApplied);
	damageStored += damageApplied;
	if (destructibleComp->GetDestructibleMesh()->DefaultDestructibleParameters.DamageParameters.DamageThreshold <= damageStored) {

		trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (!GetWorldTimerManager().IsTimerActive(blinkTimer)) {

			GetWorldTimerManager().SetTimer(blinkTimer, this, &AMyDestructibleProp::Blink, blinkTime, true, 1.25f);
			blinkTime *= 0.75;
		}
		GetWorldTimerManager().SetTimer(destroyTimer, this, &AMyDestructibleProp::DestroyDestructibleProp, 2.5, false);
	}

}

void AMyDestructibleProp::Blink()
{

	dissapear = true;
	destructibleComp->SetVisibility(!destructibleComp->IsVisible());

}

void AMyDestructibleProp::DestroyDestructibleProp()
{

	Destroy();
}
