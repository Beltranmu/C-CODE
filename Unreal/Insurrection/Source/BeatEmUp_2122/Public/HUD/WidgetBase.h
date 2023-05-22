// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateVar, int32, myMenuVar);

UCLASS()
class BEATEMUP_2122_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	FDelegateVar onChangePage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Widget")
		int32 idReturnWidget_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Widget")
		class UWidget* LastFocusedWidget;

	UFUNCTION()
		void changeScreen(int32 id);

	UFUNCTION()
		void focusWidgetHelper(UWidget* widgetF);

	virtual void NativeDestruct() override;

	//UFUNCTION()
	//void goBackWidgetMenu();

	virtual void setFocusToButton();

	//virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& FOnKeyEvent) override;

};
