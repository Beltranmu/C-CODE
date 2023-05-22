// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/LevelScriptActorBEU.h"
#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"

ALevelScriptActorBEU::ALevelScriptActorBEU(){

  soundComponent = CreateDefaultSubobject<UAudioComponent>("SoundComponent");

}

void ALevelScriptActorBEU::BeginPlay(){

  Super::BeginPlay();

  //ambientSongobj = GetWorld()->SpawnActor<AAmbientSound>();
  //mainSong.Soun
  //ambientSongobj->Play(0.0f);
  soundComponent->Sound = mainSong;
 //soundComponent->SoundClassOverride;
  soundComponent->Play(1.0f);

  for(int i = 0; i < ambientSong.Num(); ++i){
    float time = FMath::RandRange(ambientSong[i].minTime, ambientSong[i].maxTime);

    //delegate_timer_enemyUI[id_char].BindUFunction(this, FName("ResetsEnemyUI"), id_char);
    //GetWorldTimerManager().SetTimer(timer_enemyUI[id_char], delegate_timer_enemyUI[id_char], 3.0f, false);
    //GetWord()->GetTimerManager()->SetTimer(ambientSong[i].timerAmbient, this, &ALevelScriptActorBEU::PlayAmbient, );
  }

}

void ALevelScriptActorBEU::PlayAmbient(){

}