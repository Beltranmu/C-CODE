// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Inventorywidget.generated.h"

/**
 * 
 */
UCLASS()
class ARKTEQUERA_API UInventorywidget : public UUserWidget
{
	GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot1;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot2;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot3;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot4;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot5;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot6;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot7;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot8;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot9;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UVerticalBox* Slot10;
	
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pause")
    TArray<UVerticalBox*> Slots;

  UFUNCTION()
   void Init() ;
  UFUNCTION()
   void Clean() ;
};
