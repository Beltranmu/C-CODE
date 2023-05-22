#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetBase.h"
#include "WidgetGameUI.generated.h"

class AFinalBossCharacter;

UCLASS()
class BEATEMUP_2122_API UWidgetGameUI : public UWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Boss Config | Enemies")
		TSubclassOf<AFinalBossCharacter> finalBossClass;

	AFinalBossCharacter* finalBoss;

	//PLAYER ONE
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* playerOneHealth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* playerOneStamina;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* playerOneImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* playerOneSpecialCharges;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* playerOneSpecialChargeImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backScore;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backHelath;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backStamina;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* staminaText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* playerOneLifes;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* playerOneScore;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* playerOneDashes;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))

	//ENEMY PLAYER ONE
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* enemyOneHealth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* enemyOneImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backEnemyHP;

	//PLAYER TWO
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* playerTwoHealth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* playerTwoStamina;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* playerTwoImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* playerTwoSpecialCharges;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* playerTwoSpecialChargeImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backScore_1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backHelath_1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backStamina_1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* staminaText_1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* playerTwoLifes;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* playerTwoScore;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* playerTwoDashes;

	//ENEMY PLAYER TWO
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* enemyTwoHealth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* enemyTwoImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* backEnemyHP_1;

	//TEXT
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* controlsHintText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* dashPlayerOneText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* dashPlayerTwoText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* goImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* fingerImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* bossImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* bossBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Times to switch hints")
		int hintCounter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Times to switch hints")
		int maxHints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave Sign")
		float timeContinue;
	
	FTimerHandle timerContinue;

	bool imageId;

	UFUNCTION()
		void FindBoss();

	UFUNCTION()
		void BeginContinueSign();

	UFUNCTION()
		void ContinueLevelSign();

	virtual void NativeConstruct() override;

	UFUNCTION()
		void SetVisibilityPlayersUI(int id, bool show);

	UFUNCTION()
		void SetUIForEnemy(UTexture2D* tex, int id, float percentage);

	UFUNCTION()
		void SetVisibilityEnemyUI(int id, bool show);

	//Events For update the players UI
	UFUNCTION()
		void SetPlayerHP(int32 playerID, float hpValue);

	UFUNCTION()
		void SetPlayerSpecialCharge(int32 playerID, float specialChargeValue);

	UFUNCTION()
		void SetPlayerStamina(int32 playerID, float staminaValue);

	UFUNCTION()
		void SetPlayerLifes(int32 playerID, int32 lifes);

	UFUNCTION()
		void SetPlayerPoints(int32 playerID, int32 points);

	UFUNCTION()
		void SetPlayerDashes(int32 playerID, int32 dashCharges);

	UFUNCTION()
		void SetBossHP(float hpValue);

};
