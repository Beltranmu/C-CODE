// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameInstanceBEU.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/CustomNavigationConfig.h"

#include "Map/LevelScriptActorBEU.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/World.h"

void UGameInstanceBEU::deathplayer(){
  //Both player death

  if(numPlayers == 1){
    if (playerLife[0] <= 0 ) {
      
      //playerLife[0] = 3;
      //UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
      //UGameplayStatics::OpenLevel(this, FName("MainMenu"), false);
    }
  }
  else{
    if (playerLife[0] <= 0 && playerLife[1] <= 0) {
      playerLife[0] = 3;
      playerLife[1] = 3;
      //UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
      //UGameplayStatics::OpenLevel(this, FName("MainMenu"), false);
    }
  }

}

void UGameInstanceBEU::Init()
{

  Super::Init();

  FSlateApplication::Get().SetNavigationConfig( { MakeShared<FCustomNavigationConfig>() });

}


void UGameInstanceBEU::SetDificultyLevel(FName levelName) {

  
  currentLevelID = 0;
  int cmpResult  = levelName.Compare(FName("Level_01_Fixed"));
  if (0 == cmpResult) {
    currentLevelID = 0;
  }

  cmpResult = levelName.Compare(FName("Level_02_Fixed"));
  if (0 == cmpResult) {
    currentLevelID = 1;
  }

  cmpResult = levelName.Compare(FName("Level_03_Fixed"));
  if (0 == cmpResult) {
    currentLevelID = 2;
  }

  cmpResult = levelName.Compare(FName("Level_04_Boss_Arena_Fixed"));
  if (0 == cmpResult) {
    currentLevelID = 3;
  }

   cmpResult = levelName.Compare(FName("Level_04_Boss_Arena_Fixed"));
  if (0 == cmpResult) {
    currentLevelID = 4;
  }
  cmpResult = levelName.Compare(FName("Level_04_Elevator_arena_Fixed"));
  if (0 == cmpResult) {
    currentLevelID = 4;
  }

  /*ALevelScriptActorBEU* levelscript = Cast<ALevelScriptActorBEU>(UWorld::GetLevelScriptActor());
  
  LevelScriptActor* levelscript = UWorld::GetLevelScriptActor();*/



  
}

