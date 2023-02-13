// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Framework/arktquera_player_controller.h"
#include "../../Public/UI/arktequeraHUD.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"


void Aarktquera_player_controller::BeginPlay() {
  Super::BeginPlay();
  game_paused = false;


}

void Aarktquera_player_controller::SetupInputComponent() {

  Super::SetupInputComponent();

  // FInputActionBinding pauseAction =
     //InputComponent->BindAction(Nombre, modo, objeto, funcion);
  InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::PauseMenu).bExecuteWhenPaused = true;

  InputComponent->BindAction("Invetory", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::InvetoryMenu).bExecuteWhenPaused = true;

  InputComponent->BindAction("Test1", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::TestF).bExecuteWhenPaused = true;



  InputComponent->BindAction("Craft1", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft1);
  InputComponent->BindAction("Craft2", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft2);
  InputComponent->BindAction("Craft3", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft3);
  InputComponent->BindAction("Craft4", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft4);
  InputComponent->BindAction("Craft5", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft5);
  /*InputComponent->BindAction("Craft6", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft6);
  InputComponent->BindAction("Craft7", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft7);
  InputComponent->BindAction("Craft8", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft8);
  InputComponent->BindAction("Craft9", EInputEvent::IE_Pressed, this, &Aarktquera_player_controller::Craft9);*/



  // pauseAction.bExecuteWhenPaused = true;
}

void Aarktquera_player_controller::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  if(Test1.IsBound()){
    Test1.RemoveAll(this);
  }

  if (crafting_event.IsBound()) {
    crafting_event.RemoveAll(this);
  }
}

void Aarktquera_player_controller::PauseMenu() {
  game_paused = !game_paused;

  if (game_paused) {
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("PAUSAAA"));
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("NO PAUSAAA"));
  }

  UGameplayStatics::SetGamePaused(GetWorld(), game_paused);
  AHUD* currentHUD = GetHUD();
  AarktequeraHUD* currentPlayerHUD = Cast<AarktequeraHUD>(currentHUD);
  currentPlayerHUD->EnablePauseMenu(game_paused);

}


void Aarktquera_player_controller::InvetoryMenu() {
  show_inventory = !show_inventory;

  if (show_inventory) {
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Invetario"));
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("NO Invetario"));
  }

  //UGameplayStatics::SetGamePaused(GetWorld(), game_paused);
  AHUD* currentHUD = GetHUD();
  AarktequeraHUD* currentPlayerHUD = Cast<AarktequeraHUD>(currentHUD);
  currentPlayerHUD->EnableInventory(show_inventory);

}

void Aarktquera_player_controller::TestF() {
  Test1.Broadcast(0);
}


void Aarktquera_player_controller::Craft1() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Crafted 2x Foundation"));
  crafting_event.Broadcast(0);
}

void Aarktquera_player_controller::Craft2() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Crafted 2x Walls"));
  crafting_event.Broadcast(1);
}

void Aarktquera_player_controller::Craft3() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Crafted 2x Ceiling"));
  crafting_event.Broadcast(2);
}

void Aarktquera_player_controller::Craft4() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Crafted 2x Ramps"));
  crafting_event.Broadcast(3);
}

void Aarktquera_player_controller::Craft5() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Crafted 1x Door"));
  crafting_event.Broadcast(4);
}

/*void Aarktquera_player_controller::Craft6() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CRAFTING66666666"));
  crafting_event.Broadcast(5);
}

void Aarktquera_player_controller::Craft7() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CRAFTING777777777"));
  crafting_event.Broadcast(6);
}

void Aarktquera_player_controller::Craft8() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CRAFTING888888888"));
  crafting_event.Broadcast(7);
}

void Aarktquera_player_controller::Craft9() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CRAFTING999999999"));
  crafting_event.Broadcast(8);
}*/

/*void Aarktquera_player_controller::Craft2() {
  crafting_event.Broadcast(2);
}*/

