// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/WidgetGameUI.h"
#include "Components/Image.h"
#include <Components/ProgressBar.h>
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Enemies/FinalBossCharacter.h"
#include "Kismet/GameplayStatics.h"

void UWidgetGameUI::NativeConstruct()
{

	Super::NativeConstruct();

	FindBoss();

	imageId = false;

}

void UWidgetGameUI::FindBoss()
{

	finalBoss = Cast<AFinalBossCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), finalBossClass));

	if (finalBoss == nullptr)
	{
		
		bossImage->SetVisibility(ESlateVisibility::Hidden);
		bossBar->SetVisibility(ESlateVisibility::Hidden);

	}
	else
	{
	
		bossImage->SetVisibility(ESlateVisibility::Visible);
		bossBar->SetVisibility(ESlateVisibility::Visible);

	}

}

void UWidgetGameUI::SetVisibilityPlayersUI(int id, bool show)
{

	if (show)
	{

		if (id == 0)
		{

			controlsHintText->SetVisibility(ESlateVisibility::Visible);
			playerOneHealth->SetVisibility(ESlateVisibility::Visible);
			playerOneStamina->SetVisibility(ESlateVisibility::Visible);
			playerOneImage->SetVisibility(ESlateVisibility::Visible);
			playerOneSpecialCharges->SetVisibility(ESlateVisibility::Visible);
			playerOneSpecialChargeImg->SetVisibility(ESlateVisibility::Visible);
			staminaText->SetVisibility(ESlateVisibility::Visible);
			backStamina->SetVisibility(ESlateVisibility::Visible);
			backHelath->SetVisibility(ESlateVisibility::Visible);
			backScore->SetVisibility(ESlateVisibility::Visible);
			playerOneLifes->SetVisibility(ESlateVisibility::Visible);
			playerOneScore->SetVisibility(ESlateVisibility::Visible);
			playerOneDashes->SetVisibility(ESlateVisibility::Visible);
			dashPlayerOneText->SetVisibility(ESlateVisibility::Visible);

		}
		else
		{

			playerTwoHealth->SetVisibility(ESlateVisibility::Visible);
			playerTwoStamina->SetVisibility(ESlateVisibility::Visible);
			playerTwoImage->SetVisibility(ESlateVisibility::Visible);
			playerTwoSpecialCharges->SetVisibility(ESlateVisibility::Visible);
			playerTwoSpecialChargeImg->SetVisibility(ESlateVisibility::Visible);
			staminaText_1->SetVisibility(ESlateVisibility::Visible);
			backStamina_1->SetVisibility(ESlateVisibility::Visible);
			backHelath_1->SetVisibility(ESlateVisibility::Visible);
			backScore_1->SetVisibility(ESlateVisibility::Visible);
			playerTwoLifes->SetVisibility(ESlateVisibility::Visible);
			playerTwoScore->SetVisibility(ESlateVisibility::Visible);
			playerTwoDashes->SetVisibility(ESlateVisibility::Visible);
			dashPlayerTwoText->SetVisibility(ESlateVisibility::Visible);

		}

	}
	else
	{

		if (id == 0)
		{

			controlsHintText->SetVisibility(ESlateVisibility::Hidden);
			playerOneHealth->SetVisibility(ESlateVisibility::Hidden);
			playerOneStamina->SetVisibility(ESlateVisibility::Hidden);
			playerOneImage->SetVisibility(ESlateVisibility::Hidden);
			playerOneSpecialCharges->SetVisibility(ESlateVisibility::Hidden);
			playerOneSpecialChargeImg->SetVisibility(ESlateVisibility::Hidden);
			staminaText->SetVisibility(ESlateVisibility::Hidden);
			backStamina->SetVisibility(ESlateVisibility::Hidden);
			backHelath->SetVisibility(ESlateVisibility::Hidden);
			backScore->SetVisibility(ESlateVisibility::Hidden);
			playerOneLifes->SetVisibility(ESlateVisibility::Hidden);
			playerOneScore->SetVisibility(ESlateVisibility::Hidden);
			playerOneDashes->SetVisibility(ESlateVisibility::Hidden);
			dashPlayerOneText->SetVisibility(ESlateVisibility::Hidden);

		}
		else
		{

			playerTwoHealth->SetVisibility(ESlateVisibility::Hidden);
			playerTwoStamina->SetVisibility(ESlateVisibility::Hidden);
			playerTwoImage->SetVisibility(ESlateVisibility::Hidden);
			playerTwoSpecialCharges->SetVisibility(ESlateVisibility::Hidden);
			playerTwoSpecialChargeImg->SetVisibility(ESlateVisibility::Hidden);
			staminaText_1->SetVisibility(ESlateVisibility::Hidden);
			backStamina_1->SetVisibility(ESlateVisibility::Hidden);
			backHelath_1->SetVisibility(ESlateVisibility::Hidden);
			backScore_1->SetVisibility(ESlateVisibility::Hidden);
			playerTwoLifes->SetVisibility(ESlateVisibility::Hidden);
			playerTwoScore->SetVisibility(ESlateVisibility::Hidden);
			playerTwoDashes->SetVisibility(ESlateVisibility::Hidden);
			dashPlayerTwoText->SetVisibility(ESlateVisibility::Hidden);

		}

	}

}

