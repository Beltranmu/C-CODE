// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "arktquera_player_controller.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTest111, float, x);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTest111, float, x, float, y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCraftingTest, int, recipe);
UCLASS()
class ARKTEQUERA_API Aarktquera_player_controller : public APlayerController
{
	GENERATED_BODY()

  public:
    virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause Menu")
      bool game_paused;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inv Menu")
    bool show_inventory;
 
  UPROPERTY(BlueprintAssignable, Category = "Gameplay")
    FTest111 Test1;

  UPROPERTY(BlueprintAssignable, Category = "Gameplay")
    FCraftingTest crafting_event;

  virtual void SetupInputComponent() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  UFUNCTION()
    void PauseMenu();

  UFUNCTION()
    void InvetoryMenu();

  UFUNCTION()
    void TestF();

  UFUNCTION()
    void Craft1();
  UFUNCTION()
    void Craft2();
  UFUNCTION()
    void Craft3();
  UFUNCTION()
    void Craft4();
  UFUNCTION()
    void Craft5();
/*UFUNCTION()
    void Craft6();
  UFUNCTION()
    void Craft7();
  UFUNCTION()
    void Craft8();
  UFUNCTION()
    void Craft9();*/

};
