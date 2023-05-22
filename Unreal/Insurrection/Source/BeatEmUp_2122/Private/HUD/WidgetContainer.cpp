// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WidgetContainer.h"
#include "HUD/WidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"

void UWidgetContainer::NativeConstruct() {

	Super::NativeConstruct();

	WidgetSwitcher->ActiveWidgetIndex = 0;

	TArray<UWidget*> children = WidgetSwitcher->GetAllChildren();
	for (UWidget* child : children) {

		UWidgetBase* myParent = Cast<UWidgetBase>(child);

		if (myParent)
		{

			myParent->onChangePage.AddDynamic(this, &UWidgetContainer::changeScreen);

		}

	}

}

void UWidgetContainer::changeScreen(int32 id) {

	WidgetSwitcher->SetActiveWidgetIndex(id);

	UWidgetBase* myParentMenu = Cast<UWidgetBase>(WidgetSwitcher->GetActiveWidget());

	if (myParentMenu)
	{

		myParentMenu->setFocusToButton();

	}

}

void UWidgetContainer::loadLevel(FName levelToOpen)
{

	UGameplayStatics::OpenLevel(this, levelToOpen, false);

}

void UWidgetContainer::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{

	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	UWidgetBase* myParentMenu = Cast<UWidgetBase>(WidgetSwitcher->GetActiveWidget());

	if (myParentMenu)
	{

		myParentMenu->setFocusToButton();

	}

}

void UWidgetContainer::NativeDestruct()
{

	Super::NativeDestruct();

	TArray<UWidget*> children = WidgetSwitcher->GetAllChildren();

	for (UWidget* child : children)
	{

		UWidgetBase* myParent = Cast<UWidgetBase>(child);

		if (myParent->onChangePage.Contains(myParent, TEXT("changeScreen")))
		{

			myParent->onChangePage.Remove(myParent, TEXT("changeScreen"));

		}

	}

}
