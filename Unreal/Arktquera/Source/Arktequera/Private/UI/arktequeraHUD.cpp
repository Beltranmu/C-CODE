// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/UI/arktequeraHUD.h"
#include "../../Public/UI/arktequeraWidget.h"
#include "../../Public/UI/PauseMenu.h"
#include "../../Public/UI/MenuWidget.h"
#include "../../Public/UI/Inventorywidget.h"
#include "../../Public/UI/arktequeraWidget.h"
#include "../../Public/Framework/arktquera_player_controller.h"
#include "../../Public/Framework/Resources.h"
#include "../../Public/Player/PlayerInventory.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


#include "Engine/Engine.h"

void AarktequeraHUD::BeginPlay() {
  Super::BeginPlay();

  PauseWidget = CreateWidget<UPauseMenu>(PlayerOwner, BPPauseWidget);
 // PauseWidget->ResumeText->SetText(PauseWidget->RText);

  PlayerOwner->SetInputMode(FInputModeGameOnly());
  PlayerOwner->bShowMouseCursor = false;


  InventoryWidget = CreateWidget<UInventorywidget>(PlayerOwner, BPInventoryWidget);
  GameWidget = CreateWidget<UarktequeraWidget>(PlayerOwner, BPGameWidget);

  
  InventoryWidget->Init();
}





void AarktequeraHUD::EnablePauseMenu(bool enabled) {

  if (enabled) {
    PlayerOwner->SetInputMode(FInputModeGameAndUI());
    PauseWidget->AddToPlayerScreen();
    PlayerOwner->bShowMouseCursor = true;
    Aarktquera_player_controller* current_pc = Cast<Aarktquera_player_controller>(PlayerOwner);
    PauseWidget->ResumeButton->OnPressed.AddDynamic(current_pc, &Aarktquera_player_controller::PauseMenu);
    PauseWidget->MainButton->OnPressed.AddDynamic(this, &AarktequeraHUD::ReturnToMain);
  }

  else {
    PlayerOwner->SetInputMode(FInputModeGameOnly());
    FScriptDelegate rmEvent;
    rmEvent.BindUFunction(PlayerOwner, TEXT("PauseMenu"));

    FScriptDelegate rmEvent1;
    rmEvent1.BindUFunction(this, TEXT("ReturnToMain"));

    PauseWidget->ResumeButton->OnPressed.Remove(rmEvent);
    PauseWidget->MainButton->OnPressed.Remove(rmEvent1);
    PlayerOwner->bShowMouseCursor = false;
    PauseWidget->RemoveFromParent();
  }

}

void AarktequeraHUD::EnableInventory(bool enabled) {

  if (enabled) {
    PlayerOwner->SetInputMode(FInputModeGameAndUI());
    InventoryWidget->AddToPlayerScreen();
    PlayerOwner->bShowMouseCursor = true;
   // Aarktquera_player_controller* current_pc = Cast<Aarktquera_player_controller>(PlayerOwner);
    //PauseWidget->ResumeButton->OnPressed.AddDynamic(current_pc, &Aarktquera_player_controller::PauseMenu);
  }

  else {
    PlayerOwner->SetInputMode(FInputModeGameOnly());
    //FScriptDelegate rmEvent;
    //rmEvent.BindUFunction(PlayerOwner, TEXT("PauseMenu"));
    //PauseWidget->ResumeButton->OnPressed.Remove(rmEvent);
    PlayerOwner->bShowMouseCursor = false;
    InventoryWidget->RemoveFromParent();
  }

}

void AarktequeraHUD::UpdateInvetory(TArray<UResources*> myResources) {
  UImage* object_img;
  UTextBlock* cuantity;

  InventoryWidget->Clean();

  for (int i = 0; i < myResources.Num(); ++i) {
    object_img = Cast<UImage>(InventoryWidget->Slots[i]->GetChildAt(0));
    cuantity = Cast<UTextBlock>(InventoryWidget->Slots[i]->GetChildAt(1));
    object_img->SetBrushFromTexture(inventory_img[myResources[i]->Type()],false);
    object_img->SetOpacity(1.0f);

    cuantity->SetText(FText::FromString(FString::SanitizeFloat(myResources[i]->Amount())));
  }

 
}

void AarktequeraHUD::ReturnToMain() {

  PlayerOwner->SetInputMode(FInputModeGameAndUI());
  FScriptDelegate rmEvent;
  rmEvent.BindUFunction(PlayerOwner, TEXT("PauseMenu"));
  PauseWidget->ResumeButton->OnPressed.Remove(rmEvent);

  FScriptDelegate rmEvent1;
  rmEvent1.BindUFunction(this, TEXT("ReturnToMain"));
  PauseWidget->MainButton->OnPressed.Remove(rmEvent1);

  PlayerOwner->bShowMouseCursor = true;
  UGameplayStatics::OpenLevel(GetWorld(), FName("Main_menu"));
}