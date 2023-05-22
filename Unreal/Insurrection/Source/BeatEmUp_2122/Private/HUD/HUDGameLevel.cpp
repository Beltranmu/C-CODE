#include "HUD/HUDGameLevel.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WidgetContainer.h"
#include "HUD/WidgetMainMenu.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"
#include "Core/GameInstanceBEU.h"
#include "HUD/WidgetGameUI.h"
#include "Core/GamemodeBEU.h"

#include "Framework/Application/NavigationConfig.h"


AHUDGameLevel::AHUDGameLevel()
{

	isAllHidden = false;

}

void AHUDGameLevel::BeginPlay()
{

	Super::BeginPlay();

	gameInstance = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));
	AGamemodeBEU* gameMode = Cast<AGamemodeBEU>(UGameplayStatics::GetGameMode(GetWorld()));
	playerOne = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerTwo = UGameplayStatics::GetPlayerController(GetWorld(), 1);

	if (playerOne != nullptr)
	{
		
		gameMode->hudCreated_ = true;

		if (gameInstance->numPlayers == 1)
		{

			if (!PlayerOwner)
			{
				return;
			}

			widgetContainer = CreateWidget<UWidgetContainer>(playerOne, widgetSelector);
			widgetContainer->AddToPlayerScreen();

			widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(0, true);
			widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(1, false);
			widgetContainer->BP_WidgetGameUI->SetVisibilityEnemyUI(0, false);
			widgetContainer->BP_WidgetGameUI->SetVisibilityEnemyUI(1, false);
			idEnemyHit[0] = -1;
			idEnemyHit[1] = -1;

			UWidgetGameUI* UIWidget = Cast<UWidgetGameUI>(widgetContainer->WidgetSwitcher->GetActiveWidget());
			FInputModeGameOnly auxInputHelper = FInputModeGameOnly();

			if (UIWidget)
			{

				playerOne->SetInputMode(auxInputHelper);
				playerOne->bShowMouseCursor = false;

			}

		}
		else
		{

			if (!PlayerOwner)
			{

				return;

			}
			else if (PlayerOwner == playerOne)
			{

				widgetContainer = CreateWidget<UWidgetContainer>(playerOne, widgetSelector);
				widgetContainer->AddToPlayerScreen();

				widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(0, true);
				widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(1, true);
				widgetContainer->BP_WidgetGameUI->SetVisibilityEnemyUI(0, false);
				widgetContainer->BP_WidgetGameUI->SetVisibilityEnemyUI(1, false);
				idEnemyHit[0] = -1;
				idEnemyHit[1] = -1;

				UWidgetGameUI* UIWidget = Cast<UWidgetGameUI>(widgetContainer->WidgetSwitcher->GetActiveWidget());
				FInputModeGameOnly auxInputHelper = FInputModeGameOnly();

				if (UIWidget)
				{

					playerOne->SetInputMode(auxInputHelper);
					playerOne->bShowMouseCursor = false;

				}

			}
			else if (PlayerOwner == playerTwo)
			{

				FInputModeGameOnly auxInputHelper = FInputModeGameOnly();

				playerTwo->SetInputMode(auxInputHelper);
				playerTwo->bShowMouseCursor = false;

			}

		}

	}

	for (int i = 0; i < gameInstance->numPlayers; i++) {

		ATurboPlayer* player = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), i));

		if (player && widgetContainer) {
			
			UpdatePlayerLifes(i, gameInstance->playerLife[i]);
			UpdatePlayerSpecialChargesBar(i, gameInstance->playerAOEAttackCharge[i], player->maxAOECharge);

		}
	
	}
}

