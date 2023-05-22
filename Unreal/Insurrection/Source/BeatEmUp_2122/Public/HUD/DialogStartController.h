// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HUD/HUDGameLevel.h"
#include "DialogStartController.generated.h"

UCLASS()
class BEATEMUP_2122_API ADialogStartController : public AActor
{
	GENERATED_BODY()

public:

	ADialogStartController();

	UPROPERTY()
		AHUDGameLevel* hud_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start Dialog Control")
		int dialogToStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start Dialog Control")
		int dialogToEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start Dialog Control")
		int imageToShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start Dialog Control")
		float slowValue;

	FTimerHandle startLevelTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start Dialog Control")
		float timeToShow = 10.0f;

	UPROPERTY()
		bool isUsed;

	UPROPERTY()
		class UGameInstanceBEU* myGI_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class ACameraBEU* camera_;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void StartLevelImage();

	UFUNCTION()
		void StartChangeToDialog();

	UFUNCTION()
		void EndStartLevelDialog();

};
