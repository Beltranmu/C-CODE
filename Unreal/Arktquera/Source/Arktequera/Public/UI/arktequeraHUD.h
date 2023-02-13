// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "arktequeraWidget.h"
#include "arktequeraHUD.generated.h"

/**
 * 
 */
class UarktequeraWidget;

class UInventorywidget;
class UPauseMenu;

class UResources;
class UTexture2D;
UCLASS()
class ARKTEQUERA_API AarktequeraHUD : public AHUD
{
	GENERATED_BODY()
	
  public:
    // UI Menu
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
      TSubclassOf<UarktequeraWidget> BPGameWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
      TSubclassOf<UInventorywidget> BPInventoryWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
      TSubclassOf<UPauseMenu> BPPauseWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMG")
      TArray<UTexture2D*> inventory_img;


    void BeginPlay() override;

    UFUNCTION()
      void UpdateInvetory(TArray<UResources*> myResources);

    UFUNCTION()
      void EnablePauseMenu(bool enabled);

    UFUNCTION()
      void EnableInventory(bool enabled);

    UFUNCTION()
      void ReturnToMain();

  protected:

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
   UPauseMenu* PauseWidget;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
   UInventorywidget* InventoryWidget;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
   UarktequeraWidget* GameWidget;

   


};
