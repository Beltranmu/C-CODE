// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Framework/mainmenu_gm.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include <Framework/arktquera_player_controller.h>
#include "../../Public/UI/arktequeraHUD.h"

Amainmenu_gm::Amainmenu_gm() {

}

void Amainmenu_gm::BeginPlay() {

 

  /*Aarktquera_player_controller *player_controller = Cast<Aarktquera_player_controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

  game_hud = Cast<AarktequeraHUD>(player_controller->GetHUD());*/

  //game_hud->BeginGame();

}