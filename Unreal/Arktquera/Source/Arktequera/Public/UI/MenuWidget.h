// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARKTEQUERA_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite,  meta = (BindWidget), Category = "Menu")
    UButton* StartButton;

  UPROPERTY(BlueprintReadWrite,  meta = (BindWidget), Category = "Menu")
    UButton* ExitButton;

	
};
