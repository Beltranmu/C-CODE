// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AtqGameMode.generated.h"

/**
 *
*/
class UPlayerInventory;
class AArktequeraCharacter;
class AarktequeraHUD;
class URecipe;
class AProceduralMap;
class AEnviromentProp;
class ADino;

UCLASS()
class ARKTEQUERA_API AAtqGameMode : public AGameModeBase
{
  GENERATED_BODY()
public:
  AAtqGameMode();
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
    float nEnemies;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies")
    TArray<AActor*> Enemies;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    AArktequeraCharacter* player;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    UPlayerInventory* inventory;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
    AarktequeraHUD* game_hud;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    TArray<TSubclassOf<URecipe>> BP_Recipes;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    TArray<URecipe*> Recipes;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    TSubclassOf<AProceduralMap> world_reference;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    FVector  world_scale;

  UPROPERTY(EditAnywhere, Category = "World")
    int numberStones;

  UPROPERTY(EditAnywhere, Category = "World")
    int numberTrees;

  UPROPERTY(EditAnywhere, Category = "World")
    int numberDinos;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    TSubclassOf<AEnviromentProp> Tree;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    TSubclassOf<AEnviromentProp> Stone;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    TArray<TSubclassOf<ADino>> Dinos;

  UFUNCTION()
    void InteractAction(AActor* material_actor, int dmg);

  UFUNCTION()
    void Test1(float x);

  UFUNCTION()
    void CheckRecipe(int recipe);

  UFUNCTION()
    void SpawnEnviromentalProps();

  UFUNCTION()
    void SpawnDinos();
};
