// Fill out your copyright notice in the Description page of Project Settings.


#include <Enemies/DinoMovementComponent.h>
#include <GameFramework/Actor.h>
#include <Engine.h>

void UDinoMovementComponent::TickComponent(float DeltaTime,
  enum ELevelTick TickType,
  FActorComponentTickFunction * ThisTickFunction) {

  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  FVector totalMov = ConsumeInputVector();
  FVector gravity = FVector(0.f, 0.f, -1.f);
  gravity *= 9.8f;

  if (!totalMov.IsNearlyZero()) {

    FHitResult Hit;

    SafeMoveUpdatedComponent(totalMov, UpdatedComponent->GetComponentRotation(), true, Hit);
    SafeMoveUpdatedComponent(gravity, UpdatedComponent->GetComponentRotation(), true, Hit);

    /*if (Hit.IsValidBlockingHit()) {

      float dotProduct = FVector::DotProduct(totalMov.GetSafeNormal(), Hit.Normal);

      if (FMath::IsNearlyEqual(dotProduct, -1.0f)) {

        SafeMoveUpdatedComponent(FVector::UpVector);

      }else {

      }

    }*/

  }

}

float UDinoMovementComponent::getMovement() {

  return rand() % 360;

}

FVector UDinoMovementComponent::getNextMovement() {

  float nextAngle = getMovement();
  FString tmp = FString::SanitizeFloat(nextAngle);
  nextAngle = nextAngle * 3.141592817f / 180.f;
  GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, tmp);
  FVector deltaRot = FVector(cos(nextAngle), sin(nextAngle), 0.f);

  return deltaRot;

}