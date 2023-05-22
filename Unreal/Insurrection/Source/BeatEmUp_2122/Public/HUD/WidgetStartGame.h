#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetStartGame.generated.h"

UCLASS()
class BEATEMUP_2122_API UWidgetStartGame : public UWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWidgetCustomButton* startButton;

	
	UFUNCTION()
		void startGame();

	virtual void NativeConstruct() override;
	
	virtual void setFocusToButton() override;
	
};