void UWidgetGameUI::SetUIForEnemy(UTexture2D* tex, int id, float percentage)
{

	if (id == 0) {
		enemyOneImage->SetBrushFromTexture(tex, false);
		enemyOneHealth->SetPercent(percentage);
	}
	else {
		enemyTwoImage->SetBrushFromTexture(tex, false);
		enemyTwoHealth->SetPercent(percentage);
	}

}

void UWidgetGameUI::SetVisibilityEnemyUI(int id, bool show)
{

	if (show) {
		if (id == 0) {
			enemyOneHealth->SetVisibility(ESlateVisibility::Visible);
			enemyOneImage->SetVisibility(ESlateVisibility::Visible);
			backEnemyHP->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			enemyTwoHealth->SetVisibility(ESlateVisibility::Visible);
			enemyTwoImage->SetVisibility(ESlateVisibility::Visible);
			backEnemyHP_1->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else {
		if (id == 0) {
			enemyOneHealth->SetVisibility(ESlateVisibility::Hidden);
			enemyOneImage->SetVisibility(ESlateVisibility::Hidden);
			backEnemyHP->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			enemyTwoImage->SetVisibility(ESlateVisibility::Hidden);
			enemyTwoHealth->SetVisibility(ESlateVisibility::Hidden);
			backEnemyHP_1->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}

void UWidgetGameUI::BeginContinueSign()
{

	GetWorld()->GetTimerManager().SetTimer(timerContinue, this, &UWidgetGameUI::ContinueLevelSign, timeContinue, true);

}

void UWidgetGameUI::ContinueLevelSign()
{
	
	if (hintCounter < maxHints)
	{

		hintCounter++;
		imageId = !imageId;

		if (imageId)
		{

			goImage->SetVisibility(ESlateVisibility::Visible);
			fingerImage->SetVisibility(ESlateVisibility::Hidden);

		}
		else if (!imageId)
		{

			goImage->SetVisibility(ESlateVisibility::Hidden);
			fingerImage->SetVisibility(ESlateVisibility::Visible);

		}

	}
	else if (hintCounter >= maxHints)
	{
		
		goImage->SetVisibility(ESlateVisibility::Hidden);
		fingerImage->SetVisibility(ESlateVisibility::Hidden);
		hintCounter = 0;
		GetWorld()->GetTimerManager().ClearTimer(timerContinue);

	}

}

void UWidgetGameUI::SetPlayerHP(int32 playerID, float hpValue)
{

	if (playerID == 0) { playerOneHealth->SetPercent(hpValue); }

	else { playerTwoHealth->SetPercent(hpValue); }

}

void UWidgetGameUI::SetPlayerSpecialCharge(int32 playerID, float specialChargeValue)
{

	if (playerID == 0) { playerOneSpecialCharges->SetPercent(specialChargeValue); }

	else { playerTwoSpecialCharges->SetPercent(specialChargeValue); }

}

void UWidgetGameUI::SetPlayerStamina(int32 playerID, float staminaValue)
{

	if (playerID == 0) { playerOneStamina->SetPercent(staminaValue); }

	else { playerTwoStamina->SetPercent(staminaValue); }

}

void UWidgetGameUI::SetPlayerLifes(int32 playerID, int32 lifes)
{

	if (playerID == 0) { playerOneLifes->SetText(FText::AsNumber(lifes)); }

	else { playerTwoLifes->SetText(FText::AsNumber(lifes)); }

}

void UWidgetGameUI::SetPlayerPoints(int32 playerID, int32 points)
{

	if (playerID == 0) { playerOneScore->SetText(FText::AsNumber(points)); }

	else { playerTwoScore->SetText(FText::AsNumber(points)); }

}

void UWidgetGameUI::SetPlayerDashes(int32 playerID, int32 dashCharges)
{

	if (playerID == 0) { playerOneDashes->SetText(FText::AsNumber(dashCharges)); }

	else { playerTwoDashes->SetText(FText::AsNumber(dashCharges)); }

}

void UWidgetGameUI::SetBossHP(float hpValue)
{

	bossBar->SetPercent(hpValue);

}