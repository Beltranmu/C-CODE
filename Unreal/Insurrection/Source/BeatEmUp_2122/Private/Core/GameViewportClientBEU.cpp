// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameViewportClientBEU.h"
#include "Core/GameInstanceBEU.h"
#include "Kismet/GameplayStatics.h"

bool UGameViewportClientBEU::InputKey(const FInputKeyEventArgs& EventArgs)
{

  FInputKeyEventArgs newEvent = EventArgs;
  if (newEvent.IsGamepad()) {

    FName buttonName = newEvent.Key.GetFName();
    if (buttonName == FName("Gamepad_FaceButton_Bottom")) {
    }
  }
  else {
    FName keyName = newEvent.Key.GetFName();
     
    if (keyName == FName("SpaceBar")) {

    }
  }
  if (EventArgs.bIsTouchEvent) {  
    newEvent.ControllerId++;
  }
 
  return Super::InputKey(newEvent);
}

bool UGameViewportClientBEU::InputAxis(FViewport* Viewport_p, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples /*= 1*/, bool bGamepad /*= false*/)
{
  
  if(bGamepad) { 
      return Super::InputAxis(Viewport_p, ControllerId , Key, Delta,
        DeltaTime, NumSamples, bGamepad);
  }


  return Super::InputAxis(Viewport_p, ControllerId + 1, Key, Delta, DeltaTime, NumSamples = 1, bGamepad = false);
}



