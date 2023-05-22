#include "HUD/WidgetFirstScreen.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "HUD/WidgetCustomButton.h"

void UWidgetFirstScreen::NativeConstruct()
{

	Super::NativeConstruct();

	startButton->baseButton->OnClicked.AddDynamic(this, &UWidgetFirstScreen::GoToGame);
	startButton->focusHelper.AddDynamic(this, &UWidgetBase::focusWidgetHelper);

	//BeginFlash();

}

void UWidgetFirstScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

	Super::NativeTick(MyGeometry, InDeltaTime);

	MoveButtton(InDeltaTime);

}

void UWidgetFirstScreen::MoveButtton(float time)
{

	auto copyData = startButton->Slot;

	rot = rot + time * 10.0f;

	copyData->Content->SetRenderTranslation(FVector2D(0.0f, cosf(rot) * speedWave_));

	startButton->Slot = copyData;

}

void UWidgetFirstScreen::BeginFlash()
{

	//GetWorld()->GetTimerManager().SetTimer(timerFlash, this, &UWidgetFirstScreen::StartFlash, timeFlash, true);

}

void UWidgetFirstScreen::StartFlash()
{

	showButton = !showButton;

	if (showButton)
	{
		
		startButton->SetVisibility(ESlateVisibility::Visible);

	}
	else if (!showButton)
	{
		
		startButton->SetVisibility(ESlateVisibility::Hidden);

	}

}

void UWidgetFirstScreen::setFocusToButton()
{

	if (LastFocusedWidget != nullptr)
	{

		LastFocusedWidget->SetFocus();

	}
	else
	{

		startButton->baseButton->SetFocus();

	}

}

void UWidgetFirstScreen::GoToGame()
{

	changeScreen(screenID_);

}

void UWidgetFirstScreen::NativeDestruct()
{

	if (startButton->baseButton->OnClicked.Contains(startButton, TEXT("GoToGame")))
	{

		startButton->baseButton->OnClicked.Remove(startButton, TEXT("GoToGame"));

	}
	if (startButton->focusHelper.Contains(startButton, TEXT("focusWidgetHelper")))
	{

		startButton->focusHelper.Remove(startButton, TEXT("focusWidgetHelper"));

	}

}