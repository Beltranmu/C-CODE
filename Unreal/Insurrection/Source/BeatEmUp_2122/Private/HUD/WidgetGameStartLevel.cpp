// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WidgetGameStartLevel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"

void UWidgetGameStartLevel::NativeConstruct()
{

	Super::NativeConstruct();

	//GetWorld()->GetTimerManager().SetTimer(startFadeWidgetTimer, this, &ADialogStartController::EndStartDialog, waitTime * slowValue, false);
	levelText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, alphaText_));
	levelBG->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f, alphaImage_));

}

void UWidgetGameStartLevel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

	Super::NativeTick(MyGeometry, InDeltaTime);

	levelText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, alphaText_));
	levelBG->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f, alphaImage_));

}