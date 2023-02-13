// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/UI/mainMenuHUD.h"
#include "../../Public/UI/MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"


void AmainMenuHUD::BeginPlay() {
  Super::BeginPlay();

  

  PlayerOwner->SetInputMode(FInputModeGameOnly());
  PlayerOwner->bShowMouseCursor = true;

  MenuWidget = CreateWidget<UMenuWidget>(PlayerOwner, BPMenuWidget);
  MenuWidget->AddToPlayerScreen();

  MenuWidget->StartButton->OnPressed.AddDynamic(this, &AmainMenuHUD::StartGame);
  MenuWidget->ExitButton->OnPressed.AddDynamic(this, &AmainMenuHUD::Exit);


}


void AmainMenuHUD::StartGame() {

  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("START!!!"));
  UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
  MenuWidget->RemoveFromParent();
  PlayerOwner->SetInputMode(FInputModeGameOnly());
  PlayerOwner->bShowMouseCursor = false;
}

void AmainMenuHUD::Exit() {
  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("EXIT!!!"));
}
