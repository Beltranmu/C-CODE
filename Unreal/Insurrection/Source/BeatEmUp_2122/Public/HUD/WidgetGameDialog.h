// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "Player/TurboPlayer.h"
#include "Engine/DataTable.h"
#include "WidgetGameDialog.generated.h"

class ATurboPlayer;

USTRUCT(BlueprintType)
struct FDialogData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FText DisplayDialog;

	UPROPERTY(EditAnywhere)
		UTexture2D* ImgSpeaker1;

	UPROPERTY(EditAnywhere)
		UTexture2D* ImgSpeaker2;

	UPROPERTY(EditAnywhere)
		float colorSpeaker1;

	UPROPERTY(EditAnywhere)
		float colorSpeaker2;

	UPROPERTY(EditAnywhere)
		float sizeMultiplierSpeaker1X;

	UPROPERTY(EditAnywhere)
		float sizeMultiplierSpeaker1Y;

	UPROPERTY(EditAnywhere)
		float sizeMultiplierSpeaker2X;

	UPROPERTY(EditAnywhere)
		float sizeMultiplierSpeaker2Y;

};

UCLASS()
class BEATEMUP_2122_API UWidgetGameDialog : public UWidgetBase
{
	GENERATED_BODY()

public:

	//TWeakObjectPtr<ATurboPlayer> OwnerPlayer;

	//void SetOwner(ATurboPlayer* MyPlayer) { OwnerPlayer = MyPlayer; };

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* dialogToDisplay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* imageLeftDialog;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* imageRightDialog;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* textBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* nextDialog;

	UPROPERTY(EditAnywhere)
		class UDataTable* dialogData;

	UPROPERTY()
		class UGameInstanceBEU* gameInstance;

	virtual void NativeConstruct() override;

	UFUNCTION()
		void triggerNextDialog();

	virtual void setFocusToButton() override;

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