void AHUDGameLevel::EndPlay(EEndPlayReason::Type EndReason)
{

	Super::EndPlay(EndReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

}

void AHUDGameLevel::UpdateEnemiesBar(float percentageHP, int enemyType, int32 idEnemy, int idCharacter)
{

	ShowEnemyBar(enemyType, percentageHP, idCharacter);
	idEnemyHit[idCharacter] = idEnemy;
	int otherCharacterID = 1;
	if (idCharacter == 1) {
		otherCharacterID = 0;
	}
	if (idEnemyHit[otherCharacterID] == idEnemyHit[idCharacter]) {

		ShowEnemyBar(enemyType, percentageHP, otherCharacterID);
	}

}

void  AHUDGameLevel::ShowEnemyBar(int id, float percentage, int id_char)
{

	widgetContainer->BP_WidgetGameUI->SetUIForEnemy(enemyImgArray[id], id_char, percentage);

	if (!isAllHidden)
	{

		widgetContainer->BP_WidgetGameUI->SetVisibilityEnemyUI(id_char, true);

		delegate_timer_enemyUI[id_char].BindUFunction(this, FName("ResetsEnemyUI"), id_char);
		GetWorldTimerManager().SetTimer(timer_enemyUI[id_char], delegate_timer_enemyUI[id_char], 3.0f, false);

	}

}

void  AHUDGameLevel::ResetsEnemyUI(int player_id)
{

	if (!isAllHidden)
	{

		widgetContainer->BP_WidgetGameUI->SetVisibilityEnemyUI(player_id, false);
		idEnemyHit[player_id] = -1;
	
	}

}

void AHUDGameLevel::DialogBehaviour()
{

	widgetContainer->changeScreen(1);

}

void AHUDGameLevel::StartLevelWidget()
{

	widgetContainer->changeScreen(6);

}

void AHUDGameLevel::DialogEnd()
{

	widgetContainer->changeScreen(0);

}

void AHUDGameLevel::ShowDeadMenu()
{

	APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeUIOnly auxInputHelper = FInputModeUIOnly();
	player0->SetInputMode(auxInputHelper);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), slowTime);

	widgetContainer->changeScreen(7);

}

void AHUDGameLevel::PauseGame()
{

	APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeUIOnly auxInputHelper = FInputModeUIOnly();
	player0->SetPause(true);
	player0->SetInputMode(auxInputHelper);

	widgetContainer->changeScreen(2);

}

void AHUDGameLevel::UpdatePlayerHPBar(int32 playerID, float currentHP, float maxHP)
{

	widgetContainer->BP_WidgetGameUI->SetPlayerHP(playerID, currentHP / maxHP);

}

void AHUDGameLevel::UpdatePlayerSpecialChargesBar(int32 playerID, float currentSpecialCharge, float maxSpecialCharge)
{

	widgetContainer->BP_WidgetGameUI->SetPlayerSpecialCharge(playerID, currentSpecialCharge / maxSpecialCharge);

}

void AHUDGameLevel::UpdatePlayerStaminaBar(int32 playerID, float currentStamina, float maxStamina)
{

	widgetContainer->BP_WidgetGameUI->SetPlayerStamina(playerID, currentStamina / maxStamina);

}

void AHUDGameLevel::UpdatePlayerLifes(int32 playerID, int32 lifes)
{

	widgetContainer->BP_WidgetGameUI->SetPlayerLifes(playerID, lifes);

}

void AHUDGameLevel::UpdatePlayerPoints(int32 playerID, int32 points)
{

	widgetContainer->BP_WidgetGameUI->SetPlayerPoints(playerID, points);

}

void AHUDGameLevel::UpdatePlayerDashes(int32 playerID, int32 dashCharges)
{

	widgetContainer->BP_WidgetGameUI->SetPlayerDashes(playerID, dashCharges);

}

void AHUDGameLevel::UpdateBossBar(float currentHP, float maxHP)
{

	widgetContainer->BP_WidgetGameUI->SetBossHP(currentHP / maxHP);

}

void AHUDGameLevel::HideAllUI()
{

	isAllHidden = !isAllHidden;

	if (isAllHidden)
	{

		if (gameInstance->numPlayers == 1)
		{

			widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(0, false);

		}
		else
		{
		
			widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(0, false);
			widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(1, false);

		}

	}
	else
	{

		if (gameInstance->numPlayers == 1)
		{

			widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(0, true);

		}
		else
		{

			widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(0, true);
			widgetContainer->BP_WidgetGameUI->SetVisibilityPlayersUI(1, true);

		}

	}

}

void AHUDGameLevel::ShowGoHint()
{

	widgetContainer->BP_WidgetGameUI->BeginContinueSign();

}
