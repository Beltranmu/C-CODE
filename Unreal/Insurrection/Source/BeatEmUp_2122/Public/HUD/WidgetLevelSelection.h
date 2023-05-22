// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetLevelSelection.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UWidgetLevelSelection : public UWidgetBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* levelFirst;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* levelSecond;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* levelThird;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* levelFourth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* levelFourthElectricBoogaloo;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* backButton;
	
	UPROPERTY()
		class UGameInstanceBEU* myGI;
	
	UFUNCTION()
		void goLevelOne();

	UFUNCTION()
		void goLevelTwo();

	UFUNCTION()
		void goLevelThree();

	UFUNCTION()
		void goLevelFour();

	UFUNCTION()
		void goLevelFourElectricBoogaloo();

	UFUNCTION()
		void goBack();

	virtual void setFocusToButton() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

};
