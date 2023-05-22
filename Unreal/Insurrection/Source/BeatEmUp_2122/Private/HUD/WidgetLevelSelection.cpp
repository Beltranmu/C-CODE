#include "HUD/WidgetLevelSelection.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"
#include "Core/GameInstanceBEU.h"

void UWidgetLevelSelection::NativeConstruct()
{

	Super::NativeConstruct();

	myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

	levelFirst->baseButton->OnClicked.AddDynamic(this, &UWidgetLevelSelection::goLevelOne);
	levelSecond->baseButton->OnClicked.AddDynamic(this, &UWidgetLevelSelection::goLevelTwo);
	levelThird->baseButton->OnClicked.AddDynamic(this, &UWidgetLevelSelection::goLevelThree);
	levelFourth->baseButton->OnClicked.AddDynamic(this, &UWidgetLevelSelection::goLevelFour);
	levelFourthElectricBoogaloo->baseButton->OnClicked.AddDynamic(this, &UWidgetLevelSelection::goLevelFourElectricBoogaloo);
	backButton->baseButton->OnClicked.AddDynamic(this, &UWidgetLevelSelection::goBack);

	levelFirst->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	levelSecond->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	levelThird->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	levelFourth->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	levelFourthElectricBoogaloo->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);
	backButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);

}

void UWidgetLevelSelection::goLevelOne()
{

	UGameplayStatics::OpenLevel(GetWorld(), "Level_01_Fixed");

}

void UWidgetLevelSelection::goLevelTwo()
{

	UGameplayStatics::OpenLevel(GetWorld(), "Level_02_Fixed");

}

void UWidgetLevelSelection::goLevelThree()
{

	UGameplayStatics::OpenLevel(GetWorld(), "Level_03_Fixed");

}

void UWidgetLevelSelection::goLevelFour()
{

	UGameplayStatics::OpenLevel(GetWorld(), "Level_04_Elevator_arena_Fixed");

}

void UWidgetLevelSelection::goLevelFourElectricBoogaloo()
{

	UGameplayStatics::OpenLevel(GetWorld(), "Level_04_Boss_Arena_Fixed");

}

void UWidgetLevelSelection::goBack()
{
	
	levelFirst->baseButton->SetFocus();
	changeScreen(idReturnWidget_);

}

void UWidgetLevelSelection::setFocusToButton()
{

	if (LastFocusedWidget != nullptr)
	{

		LastFocusedWidget->SetFocus();

	}
	else
	{

		levelFirst->baseButton->SetFocus();

	}

}

void UWidgetLevelSelection::NativeDestruct()
{

	if (levelFirst->baseButton->OnClicked.Contains(levelFirst, TEXT("goLevelOne")))
	{

		levelFirst->baseButton->OnClicked.Remove(levelFirst, TEXT("goLevelOne"));

	}
	if (levelFirst->focusHelper.Contains(levelFirst, TEXT("focusWidgetHelper")))
	{

		levelFirst->focusHelper.Remove(levelFirst, TEXT("focusWidgetHelper"));

	}

	if (levelSecond->baseButton->OnClicked.Contains(levelSecond, TEXT("goLevelTwo")))
	{

		levelSecond->baseButton->OnClicked.Remove(levelSecond, TEXT("goLevelTwo"));

	}
	if (levelSecond->focusHelper.Contains(levelSecond, TEXT("focusWidgetHelper")))
	{

		levelSecond->focusHelper.Remove(levelSecond, TEXT("focusWidgetHelper"));

	}

	if (levelThird->baseButton->OnClicked.Contains(levelThird, TEXT("goLevelThree")))
	{

		levelThird->baseButton->OnClicked.Remove(levelThird, TEXT("goLevelThree"));

	}
	if (levelThird->focusHelper.Contains(levelThird, TEXT("focusWidgetHelper")))
	{

		levelThird->focusHelper.Remove(levelThird, TEXT("focusWidgetHelper"));

	}

	if (levelFourth->baseButton->OnClicked.Contains(levelFourth, TEXT("goLevelFour")))
	{

		levelFourth->baseButton->OnClicked.Remove(levelFourth, TEXT("goLevelFour"));

	}
	if (levelFourth->focusHelper.Contains(levelFourth, TEXT("focusWidgetHelper")))
	{

		levelFourth->focusHelper.Remove(levelFourth, TEXT("focusWidgetHelper"));

	}

	if (levelFourthElectricBoogaloo->baseButton->OnClicked.Contains(levelFourthElectricBoogaloo, TEXT("goLevelFourElectricBoogaloo")))
	{

		levelFourthElectricBoogaloo->baseButton->OnClicked.Remove(levelFourthElectricBoogaloo, TEXT("goLevelFourElectricBoogaloo"));

	}
	if (levelFourthElectricBoogaloo->focusHelper.Contains(levelFourthElectricBoogaloo, TEXT("focusWidgetHelper")))
	{

		levelFourthElectricBoogaloo->focusHelper.Remove(levelFourthElectricBoogaloo, TEXT("focusWidgetHelper"));

	}

	if (backButton->baseButton->OnClicked.Contains(backButton, TEXT("goBack")))
	{

		backButton->baseButton->OnClicked.Remove(backButton, TEXT("goBack"));

	}
	if (backButton->focusHelper.Contains(backButton, TEXT("focusWidgetHelper")))
	{

		backButton->focusHelper.Remove(backButton, TEXT("focusWidgetHelper"));

	}

}
