// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CheatManagerBEU.h"
#include "Core/GamemodeBEU.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TurboPlayer.h"

void UCheatManagerBEU::unlimitedUltimate() {
	AGamemodeBEU* gmBEU = Cast<AGamemodeBEU>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gmBEU) {
		gmBEU->players[0]->unlimitedUltimate = this->unlimitedUltimate_;
		gmBEU->players[gmBEU->numPlayers - 1]->unlimitedUltimate = this->unlimitedUltimate_;
	}

	unlimitedUltimate_ = !unlimitedUltimate_;
}

void UCheatManagerBEU::godMode() {
	AGamemodeBEU* gmBEU = Cast<AGamemodeBEU>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gmBEU) {
		gmBEU->players[0]->godMode_ = this->godMode_;
		gmBEU->players[gmBEU->numPlayers - 1]->godMode_ = this->godMode_;
	}
	
	godMode_ = !godMode_;
}

void UCheatManagerBEU::noDMGMode() {
	AGamemodeBEU* gmBEU = Cast<AGamemodeBEU>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gmBEU) {
		gmBEU->players[0]->noDMGMode_ = this->noDMGMode_;
		gmBEU->players[gmBEU->numPlayers - 1]->noDMGMode_ = this->noDMGMode_;
	}
	
	godMode_ = !godMode_;
}

void UCheatManagerBEU::drawAttacks() {
	AGamemodeBEU* gmBEU = Cast<AGamemodeBEU>(UGameplayStatics::GetGameMode(GetWorld()));

	bool drawA = gmBEU->drawAttacks;


	gmBEU->drawAttacks = !gmBEU->drawAttacks;
}

void UCheatManagerBEU::infiniteDash() {
  AGamemodeBEU* gmBEU = Cast<AGamemodeBEU>(UGameplayStatics::GetGameMode(GetWorld()));

  if (gmBEU) {
    int dashCharges = 3;
    if (infiniteDash_) {
      dashCharges = -1;
    }
    gmBEU->players[0]->maxDashCharges = dashCharges;
    gmBEU->players[gmBEU->numPlayers - 1]->maxDashCharges = dashCharges;
  }

  infiniteDash_ = !infiniteDash_;
}

void UCheatManagerBEU::timeDilatation(float t) {
  UGameplayStatics::SetGlobalTimeDilation(GetWorld(),t);
}