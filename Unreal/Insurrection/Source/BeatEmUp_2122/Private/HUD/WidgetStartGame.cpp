#include "HUD/WidgetStartGame.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"

void UWidgetStartGame::NativeConstruct()
{

	Super::NativeConstruct();

	startButton->baseButton->OnClicked.AddDynamic(this, &UWidgetStartGame::startGame);

}

void UWidgetStartGame::startGame()
{

	changeScreen(1);

}

void UWidgetStartGame::setFocusToButton()
{

	startButton->baseButton->SetFocus();

}
