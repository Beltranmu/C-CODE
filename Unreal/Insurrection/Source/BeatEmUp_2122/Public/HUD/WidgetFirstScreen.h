#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetFirstScreen.generated.h"

UCLASS()
class BEATEMUP_2122_API UWidgetFirstScreen : public UWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backgroundImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* startButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Screen ID")
		int screenID_;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Speed")
		float speedWave_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start Button Flash")
		float timeFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotSpeed")
		float rot;


	FTimerHandle timerFlash;

	bool showButton = true;
	
	UFUNCTION()
		void GoToGame();

	UFUNCTION()
		void StartFlash();

	UFUNCTION()
		void BeginFlash();

	UFUNCTION()
		void MoveButtton(float time);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void setFocusToButton() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:


};
