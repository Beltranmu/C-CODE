// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetSettingsAudio.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UWidgetSettingsAudio : public UWidgetBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* musicButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* SFXButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* ambientButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* backButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 backID;

	virtual void NativeConstruct() override;

	UFUNCTION()
		void musicBehaviour();

	UFUNCTION()
		void SFXBehaviour();

	UFUNCTION()
		void backBehaviour();

	virtual void setFocusToButton() override;

};
