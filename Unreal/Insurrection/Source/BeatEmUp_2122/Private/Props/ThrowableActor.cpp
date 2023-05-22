// Fill out your copyright notice in the Description page of Project Settings.

#include "Props/ThrowableActor.h"
#include "TimerManager.h"
#include "Core/BaseEnemy.h"
#include "Player/TurboPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AThrowableActor::AThrowableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	myMesh = CreateDefaultSubobject<UStaticMeshComponent>("My Mesh");
	myMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AThrowableActor::BeginPlay()
{
	Super::BeginPlay();

	if (destroysOnCollisionWithTerrain) {
		myMesh->SetNotifyRigidBodyCollision(true);
	}

	OnActorBeginOverlap.AddDynamic(this, &AThrowableActor::OverlapWithEnemy);
	OnActorHit.AddDynamic(this, &AThrowableActor::OnHitBehaviour);
	
}

void AThrowableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  
	if(OnActorBeginOverlap.IsBound()){
		OnActorBeginOverlap.RemoveAll(this);
	}

  if (OnActorHit.IsBound()) {
		OnActorHit.RemoveAll(this);
  }

  if (hitEvent.IsBound()) {
		hitEvent.RemoveAll(this);
  }

}

// Called every frame
void AThrowableActor::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

void AThrowableActor::DestroyThrowableActor()
{

	Destroy();

}

void AThrowableActor::Pick(AActor * self, ATurboPlayer * player)
{
	//Self is not used here, maybe I will remove it when I finish implementing this interface

	player->attachedActor = this;
	myMesh->SetCollisionProfileName(TEXT("ThrowableOnAir"));
	this->K2_AttachRootComponentToActor(player, boneName, EAttachLocation::SnapToTarget);
	player->attachedActor->AddActorLocalOffset(offset);
	if (needsRotation) {
		myMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	}
	player->holdingObject = true;
  player->holdingObjectAnimation = true;
	isBeingHeld = true;
	player->GetCharacterMovement()->MaxWalkSpeed = player->defaultWalkSpeed * speedMultiplierWhenHeld;
	playerPickID = player->characterID;
	hitEvent.AddDynamic(player, &ATurboPlayer::callToUIEvent);
	
}

void AThrowableActor::Throw(FVector dir)
{

	isBeingHeld = false;
	onAir = true;
	myMesh->SetUsingAbsoluteRotation(true);
	myMesh->SetSimulatePhysics(true);
	myMesh->SetMassOverrideInKg(NAME_None, weight, true);
	if (!usePhysics) {
		myMesh->SetEnableGravity(false);
	}
	throwAngle.Normalize();
	//will flip or not the direction taking into account where the player is facing
	throwAngle *= dir;
	myMesh->AddImpulse(throwAngle * launchForce);

	GetWorldTimerManager().SetTimer(destroyTimer, this, &AThrowableActor::DestroyThrowableActor, timeToDestroy, false);


}

void AThrowableActor::OverlapWithEnemy(AActor* self, AActor* other)
{
	
	if (onAir) {

    if (other->GetClass()->ImplementsInterface(UEnemyDataUI::StaticClass())) {
      int32 enemy_type = 0;
      float hp_percentage = 0.0f;
      int32 points = 0;
      IEnemyDataUI::Execute_CatchUIDataEnemy(other, enemy_type, hp_percentage, points, playerPickID);
      hitEvent.Broadcast(enemy_type, hp_percentage, points);

    }
		if (other->GetClass()->ImplementsInterface(UIDamagable::StaticClass())) {

			ATurboPlayer* player = Cast<ATurboPlayer>(other);
			if (canDamagePlayer || !player) {

				
				FVector knockbackDir = (this->GetActorLocation() - other->GetActorLocation()).GetSafeNormal();
				knockbackDir *= stats.attack_knockback;
				knockbackDir.Z = stats.yKnockBackForce;
				IIDamagable::Execute_TakeAHit(other, knockbackDir, stats, false, this);
				//TODO-> add a reference of the player and do the hit event fot the UI
				//ownerplayer.broad


			}

		}
		

	}

}

void AThrowableActor::OnHitBehaviour(AActor * self, AActor * other, FVector normalImpulse, const FHitResult & hit)
{

	if (onAir && destroysOnCollisionWithTerrain) {

		DestroyThrowableActor();

	}

}

void AThrowableActor::TakeAHit_Implementation(FVector knockback_force, FAttackStats & attack_info, bool hitFromBehind, AActor* hiter)
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



