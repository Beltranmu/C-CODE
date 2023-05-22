// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/NavigationConfig.h"
#include "GameFramework/GameModeBase.h"
#include "GamemodeBEU.generated.h"

class AEnemyCommander;
class ATurboPlayer;
class AHUDGameLevel;
class UGameInstanceBEU;
class ACameraBEU;
class UTexture;
class AFinalBossCharacter;

UENUM(BlueprintType)
enum class Difficulty : uint8 {
  kDificulty_Easy = 0			UMETA(DisplayName = "EASY"),
  kDificulty_Normal = 1		UMETA(DisplayName = "NORMAL"),
  kDificulty_Hard = 2     UMETA(DisplayName = "HARD")
};

/**
 *
 */
UCLASS()
class BEATEMUP_2122_API AGamemodeBEU : public AGameModeBase
{
	GENERATED_BODY()

public:

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Difficulty")
    Difficulty gameDifficulty;

  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
  //  bool t;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Enemies")
		TSubclassOf<AFinalBossCharacter> finalBossClass;

	AFinalBossCharacter* finalBoss;
	
	// ATTRIBUTES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core")
		int32 numPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core")
		AEnemyCommander* enemyCommander;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core")
		TSubclassOf<AEnemyCommander> BP_enemyCommander;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<ATurboPlayer*> players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		AHUDGameLevel* hud_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Instance")
		UGameInstanceBEU* gameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TSubclassOf<ACameraBEU> cameraBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Fix")
		bool hudCreated_;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheatManager")
    bool drawAttacks;



  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TexturePlayer2")
    UTexture* BaseColorTexture;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TexturePlayer2")
    UTexture* EmisiveTexture;

	UFUNCTION()
		void BeginGame();

	// METHODS
	AGamemodeBEU();

	virtual void BeginPlay() override;

	//virtual EUINavigationAction GetNavigationActionFromKey(const FKey& InKey) const override;

};
