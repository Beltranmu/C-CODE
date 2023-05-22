// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetCredits.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UWidgetCredits : public UWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* bgCredits;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* backButton;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* savi;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* suuuu;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* antequera;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* reee;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* disenio;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* art1;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* art2;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* art3;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* art4;
	//
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UWidgetCustomButton* art5;

	UFUNCTION()
		void backBehaviour();

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void setFocusToButton() override;

};
