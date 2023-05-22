#include "HUD/WidgetCustomButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWidgetCustomButton::NativeConstruct()
{

	Super::NativeConstruct();

	baseButton->SetStyle(buttonImages);

	baseButton->OnHovered.AddDynamic(baseButton, &UButton::SetFocus);
	baseButton->OnPressed.AddDynamic(this, &UWidgetCustomButton::SetPressColor);
	baseButton->OnReleased.AddDynamic(this, &UWidgetCustomButton::SetReleaseColor);

}


void UWidgetCustomButton::NativeDestruct()
{

	if (baseButton->OnHovered.Contains(baseButton, TEXT("SetFocus")))
	{

		baseButton->OnHovered.Remove(baseButton, TEXT("SetFocus"));

	}

	if (baseButton->OnPressed.Contains(baseButton, TEXT("SetPressColor")))
	{

		baseButton->OnPressed.Remove(baseButton, TEXT("SetPressColor"));

	}

	if (baseButton->OnReleased.Contains(baseButton, TEXT("SetReleaseColor")))
	{

		baseButton->OnReleased.Remove(baseButton, TEXT("SetReleaseColor"));

	}

}

UWidget* UWidgetCustomButton::NextFocus(EUINavigation nextDir)
{

	switch (nextDir)
	{
	case EUINavigation::Left:
		if (left_)
		{

			return left_->baseButton;

		}
		break;
	case EUINavigation::Right:
		if (right_)
		{

			return right_->baseButton;
			
		}
		break;
	case EUINavigation::Up:
		if (up_)
		{

			return up_->baseButton;

		}
		break;
	case EUINavigation::Down:
		if (down_)
		{

			return down_->baseButton;

		}
		break;
	case EUINavigation::Next:
		break;
	case EUINavigation::Previous:
		break;
	case EUINavigation::Num:
		break;
	case EUINavigation::Invalid:
		break;
	default:
		break;
	}
	return baseButton;
}

void UWidgetCustomButton::SynchronizeProperties()
{

	Super::SynchronizeProperties();

	copyFont = buttonText->Font;
	copyFont.Size = textSize_;
	buttonText->SetFont(copyFont);

	if (baseButton && buttonText)
	{

		buttonText->SetText(buttonTextToDisplay);
		buttonText->SetColorAndOpacity(NTextColor);

		baseButton->SetBackgroundColor(NImageColor);

		if (up_)
		{
			
			buttonDelegate.BindUFunction(this, TEXT("NextFocus"));
			baseButton->SetNavigationRuleCustom(EUINavigation::Up, buttonDelegate);

		}
		else {

			baseButton->SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);

		}

		if (down_)
		{

			buttonDelegate.BindUFunction(this, TEXT("NextFocus"));
			baseButton->SetNavigationRuleCustom(EUINavigation::Down, buttonDelegate);

		}
		else {

			baseButton->SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Stop);

		}

		if (left_)
		{

			buttonDelegate.BindUFunction(this, TEXT("NextFocus"));
			baseButton->SetNavigationRuleCustom(EUINavigation::Left, buttonDelegate);

		}
		else {

			baseButton->SetNavigationRuleBase(EUINavigation::Left, EUINavigationRule::Stop);

		}

		if (right_)
		{

			buttonDelegate.BindUFunction(this, TEXT("NextFocus"));
			baseButton->SetNavigationRuleCustom(EUINavigation::Right, buttonDelegate);

		}
		else {

			baseButton->SetNavigationRuleBase(EUINavigation::Right, EUINavigationRule::Stop);

		}

	}

}

void UWidgetCustomButton::SetPressColor()
{

	buttonText->SetColorAndOpacity(PTextColor);

	baseButton->SetBackgroundColor(PImageColor);

}

void UWidgetCustomButton::SetReleaseColor()
{
	
	if (baseButton->HasAnyUserFocus())
	{

		buttonText->SetColorAndOpacity(FTextColor);

		baseButton->SetBackgroundColor(FImageColor);

	}
	else
	{
	
		buttonText->SetColorAndOpacity(NTextColor);

		baseButton->SetBackgroundColor(NImageColor);
	
	}

}

void UWidgetCustomButton::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{

	Super::NativeOnAddedToFocusPath(InFocusEvent);

	isFocused = true;

	focusHelper.Broadcast(baseButton);

	baseButton->WidgetStyle.Normal = buttonImages.Hovered;

	buttonText->SetColorAndOpacity(FTextColor);

	baseButton->SetBackgroundColor(FImageColor);

}

void UWidgetCustomButton::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{

	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	isFocused = false;

	baseButton->WidgetStyle.Normal = buttonImages.Normal;

	buttonText->SetColorAndOpacity(NTextColor);

	baseButton->SetBackgroundColor(NImageColor);

}