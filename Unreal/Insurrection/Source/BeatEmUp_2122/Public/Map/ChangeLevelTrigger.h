// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HUD/HUDGameLevel.h"
#include "ChangeLevelTrigger.generated.h"


class UStaticMeshComponent;
UCLASS()
class BEATEMUP_2122_API AChangeLevelTrigger : public AActor
{
	GENERATED_BODY()

public:

	AChangeLevelTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		AHUDGameLevel* hud_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int dialogToStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int dialogToEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int imageToShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		float slowValue;

	UPROPERTY()
		class UGameInstanceBEU* myGI_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		bool alreadyUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Level Paramenters")
		float timeCL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Level Paramenters")
		FString mapName;

	FTimerHandle timerCL;

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void TriggerChangeLevel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void changeLevel();

	void continueGame();

};
