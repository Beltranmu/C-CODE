// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Player/TurboPlayer.h"
#include "WidgetContainer.generated.h"

/**
 *
 */
UCLASS()
class BEATEMUP_2122_API UWidgetContainer : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetGameUI* BP_WidgetGameUI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetGameDialog* BP_WidgetGameDialog;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetSettingsInput* BP_WidgetSettingsInput;

	UFUNCTION()
		void changeScreen(int32 id);

	UFUNCTION()
		void loadLevel(FName levelToOpen);

	//TWeakObjectPtr<ATurboPlayer> OwnerPlayer;

	//void SetOwner(ATurboPlayer* MainPlayer) { OwnerPlayer = MainPlayer; };

	virtual void NativeDestruct() override;

	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

};
