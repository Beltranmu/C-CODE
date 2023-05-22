#include "HUD/WidgetPlayerSelection.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "EngineSettings/Classes/GameMapsSettings.h"
#include "Core/GameInstanceBEU.h"
#include "Core/MenuGameModeBEU.h"

void UWidgetPlayerSelection::NativeConstruct()
{

	Super::NativeConstruct();

	myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

	onePlayer->baseButton->OnClicked.AddDynamic(this, &UWidgetPlayerSelection::onePlayerBehaviour);
	twoPlayers->baseButton->OnClicked.AddDynamic(this, &UWidgetPlayerSelection::twoPlayersBehaviour);
	oneController->baseButton->OnClicked.AddDynamic(this, &UWidgetPlayerSelection::oneControllerBehaviour);
	twoController->baseButton->OnClicked.AddDynamic(this, &UWidgetPlayerSelection::twoControllerBehaviour);
	startButton->baseButton->OnClicked.AddDynamic(this, &UWidgetPlayerSelection::startBehaviour);
	backButton->baseButton->OnClicked.AddDynamic(this, &UWidgetPlayerSelection::backButtonBehaviour);

	onePlayer->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	twoPlayers->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	oneController->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	twoController->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	startButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	backButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);

}

void UWidgetPlayerSelection::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

	Super::NativeTick(MyGeometry, InDeltaTime);

	if (startButton->isFocused)
	{

		backButton->up_ = startButton;
		backButton->SynchronizeProperties();
		
	}
	else if (onePlayer->isFocused)
	{

		backButton->up_ = onePlayer;
		backButton->SynchronizeProperties();

	}
	else if (twoPlayers->isFocused)
	{

		backButton->up_ = twoPlayers;
		backButton->SynchronizeProperties();

	}
	else if (oneController->isFocused)
	{

		backButton->up_ = oneController;
		backButton->SynchronizeProperties();

	}
	else if (twoController->isFocused)
	{

		backButton->up_ = twoController;
		backButton->SynchronizeProperties();

	}

	if (onePlayer->isFocused)
	{

		p1->SetVisibility(ESlateVisibility::Visible);
		p2->SetVisibility(ESlateVisibility::Hidden);
		p1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.65f));
		p2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

	}
	else if (twoPlayers->isFocused)
	{

		p1->SetVisibility(ESlateVisibility::Visible);
		p2->SetVisibility(ESlateVisibility::Visible);
		p1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.65f));
		p2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.65f));

	}

}

void UWidgetPlayerSelection::startBehaviour()
{

	UGameplayStatics::OpenLevel(GetWorld(), "Level_01_Fixed");

}

void UWidgetPlayerSelection::onePlayerBehaviour()
{

	myGI->numPlayers = 1;

	AMenuGameModeBEU* gameMode = Cast<AMenuGameModeBEU>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->controllers = false;

	UGameMapsSettings* Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());

	if (IsValid(Settings))
	{

		Settings->bOffsetPlayerGamepadIds = gameMode->controllers;

	}

	onePlayer->SetVisibility(ESlateVisibility::HitTestInvisible);
	twoPlayers->SetVisibility(ESlateVisibility::Hidden);

	oneController->SetVisibility(ESlateVisibility::Hidden);
	twoController->SetVisibility(ESlateVisibility::Hidden);
	p1->SetVisibility(ESlateVisibility::Visible);
	p2->SetVisibility(ESlateVisibility::Hidden);
	p1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	p2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

	startButton->SetVisibility(ESlateVisibility::Visible);
	startButton->baseButton->SetFocus();

	if (myGI->thisBooleanExistsToPreventSpawningMoreThan2PlayersBoogaloo)
	{

		UGameplayStatics::RemovePlayer(UGameplayStatics::GetPlayerController(GetWorld(), 1), true);
		myGI->thisBooleanExistsToPreventSpawningMoreThan2PlayersBoogaloo = false;

	}

}

void UWidgetPlayerSelection::twoPlayersBehaviour()
{

	myGI->numPlayers = 2;

	onePlayer->SetVisibility(ESlateVisibility::Hidden);
	twoPlayers->SetVisibility(ESlateVisibility::HitTestInvisible);

	p1->SetVisibility(ESlateVisibility::Visible);
	p2->SetVisibility(ESlateVisibility::Visible);

	oneController->SetVisibility(ESlateVisibility::Visible);
	twoController->SetVisibility(ESlateVisibility::Visible);

	p1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	p2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	oneController->baseButton->SetFocus();

}

void UWidgetPlayerSelection::oneControllerBehaviour()
{

	AMenuGameModeBEU* gameMode = Cast<AMenuGameModeBEU>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->controllers = true;

	UGameMapsSettings* Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());

	if (IsValid(Settings))
	{

		Settings->bOffsetPlayerGamepadIds = gameMode->controllers;

	}

	oneController->SetVisibility(ESlateVisibility::HitTestInvisible);
	twoController->SetVisibility(ESlateVisibility::Hidden);

	startButton->SetVisibility(ESlateVisibility::Visible);
	startButton->baseButton->SetFocus();

}

