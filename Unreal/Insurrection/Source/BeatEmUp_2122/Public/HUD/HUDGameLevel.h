// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDGameLevel.generated.h"

class UWidgetContainer;

UCLASS()
class BEATEMUP_2122_API AHUDGameLevel : public AHUD
{
	GENERATED_BODY()

public:

	AHUDGameLevel();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WidgetSwitch")
		TSubclassOf<UWidgetContainer> widgetSelector;

	UPROPERTY(BlueprintReadWrite, Category = "WidgetOption")
		UWidgetContainer* widgetContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy IMG")
		TArray<UTexture2D*> enemyImgArray;

	UPROPERTY(EditAnywhere, Category = "EnemyUI|Timers")
		FTimerHandle timer_enemyUI[2];

	FTimerDelegate delegate_timer_enemyUI[2];

	UPROPERTY(EditAnywhere, Category = "Time Control")
		float slowTime;

	UPROPERTY(BlueprintReadWrite, Category = "WidgetOption")
		APlayerController* playerOne;

	UPROPERTY(BlueprintReadWrite, Category = "WidgetOption")
		APlayerController* playerTwo;

	UPROPERTY(EditAnywhere, Category = "EnemyUI")
		int32 idEnemyHit[2];

	UPROPERTY()
		class UGameInstanceBEU* gameInstance;

	void BeginPlay() override;

	void EndPlay(EEndPlayReason::Type EndReason) override;

	UFUNCTION()
		void UpdateEnemiesBar(float percentageHP, int enemyType, int32 idEnemy, int idCharacter);

	UFUNCTION()
		void ShowEnemyBar(int id, float percentage, int id_char);

	UFUNCTION()
		void ResetsEnemyUI(int player_id);

	UFUNCTION()
		void DialogBehaviour();

	UFUNCTION()
		void StartLevelWidget();

	UFUNCTION()
		void DialogEnd();

	UFUNCTION()
		void PauseGame();

	UFUNCTION()
		void ShowDeadMenu();

	//EVENTS TO UPDATE PLAYER/S UI

	UFUNCTION()
		void UpdatePlayerHPBar(int32 playerID, float currentHP, float maxHP);

	UFUNCTION()
		void UpdatePlayerSpecialChargesBar(int32 playerID, float currentSpecialCharge, float maxSpecialCharge);

	UFUNCTION()
		void UpdatePlayerStaminaBar(int32 playerID, float currentStamina, float maxStamina);

	UFUNCTION()
		void UpdatePlayerLifes(int32 playerID, int32 lifes);

	UFUNCTION()
		void UpdatePlayerPoints(int32 playerID, int32 points);

	UFUNCTION()
		void UpdatePlayerDashes(int32 playerID, int32 dashCharges);

	UFUNCTION()
		void UpdateBossBar(float currentHP, float maxHP);

	UFUNCTION()
		void HideAllUI();

	UFUNCTION()
		void ShowGoHint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Toggle")
		bool isAllHidden;

};
