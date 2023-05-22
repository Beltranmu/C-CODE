#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "Components/CanvasPanel.h"
#include "WidgetPlayerSelection.generated.h"

UCLASS()
class BEATEMUP_2122_API UWidgetPlayerSelection : public UWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* startButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* onePlayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* twoPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* oneController;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* twoController;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* backButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* p1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* p2;

	UPROPERTY()
		class UGameInstanceBEU* myGI;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
		void startBehaviour();

	UFUNCTION()
		void onePlayerBehaviour();

	UFUNCTION()
		void twoPlayersBehaviour();

	UFUNCTION()
		void oneControllerBehaviour();

	UFUNCTION()
		void twoControllerBehaviour();

	UFUNCTION()
		void backButtonBehaviour();

	virtual void setFocusToButton() override;

protected:

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

};
