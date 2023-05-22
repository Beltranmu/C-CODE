// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameModeBEU.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API AMenuGameModeBEU : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMenuGameModeBEU();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
		bool controllers;

};
