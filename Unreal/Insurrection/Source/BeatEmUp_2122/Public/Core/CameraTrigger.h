// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CommonBEU.h"
#include "CameraTrigger.generated.h"

UCLASS()
class BEATEMUP_2122_API ACameraTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* myBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FCameraLockConfig lockConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool destroyOnActivate = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
