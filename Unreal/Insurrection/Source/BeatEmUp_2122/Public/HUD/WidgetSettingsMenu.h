#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetSettingsMenu.generated.h"

UCLASS()
class BEATEMUP_2122_API UWidgetSettingsMenu : public UWidgetBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* audioButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* videoButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* inputButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* menuButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* backButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 inputID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 audioID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 videoID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Params")
		int32 backID;

	UPROPERTY(EditAnywhere, Category = "EnemyUI")
		bool isInGame;

	UPROPERTY()
		class UGameInstanceBEU* myGI;

	virtual void NativeConstruct() override;

	UFUNCTION()
		void showInput();

	UFUNCTION()
		void loadGameButton();

	UFUNCTION()
		void showAudioSettings();

	UFUNCTION()
		void showVideoSettings();

	UFUNCTION()
		void goBack();

	UFUNCTION()
		void goMenu();

	virtual void setFocusToButton() override;


};
