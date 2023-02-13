// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "mainMenuHUD.generated.h"

/**
 * 
 */

class UMenuWidget;
UCLASS()
class ARKTEQUERA_API AmainMenuHUD : public AHUD
{
	GENERATED_BODY()
	

public:

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    TSubclassOf<UMenuWidget> BPMenuWidget;

  virtual void BeginPlay() override;
  UFUNCTION()
    void StartGame();

  UFUNCTION()
    void Exit();


  UMenuWidget* MenuWidget;
};
