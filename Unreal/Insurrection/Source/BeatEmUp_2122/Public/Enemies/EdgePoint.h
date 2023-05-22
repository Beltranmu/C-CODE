// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EdgePoint.generated.h"

UCLASS()
class BEATEMUP_2122_API AEdgePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEdgePoint();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Idle Side")
    bool isFree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
