// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScriptActorBEU.generated.h"

/**
 * 
 */
class AAmbientSound;
class UAudioComponent;
USTRUCT(BlueprintType)
struct FAmbientSongs {

  GENERATED_BODY()





  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ambient")
    USoundBase* ambientSong;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ambient"/*, meta = (ClampMin = "10.0", ClampMax "100.0", UIMin = "10.0", UIMax = "100.0")*/)
    float minTime = 10.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ambient"/*, meta = (ClampMin = "20.0", ClampMax "100.0", UIMin = "20.0", UIMax = "100.0")*/)
    float maxTime = 20.0f;

    FTimerHandle timerAmbient;

};




UCLASS()
class BEATEMUP_2122_API ALevelScriptActorBEU : public ALevelScriptActor
{
  GENERATED_BODY()


public:

    ALevelScriptActorBEU();
    AAmbientSound* ambientSongobj;
    UAudioComponent* soundComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelSong")
      USoundBase* mainSong;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelSong")
      TArray<FAmbientSongs> ambientSong;



    virtual void BeginPlay();

    UFUNCTION()
    void PlayAmbient();
	
};
