#include "HUD/WidgetCredits.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"

void UWidgetCredits::NativeConstruct()
{

	Super::NativeConstruct();

	backButton->baseButton->OnClicked.AddDynamic(this, &UWidgetCredits::backBehaviour);

}

void UWidgetCredits::backBehaviour()
{

	changeScreen(idReturnWidget_);

}

void UWidgetCredits::NativeDestruct()
{

	Super::NativeDestruct();

	if (backButton->baseButton->OnClicked.Contains(backButton, TEXT("backBehaviour")))
	{

		backButton->baseButton->OnClicked.Remove(backButton, TEXT("backBehaviour"));

	}

}

void UWidgetCredits::setFocusToButton()
{

	backButton->baseButton->SetFocus();

}
