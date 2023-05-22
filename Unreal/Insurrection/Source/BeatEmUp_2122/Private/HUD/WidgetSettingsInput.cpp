// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WidgetSettingsInput.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"

void UWidgetSettingsInput::NativeConstruct() {

	Super::NativeConstruct();

	backButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsInput::backBehaviour);

}

void UWidgetSettingsInput::backBehaviour() {

	if (isInMenu) 
	{
	
		APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FInputModeGameOnly auxInputHelper = FInputModeGameOnly();
		player0->SetInputMode(auxInputHelper);
		changeScreen(backID);
	
	}
	else 
	{
	
		changeScreen(backID);

	}


}

void UWidgetSettingsInput::setFocusToButton() {

	backButton->baseButton->SetFocus();

}