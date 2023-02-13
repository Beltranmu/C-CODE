// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "mainmenu_gm.generated.h"

/**
 * 
 */
class AarktequeraHUD;


UCLASS()
class ARKTEQUERA_API Amainmenu_gm : public AGameModeBase
{
	GENERATED_BODY()

public:
  Amainmenu_gm();
  virtual void BeginPlay() override;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
  AarktequeraHUD* game_hud;




};
