// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/CameraBEU.h"
#include "GameInstanceBEU.generated.h"

/**
 * 
 */

UCLASS()
class BEATEMUP_2122_API UGameInstanceBEU : public UGameInstance
{
	GENERATED_BODY()
public:

  UPROPERTY(EditAnywhere, Category = "Players Lifes")
    int32 playerLife[2];

  UPROPERTY(EditAnywhere, Category = "Players Scores")
    int32 playerScore[2];

  UPROPERTY(EditAnywhere, Category = "Players MaxScores")
    int32 playerMaxScore[2];

  UPROPERTY(EditAnywhere, Category = "Players AOECharges")
    int playerAOEAttackCharge[2];
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPlayers")
		int32 numPlayers= 1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Input")
    bool secondPlayerKB; // True 2nd player has keyboard, otherwise player 1 have keyboard

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
		int nDialog = 0;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    int nImage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		ACameraBEU* camera;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    int currentLevelID;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Boolean")
	  bool thisBooleanExistsToPreventSpawningMoreThan2PlayersBoogaloo = false;




  UFUNCTION()
   void deathplayer();

  virtual void Init() override;

  void SetDificultyLevel(FName levelName);

};
