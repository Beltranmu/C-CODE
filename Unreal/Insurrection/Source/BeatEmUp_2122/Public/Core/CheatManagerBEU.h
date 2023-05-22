// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "CheatManagerBEU.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UCheatManagerBEU : public UCheatManager
{
	GENERATED_BODY()
	
public:

  bool godMode_ = true;
  bool noDMGMode_ = true;
  bool unlimitedUltimate_ = true;
  bool infiniteDash_ = true;

   UFUNCTION(exec)
   void unlimitedUltimate();
    
   UFUNCTION(exec)
   void godMode(); 

   UFUNCTION(exec)
     void noDMGMode();

   UFUNCTION(exec)
   void drawAttacks();

   UFUNCTION(exec)
     void infiniteDash();


   UFUNCTION(exec)
     void timeDilatation(float t);

};
