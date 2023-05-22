// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerControllerBEU.h"
#include "Core/CheatManagerBEU.h"

APlayerControllerBEU::APlayerControllerBEU(){

  CheatClass = UCheatManagerBEU::StaticClass();

}