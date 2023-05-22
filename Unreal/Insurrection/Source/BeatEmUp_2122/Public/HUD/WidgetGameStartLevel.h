// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetGameStartLevel.generated.h"

/**
 *
 */
UCLASS()
class BEATEMUP_2122_API UWidgetGameStartLevel : public UWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* levelText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* levelBG;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Properties")
		float alphaImage_;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Properties")
		float alphaText_;

	FTimerHandle startFadeWidgetTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start Dialog Control")
		float waitTime = 1.0f;

	virtual void NativeConstruct() override;

protected:

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
