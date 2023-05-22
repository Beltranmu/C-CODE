// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetGameOver.generated.h"

UCLASS()
class BEATEMUP_2122_API UWidgetGameOver : public UWidgetBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* GameOverBackground;

	UPROPERTY()
		class UGameInstanceBEU* myGI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* RestartButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* GoMenuButton;

	UPROPERTY(EditAnywhere, Category = "Video")
		class UMediaPlayer* TestPlayerVideo;

	UPROPERTY(EditAnywhere, Category = "Video")
		class UMediaSource* VideoToPlay;

	UPROPERTY(EditAnywhere, Category = "Video")
		class UMediaTexture* TextureToDisplay;

	UPROPERTY(EditAnywhere, Category = "Video Control")
		bool alreadyActive = false;

	virtual void NativeConstruct() override;

	virtual void setFocusToButton() override;
	
	UFUNCTION()
		void goMenu();

	UFUNCTION()
		void restartLevel();

};
