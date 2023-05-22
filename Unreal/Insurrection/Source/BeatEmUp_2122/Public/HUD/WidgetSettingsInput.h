// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetSettingsInput.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UWidgetSettingsInput : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* InputImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* backButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 backID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Place")
		bool isInMenu;

	UFUNCTION()
		void backBehaviour();

		virtual void NativeConstruct() override;

		virtual void setFocusToButton() override;

};
