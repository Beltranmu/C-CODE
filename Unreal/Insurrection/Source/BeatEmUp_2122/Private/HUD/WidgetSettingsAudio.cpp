// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WidgetSettingsAudio.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"

void UWidgetSettingsAudio::NativeConstruct() {

	Super::NativeConstruct();

	backButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsAudio::backBehaviour);

}

void UWidgetSettingsAudio::backBehaviour() {

	changeScreen(backID);

}

void UWidgetSettingsAudio::SFXBehaviour() {



}

void UWidgetSettingsAudio::musicBehaviour() {



}

void UWidgetSettingsAudio::setFocusToButton() {

	musicButton->baseButton->SetFocus();

}
