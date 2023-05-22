// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetMainMenu.generated.h"

//class UButton;

UCLASS()
class BEATEMUP_2122_API UWidgetMainMenu : public UWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* playButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* continueButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* selectLevelButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* optionsButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* creditsButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* exitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 playID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 selectLevelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 optionsID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 creditsID;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	

	UFUNCTION()
		void playBehaviour();

	UFUNCTION()
		void continueBehaviour();

	UFUNCTION()
		void selectLevelBehaviour();

	UFUNCTION()
		void optionsBehaviour();

	UFUNCTION()
		void creditsBehaviour();

	UFUNCTION()
		void exitBehaviour();

	virtual void setFocusToButton() override;

protected:

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

};
