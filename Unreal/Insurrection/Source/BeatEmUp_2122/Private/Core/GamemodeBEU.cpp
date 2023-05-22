// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/GamemodeBEU.h"
#include "Enemies/EnemyCommander.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstanceBEU.h"
#include "Player/TurboPlayer.h"
#include "HUD/HUDGameLevel.h"
#include "Misc/CoreDelegates.h"
#include "Core/CameraBEU.h"
#include "HUD/HUDMenu.h"
#include "Core/CheatManagerBEU.h"
#include "Player/PlayerControllerBEU.h"
#include "EngineSettings/Classes/GameMapsSettings.h"
#include "Enemies/FinalBossCharacter.h"
#include "MediaTexture.h"

AGamemodeBEU::AGamemodeBEU(){

  numPlayers = 1;
	gameDifficulty = Difficulty::kDificulty_Easy;

}

void AGamemodeBEU::BeginPlay(){
	// Spawn Enemy Commander
	Super::BeginPlay();

  /*UGameMapsSettings* Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());
  if (IsValid(Settings)) {
    Settings->bOffsetPlayerGamepadIds = t;
  }*/

  FName levelName = FName(*GetWorld()->GetName());


	hudCreated_ = false;

	if (numPlayers == 1) {
		APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FInputModeGameOnly auxInputHelper = FInputModeGameOnly();
		player0->SetInputMode(auxInputHelper);
		gameDifficulty = Difficulty::kDificulty_Easy;
	}
	else
	{
		APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		APlayerController* player1 = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		FInputModeGameOnly auxInputHelper = FInputModeGameOnly();
		player0->SetInputMode(auxInputHelper);
		player1->SetInputMode(auxInputHelper);
		gameDifficulty = Difficulty::kDificulty_Hard;
	}

	//TMap <FKey, EUINavigation>& KeysForFocus = FSlateApplication::Get().GetNavigationConfig()->KeyEventRules;
	//
	//KeysForFocus.Emplace(EKeys::W, EUINavigation::Up);
	//KeysForFocus.Emplace(EKeys::S, EUINavigation::Down);
	//KeysForFocus.Emplace(EKeys::A, EUINavigation::Left);
	//KeysForFocus.Emplace(EKeys::D, EUINavigation::Right);

	FTransform tmp_tr;
	tmp_tr.SetLocation(FVector(0.f, 0.f, 0.f));
	tmp_tr.SetRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());
	gameInstance = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

  gameInstance->SetDificultyLevel(levelName);

	enemyCommander = GetWorld()->SpawnActorDeferred<AEnemyCommander>(BP_enemyCommander, tmp_tr);
	hud_ = Cast<AHUDGameLevel>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	enemyCommander->hud = hud_;
	numPlayers = gameInstance->numPlayers;

	gameDifficulty = Difficulty::kDificulty_Easy;

	finalBoss = Cast<AFinalBossCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), finalBossClass));

	if (finalBoss)
	{

		finalBoss->UpdateBossHealthBar.AddDynamic(hud_, &AHUDGameLevel::UpdateBossBar);

	}

	if (numPlayers > 1 && !gameInstance->thisBooleanExistsToPreventSpawningMoreThan2PlayersBoogaloo) {
		UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
		//gameDifficulty = Difficulty::kDificulty_Hard;
	}

	// Get Character References
	for (int i = 0; i < numPlayers; ++i) {
		ATurboPlayer* tmp = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), i)->GetCharacter());
		players.Add(tmp);
		tmp->characterID = i;

		enemyCommander->mainCharacter.Add(players[i]);
		players[i]->catchEnemyDataOnHit.AddDynamic(hud_, &AHUDGameLevel::ShowEnemyBar);
		
		players[i]->updateUIHP.AddDynamic(hud_, &AHUDGameLevel::UpdatePlayerHPBar);
		players[i]->updateUIAOE.AddDynamic(hud_, &AHUDGameLevel::UpdatePlayerSpecialChargesBar);
		players[i]->updateUIStamina.AddDynamic(hud_, &AHUDGameLevel::UpdatePlayerStaminaBar);
		players[i]->updateUILifes.AddDynamic(hud_, &AHUDGameLevel::UpdatePlayerLifes);
		players[i]->updateUIScore.AddDynamic(hud_, &AHUDGameLevel::UpdatePlayerPoints);
		players[i]->updateUIDash.AddDynamic(hud_, &AHUDGameLevel::UpdatePlayerDashes);
		
		players[i]->triggerEvent.AddDynamic(enemyCommander, &AEnemyCommander::GetSpawnTrigger);
		players[i]->myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

	}
  if (numPlayers > 1  && !gameInstance->thisBooleanExistsToPreventSpawningMoreThan2PlayersBoogaloo) {
		players[1]->original_color = FColor(255,0,0,255);
    /*const ConstructorHelpers::FObjectFinder<UTexture> TextureColor(TEXT("/Game/Art/materials/Personajes/Player/PLAYERV2_BASECOLOR"));
    const ConstructorHelpers::FObjectFinder<UTexture> TextureEmisive(TEXT("/Game/Art/materials/Personajes/Player/PLAYERV2_EMISIVO"));
    players[1]->materialCharacter->SetTextureParameterValue(FName("BASECOLOR"), TextureColor.Object);
    players[1]->materialCharacter->SetTextureParameterValue(FName("EMISIVE"), TextureEmisive.Object);*/
	gameInstance->thisBooleanExistsToPreventSpawningMoreThan2PlayersBoogaloo = true;

  }
	
	
	/*FActorSpawnParameters spawnParams;
	gameInstance->camera = GetWorld()->SpawnActor<ACameraBEU>(cameraBP, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(gameInstance->camera);*/


  //testsong.start

}



void AGamemodeBEU::BeginGame()
{

}