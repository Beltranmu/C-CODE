// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class ARKTEQUERA_API UPauseMenu : public UUserWidget
{
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UButton* ResumeButton;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UButton* MainButton;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
    UButton* ExitButton;

};
