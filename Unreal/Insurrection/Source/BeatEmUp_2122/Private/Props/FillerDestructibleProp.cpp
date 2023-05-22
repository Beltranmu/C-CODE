// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/FillerDestructibleProp.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Materials/MaterialInterface.h"
#include "Core/BaseCharacter.h"


// Sets default values
AFillerDestructibleProp::AFillerDestructibleProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");

	myMesh = CreateDefaultSubobject<UStaticMeshComponent>("Destructible Component");
	myMesh->SetupAttachment(RootComponent);
	myMesh->SetNotifyRigidBodyCollision(true);

	trigger = CreateDefaultSubobject<UBoxComponent>("Trigger / Collider");
	trigger->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFillerDestructibleProp::BeginPlay()
{
	Super::BeginPlay();

	damageStored = 0;
	UMaterialInterface* mat = myMesh->GetMaterial(0);
	fadeMat = UMaterialInstanceDynamic::Create(mat, NULL);
	myMesh->SetMaterial(0, fadeMat);

	OnActorBeginOverlap.AddDynamic(this, &AFillerDestructibleProp::OverlapBehaviour);
	
}

// Called every frame
void AFillerDestructibleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (dissapear) {

		opacity = FMath::Lerp(opacity, 0.0f, dissapearRate * DeltaTime);
		fadeMat->SetScalarParameterValue(TEXT("Opacity"), opacity);

	}

}

void AFillerDestructibleProp::OverlapBehaviour(AActor* self, AActor* other)
{
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(other);

	if (baseCharacter) {
		

		float damageApplied = 100.0f;
		const FVector hiterPos = other->GetActorLocation();
		const FVector destructiblePos = GetActorLocation();
		if (damageApplied < minDamage) {
			damageApplied = minDamage;
		}
		else {
			if (damageApplied > maxDamage) {
				damageApplied = maxDamage;
			}
		}
		FVector dir = destructiblePos - hiterPos;
		dir.Z /= 2;
		dir.Normalize();
		FVector impactPoint;
		myMesh->GetClosestPointOnCollision(other->GetActorLocation(), impactPoint);
		
		
		trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		myMesh->SetSimulatePhysics(true);
		myMesh->AddImpulse(dir * forceApplied);
		if (!GetWorldTimerManager().IsTimerActive(blinkTimer)) {

			GetWorldTimerManager().SetTimer(blinkTimer, this, &AFillerDestructibleProp::Blink, blinkTime, true, 1.25f);
			blinkTime *= 0.75;
		}
			
		GetWorldTimerManager().SetTimer(destroyTimer, this, &AFillerDestructibleProp::DestroyFillerProp, 2.5, false);

	}

}

void AFillerDestructibleProp::Blink()
{

	dissapear = true;
	myMesh->SetVisibility(!myMesh->IsVisible());

}

void AFillerDestructibleProp::DestroyFillerProp()
{
	Destroy();
}

