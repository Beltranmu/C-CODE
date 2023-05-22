// Fill out your copyright notice in the Description page of Project Settings.
//QUIEN CAMBIE EL START MENU LO ANIQUILO

#include "HUD/HUDMenu.h"
#include "HUD/WidgetContainer.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WidgetFirstScreen.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"

AHUDMenu::AHUDMenu() {

}

void AHUDMenu::BeginPlay() {

	Super::BeginPlay();

	auxController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (auxController != nullptr)
	{

		myWidget = CreateWidget<UWidgetContainer>(PlayerOwner, widgetSelector);
		myWidget->AddToViewport();

		UWidgetFirstScreen* firstScreen = Cast<UWidgetFirstScreen>(myWidget->WidgetSwitcher->GetActiveWidget());

		FInputModeUIOnly auxInputHelper = FInputModeUIOnly();

		if (firstScreen)
		{

			auxInputHelper.SetWidgetToFocus(firstScreen->startButton->baseButton->TakeWidget());

		}

		auxController->SetInputMode(auxInputHelper);
		auxController->bShowMouseCursor = true;

	}

}