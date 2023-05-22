#include "HUD/WidgetMainMenu.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"

void UWidgetMainMenu::NativeConstruct()
{

	Super::NativeConstruct();

	playButton->baseButton->OnClicked.AddDynamic(this, &UWidgetMainMenu::playBehaviour);
	continueButton->baseButton->OnClicked.AddDynamic(this, &UWidgetMainMenu::continueBehaviour);
	selectLevelButton->baseButton->OnClicked.AddDynamic(this, &UWidgetMainMenu::selectLevelBehaviour);
	optionsButton->baseButton->OnClicked.AddDynamic(this, &UWidgetMainMenu::optionsBehaviour);
	creditsButton->baseButton->OnClicked.AddDynamic(this, &UWidgetMainMenu::creditsBehaviour);
	exitButton->baseButton->OnClicked.AddDynamic(this, &UWidgetMainMenu::exitBehaviour);

	playButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	continueButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	selectLevelButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	optionsButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	creditsButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	exitButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);

}

void UWidgetMainMenu::NativeDestruct()
{

	Super::NativeDestruct();

	if (playButton->baseButton->OnClicked.Contains(playButton, TEXT("playBehaviour")))
	{

		playButton->baseButton->OnClicked.Remove(playButton, TEXT("playBehaviour"));

	}
	if (playButton->focusHelper.Contains(playButton, TEXT("focusWidgetHelper")))
	{

		playButton->focusHelper.Remove(playButton, TEXT("focusWidgetHelper"));

	}

	if (continueButton->baseButton->OnClicked.Contains(continueButton, TEXT("continueBehaviour")))
	{

		continueButton->baseButton->OnClicked.Remove(continueButton, TEXT("continueBehaviour"));

	}
	if (continueButton->focusHelper.Contains(continueButton, TEXT("focusWidgetHelper")))
	{

		continueButton->focusHelper.Remove(continueButton, TEXT("focusWidgetHelper"));

	}

	if (selectLevelButton->baseButton->OnClicked.Contains(selectLevelButton, TEXT("selectLevelBehaviour")))
	{

		selectLevelButton->baseButton->OnClicked.Remove(selectLevelButton, TEXT("selectLevelBehaviour"));

	}
	if (selectLevelButton->focusHelper.Contains(selectLevelButton, TEXT("focusWidgetHelper")))
	{

		selectLevelButton->focusHelper.Remove(selectLevelButton, TEXT("focusWidgetHelper"));

	}

	if (optionsButton->baseButton->OnClicked.Contains(optionsButton, TEXT("optionsBehaviour")))
	{

		optionsButton->baseButton->OnClicked.Remove(optionsButton, TEXT("optionsBehaviour"));

	}
	if (optionsButton->focusHelper.Contains(optionsButton, TEXT("focusWidgetHelper")))
	{

		optionsButton->focusHelper.Remove(optionsButton, TEXT("focusWidgetHelper"));

	}

	if (creditsButton->baseButton->OnClicked.Contains(creditsButton, TEXT("creditsBehaviour")))
	{

		creditsButton->baseButton->OnClicked.Remove(creditsButton, TEXT("creditsBehaviour"));

	}
	if (creditsButton->focusHelper.Contains(creditsButton, TEXT("focusWidgetHelper")))
	{

		creditsButton->focusHelper.Remove(creditsButton, TEXT("focusWidgetHelper"));

	}

	if (exitButton->baseButton->OnClicked.Contains(exitButton, TEXT("exitBehaviour")))
	{

		exitButton->baseButton->OnClicked.Remove(exitButton, TEXT("exitBehaviour"));

	}
	if (exitButton->focusHelper.Contains(exitButton, TEXT("focusWidgetHelper")))
	{

		exitButton->focusHelper.Remove(exitButton, TEXT("focusWidgetHelper"));

	}

}

void UWidgetMainMenu::playBehaviour()
{
	
	changeScreen(playID);

}

void UWidgetMainMenu::continueBehaviour()
{

	

}

void UWidgetMainMenu::selectLevelBehaviour()
{

	changeScreen(selectLevelID);

}

void UWidgetMainMenu::optionsBehaviour()
{

	changeScreen(optionsID);

}

void UWidgetMainMenu::creditsBehaviour()
{

	changeScreen(creditsID);

}

void UWidgetMainMenu::exitBehaviour()
{

	FGenericPlatformMisc::RequestExit(false);

}

void UWidgetMainMenu::setFocusToButton()
{

	if (LastFocusedWidget != nullptr)
	{
		
		LastFocusedWidget->SetFocus();

	}
	else
	{
	
		playButton->baseButton->SetFocus();
	
	}

}

FReply UWidgetMainMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{

	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	const EUINavigationAction NavAction = FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent);

	if (NavAction == EUINavigationAction::Back)
	{

		exitButton->baseButton->SetFocus();

	}

	return reply;

}