void UWidgetPlayerSelection::twoControllerBehaviour()
{


	AMenuGameModeBEU* gameMode = Cast<AMenuGameModeBEU>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->controllers = false;

	UGameMapsSettings* Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());

	if (IsValid(Settings))
	{

		Settings->bOffsetPlayerGamepadIds = gameMode->controllers;

	}

	oneController->SetVisibility(ESlateVisibility::Hidden);
	twoController->SetVisibility(ESlateVisibility::HitTestInvisible);

	startButton->SetVisibility(ESlateVisibility::Visible);
	startButton->baseButton->SetFocus();

}

void UWidgetPlayerSelection::backButtonBehaviour()
{
	
	onePlayer->SetVisibility(ESlateVisibility::Visible);
	twoPlayers->SetVisibility(ESlateVisibility::Visible);

	oneController->SetVisibility(ESlateVisibility::Hidden);
	twoController->SetVisibility(ESlateVisibility::Hidden);

	startButton->SetVisibility(ESlateVisibility::Hidden);

	onePlayer->baseButton->SetFocus();

	changeScreen(idReturnWidget_);

}

void UWidgetPlayerSelection::setFocusToButton()
{

	if (LastFocusedWidget != nullptr)
	{

		LastFocusedWidget->SetFocus();

	}
	else
	{

		onePlayer->baseButton->SetFocus();

	}

}

FReply UWidgetPlayerSelection::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{

	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	const EUINavigationAction NavAction = FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent);

	if (NavAction == EUINavigationAction::Back && (onePlayer->isFocused || twoPlayers->isFocused))
	{
	
		backButton->baseButton->SetFocus();
	
	}
	else if (NavAction == EUINavigationAction::Back && startButton->isFocused && myGI->numPlayers == 1)
	{
		
		onePlayer->SetVisibility(ESlateVisibility::Visible);
		twoPlayers->SetVisibility(ESlateVisibility::Visible);
		startButton->SetVisibility(ESlateVisibility::Hidden);
		onePlayer->baseButton->SetFocus();
	
	}
	else if (NavAction == EUINavigationAction::Back && (oneController->isFocused || twoController->isFocused))
	{
	
		onePlayer->SetVisibility(ESlateVisibility::Visible);
		twoPlayers->SetVisibility(ESlateVisibility::Visible);
	
		oneController->SetVisibility(ESlateVisibility::Hidden);
		twoController->SetVisibility(ESlateVisibility::Hidden);
	
		twoPlayers->baseButton->SetFocus();
	
	}
	else if(NavAction == EUINavigationAction::Back && startButton->isFocused && myGI->numPlayers == 2)
	{
		
		oneController->SetVisibility(ESlateVisibility::Visible);
		twoController->SetVisibility(ESlateVisibility::Visible);
		startButton->SetVisibility(ESlateVisibility::Hidden);
		oneController->baseButton->SetFocus();
	
	}

	return reply;

}

void UWidgetPlayerSelection::NativeDestruct()
{

	if (startButton->baseButton->OnClicked.Contains(startButton, TEXT("startBehaviour")))
	{

		startButton->baseButton->OnClicked.Remove(startButton, TEXT("startBehaviour"));

	}
	if (startButton->focusHelper.Contains(startButton, TEXT("focusWidgetHelper")))
	{

		startButton->focusHelper.Remove(startButton, TEXT("focusWidgetHelper"));

	}

	if (backButton->baseButton->OnClicked.Contains(backButton, TEXT("backButtonBehaviour")))
	{

		backButton->baseButton->OnClicked.Remove(backButton, TEXT("backButtonBehaviour"));

	}
	if (backButton->focusHelper.Contains(backButton, TEXT("focusWidgetHelper")))
	{

		backButton->focusHelper.Remove(backButton, TEXT("focusWidgetHelper"));

	}

	if (onePlayer->baseButton->OnClicked.Contains(onePlayer, TEXT("onePlayerBehaviour")))
	{

		onePlayer->baseButton->OnClicked.Remove(onePlayer, TEXT("onePlayerBehaviour"));

	}
	if (onePlayer->focusHelper.Contains(onePlayer, TEXT("focusWidgetHelper")))
	{

		onePlayer->focusHelper.Remove(onePlayer, TEXT("focusWidgetHelper"));

	}

	if (twoPlayers->baseButton->OnClicked.Contains(twoPlayers, TEXT("twoPlayersBehaviour")))
	{

		twoPlayers->baseButton->OnClicked.Remove(twoPlayers, TEXT("twoPlayersBehaviour"));

	}
	if (twoPlayers->focusHelper.Contains(twoPlayers, TEXT("focusWidgetHelper")))
	{

		twoPlayers->focusHelper.Remove(twoPlayers, TEXT("focusWidgetHelper"));

	}

	if (oneController->baseButton->OnClicked.Contains(oneController, TEXT("oneControllerBehaviour")))
	{

		oneController->baseButton->OnClicked.Remove(oneController, TEXT("oneControllerBehaviour"));

	}
	if (oneController->focusHelper.Contains(oneController, TEXT("focusWidgetHelper")))
	{

		oneController->focusHelper.Remove(oneController, TEXT("focusWidgetHelper"));

	}

	if (twoController->baseButton->OnClicked.Contains(twoController, TEXT("twoControllerBehaviour")))
	{

		twoController->baseButton->OnClicked.Remove(twoController, TEXT("twoControllerBehaviour"));

	}
	if (twoController->focusHelper.Contains(twoController, TEXT("focusWidgetHelper")))
	{

		twoController->focusHelper.Remove(twoController, TEXT("focusWidgetHelper"));

	}

}