#include "HUD/WidgetSettingsMenu.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"
#include "Core/GameInstanceBEU.h"
#include "Kismet/GameplayStatics.h"
#include "EngineSettings/Classes/GameMapsSettings.h"

void UWidgetSettingsMenu::NativeConstruct()
{

	Super::NativeConstruct();

	myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

	loadGameButton();

	backButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsMenu::goBack);
	menuButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsMenu::goMenu);
	//inputButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsMenu::showInput);
	//videoButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsMenu::showVideoSettings);
	//audioButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsMenu::showAudioSettings);

	//inputButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsMenu::goBack);
	//videoButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsMenu::goBack);
	//audioButton->baseButton->OnClicked.AddDynamic(this, &UWidgetSettingsMenu::goBack);

}

void UWidgetSettingsMenu::showInput()
{

	changeScreen(inputID);

}

void UWidgetSettingsMenu::showAudioSettings()
{

	changeScreen(audioID);

}

void UWidgetSettingsMenu::showVideoSettings()
{

	changeScreen(videoID);

}

void UWidgetSettingsMenu::loadGameButton()
{

	if (isInGame)
	{

		menuButton->baseButton->SetVisibility(ESlateVisibility::Visible);

		backButton->right_ = menuButton;
		backButton->left_ = menuButton;

	}
	else
	{

		menuButton->baseButton->SetVisibility(ESlateVisibility::Hidden);
		//CAMBIAR EN UN FUTURO ESTO

	}

}

void UWidgetSettingsMenu::goMenu()
{

	UGameMapsSettings* Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());

	if (IsValid(Settings))
	{

		Settings->bOffsetPlayerGamepadIds = true;

	}
	myGI->numPlayers = 1;
	myGI->thisBooleanExistsToPreventSpawningMoreThan2PlayersBoogaloo = false;
	UGameplayStatics::RemovePlayer(UGameplayStatics::GetPlayerController(GetWorld(), 1), true);
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");

}

void UWidgetSettingsMenu::goBack()
{

	if (isInGame)
	{

		APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		player0->SetPause(false);

		FInputModeGameOnly auxInputHelper = FInputModeGameOnly();
		player0->SetInputMode(auxInputHelper);

	}

	changeScreen(backID);

}

void UWidgetSettingsMenu::setFocusToButton()
{

	//audioButton->baseButton->SetFocus();
	backButton->baseButton->SetFocus();

}