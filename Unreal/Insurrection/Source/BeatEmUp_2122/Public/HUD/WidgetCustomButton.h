// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetNavigation.h"
#include "Components/Button.h"
#include "WidgetCustomButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocused, UWidget*, widget);

UCLASS()
class BEATEMUP_2122_API UWidgetCustomButton : public UUserWidget
{
	GENERATED_BODY()

public:

	FOnFocused focusHelper;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* baseButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* buttonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FText buttonTextToDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		bool isFocused = false;

	//NORMAL STATUS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FSlateColor NTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FLinearColor NImageColor;

	//FOCUSED STATUS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FSlateColor FTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FLinearColor FImageColor;

	//PRESSED STATUS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FSlateColor PTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FLinearColor PImageColor;

	//ENABLE STATUS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FSlateColor ETextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FLinearColor EImageColor;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Data")
		FButtonStyle buttonImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons To Move")
		UWidgetCustomButton* up_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons To Move")
		UWidgetCustomButton* down_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons To Move")
		UWidgetCustomButton* left_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons To Move")
		UWidgetCustomButton* right_;

	FSlateFontInfo copyFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Size")
		int textSize_ = 0;

	UFUNCTION(BlueprintCallable)
	UWidget* NextFocus(EUINavigation nextDir);

	UFUNCTION()
	void SetPressColor();

	UFUNCTION()
		void SetReleaseColor();

	FCustomWidgetNavigationDelegate buttonDelegate;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void SynchronizeProperties() override;

	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;

	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

};
