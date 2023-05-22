#include "HUD/WidgetGameOver.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstanceBEU.h"
#include "MediaTexture.h"
#include "MediaPlayer.h"
#include "Framework/Application/NavigationConfig.h"
#include "Framework/Application/SlateApplication.h"
#include "GameMapsSettings.h"


void UWidgetGameOver::NativeConstruct()
{

	Super::NativeConstruct();

	myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));
	RestartButton->baseButton->OnClicked.AddDynamic(this, &UWidgetGameOver::restartLevel);
	GoMenuButton->baseButton->OnClicked.AddDynamic(this, &UWidgetGameOver::goMenu);

}

void UWidgetGameOver::setFocusToButton()
{

	RestartButton->baseButton->SetFocus();

	if (!alreadyActive)
	{

		TextureToDisplay->SetMediaPlayer(TestPlayerVideo);
		TestPlayerVideo->OpenSource(VideoToPlay);
		TestPlayerVideo->Play();

		alreadyActive = true;

	}

}


void UWidgetGameOver::goMenu()
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

void UWidgetGameOver::restartLevel()
{

	myGI->playerLife[0] = 3;
	myGI->playerLife[1] = 3;
	UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetName()));

}