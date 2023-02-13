// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "DinoMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARKTEQUERA_API UDinoMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
public:

  UFUNCTION()
    float getMovement();

  UFUNCTION()
    FVector getNextMovement();
};
