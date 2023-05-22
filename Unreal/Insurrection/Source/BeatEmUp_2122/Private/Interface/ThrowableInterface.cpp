// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/ThrowableInterface.h"
#include "Components/StaticMeshComponent.h"
#include "Player/TurboPlayer.h"

// Add default functionality here for any IThrowableInterface functions that are not pure virtual.


void IThrowableInterface::Pick(AActor* self, ATurboPlayer* player) {

	//Attached actor tiene que pasar a ser un actor simplemente.
	player->attachedActor = self;
	TArray<UStaticMeshComponent*> components;
	self->GetComponents<UStaticMeshComponent>(components);
	if (components[0]) {
		components[0]->SetCollisionProfileName(TEXT("ThrowableOnAir"));
		components[0]->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		self->K2_AttachRootComponentToActor(player, FName("NONE"), EAttachLocation::SnapToTarget);
		player->attachedActor->AddActorLocalOffset(FVector(0.0f, 0.0f, 125.0f));
		player->holdingObject = true;
	}
	
	//player->attachedActor->hitEvent.AddDynamic(this, &ATurboPlayer::callToUIEvent);
	/*
	self->K2_AttachRootComponentToActor(this, FName("NONE"), EAttachLocation::SnapToTarget);
	attachedActor->AddActorLocalOffset(FVector(0.0f, 0.0f, 125.0f));
	attachedActor->isBeingHeld = true;
	attachedActor->myMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	holdingObject = true;
	*/
}

void IThrowableInterface::Throw(FVector dir)
{

	//For now it has no sense to write anything as this will be override.

}

void IThrowableInterface::OverlapWithEnemy(AActor * self, AActor * other)
{

	//For now it has no sense to write anything as this will be override.

}

