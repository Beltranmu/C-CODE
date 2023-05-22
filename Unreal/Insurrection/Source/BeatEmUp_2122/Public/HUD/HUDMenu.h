// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDMenu.generated.h"

class UWidgetContainer;

UCLASS()
class BEATEMUP_2122_API AHUDMenu : public AHUD
{
	GENERATED_BODY()

public:

	AHUDMenu();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WidgetSwitch")
		TSubclassOf<UWidgetContainer> widgetSelector;

	UPROPERTY(BlueprintReadWrite, Category = "WidgetOption")
		UWidgetContainer* myWidget;

	UPROPERTY(BlueprintReadWrite, Category = "WidgetOption")
		APlayerController* auxController;

	void BeginPlay() override;

	//UFUNCTION()
	//	void goBackWidget();

};
