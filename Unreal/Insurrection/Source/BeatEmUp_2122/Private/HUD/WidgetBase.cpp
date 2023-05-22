// Fill out your copyright notice in the Description page of Project Settings.

//IMPORTANT: WIDGETBASE, WIDGETCUSTOMBUTTON, WIDGET MAIN MENU AND WIDGETCONTAINER

#include "HUD/WidgetBase.h"
#include "Input/Reply.h"


void UWidgetBase::changeScreen(int32 id)
{

	onChangePage.Broadcast(id);

}

void UWidgetBase::focusWidgetHelper(UWidget* widgetF)
{

	LastFocusedWidget = widgetF;

}

void UWidgetBase::NativeDestruct()
{

	Super::NativeDestruct();

	onChangePage.Clear();

}

void UWidgetBase::setFocusToButton()
{


}