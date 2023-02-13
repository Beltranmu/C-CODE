// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Framework/AtqGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

#include <../ArktequeraCharacter.h>
#include <Player/BuildComponent.h>
#include "../../Arktequera/Public/Enviroment/EnviromentProp.h"
#include "../../Public/Player/InteractComponent.h"
#include "../../Public/Player/PlayerInventory.h"

#include "../../Public/Framework/Resources.h"
#include "../../Public/Framework/Recipe.h"
#include <Enemies/IDamagable.h>
#include <Enemies/Dino.h>

#include <Player/PlayerInventory.h>
#include <Framework/arktquera_player_controller.h>
#include <UI/arktequeraHUD.h>
#include <Enviroment/ProceduralMap.h>
#include <Enviroment/EnviromentProp.h>

AAtqGameMode::AAtqGameMode() {

}

void AAtqGameMode::BeginPlay() {
  Super::BeginPlay();

  player = Cast<AArktequeraCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
  //  player->MatCollect.AddDynamic(this, &AAtqGameMode::AddMaterial);
  player->interact->InteractEvent.AddDynamic(this, &AAtqGameMode::InteractAction);
 // player->buildMode->updateEvent.AddDynamic(this, &AAtqGameMode::InteractAction);
  inventory = player->myInventory;
  //Create Enemies

  Aarktquera_player_controller *player_controller = Cast<Aarktquera_player_controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  player_controller->Test1.AddDynamic(this, &AAtqGameMode::Test1);
  player_controller->crafting_event.AddDynamic(this, &AAtqGameMode::CheckRecipe);

  game_hud = Cast<AarktequeraHUD>(player_controller->GetHUD());
  player->buildMode->updateEvent.AddDynamic(game_hud, &AarktequeraHUD::UpdateInvetory);

  for (int i = 0; i < BP_Recipes.Num(); i++) {
   
    URecipe *newRecipe = NewObject<URecipe>(this, BP_Recipes[i]);
    newRecipe->CreateRecipe();
    Recipes.Add(newRecipe);

    //UActorComponent* NewComp = NewObject<UActorComponent>(Owner, ComponentClass);
    // TSubclassOf<URecipe> RecipeClass;
    //Recipes[i]->CreateRecipe();
    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::FromInt(Recipes[i]->amount_resources));
   // GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::FromInt(Recipes[i]->Recipe[0]->Amount()));
    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::FromInt(Recipes[i]->Recipe[0]->Type()));
  }
  FVector spawn_pos = { 0.0f, 0.0f, 0.0f };
  FTransform world_tr = FTransform(spawn_pos.Rotation(), spawn_pos, world_scale);

  GetWorld()->SpawnActorDeferred<AProceduralMap>(world_reference, world_tr);
  SpawnEnviromentalProps();
  SpawnDinos();
}

void AAtqGameMode::InteractAction(AActor* hit_actor, int dmg) {

  if (hit_actor->GetClass()->ImplementsInterface(UIDamagable::StaticClass())) {
    IIDamagable::Execute_NativeGetAttacked(hit_actor, dmg);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Interactuo");

    AEnviromentProp* myProp = Cast<AEnviromentProp>(hit_actor);
    //inventory->CheckResources();
    inventory->AddResources(myProp->type_, myProp->drop_*dmg);
    game_hud->UpdateInvetory(inventory->myResources);
    myProp->takeHealth(dmg);

  }
  
}


void AAtqGameMode::Test1(float x ){
  inventory->SubResources(0, 2);
  //int mollete = 0;
  game_hud->UpdateInvetory(inventory->myResources);
}


void AAtqGameMode::CheckRecipe(int recipe) {
  bool crafting_done = true;

  //int i_recipe = recipe;
 // GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::FromInt(recipe));
  for (int i = 0; i < Recipes[recipe]->Recipe.Num() && crafting_done; ++i) {

    crafting_done = crafting_done && inventory->IsResource(Recipes[recipe]->Recipe[i]->Type(), Recipes[recipe]->Recipe[i]->Amount());
  }

  if (crafting_done) {
    inventory->AddResources(Recipes[recipe]->type_out_resource, Recipes[recipe]->amount_out_resource);
    for (int i = 0; i < Recipes[recipe]->amount_resources ; ++i) {
      inventory->SubResources(Recipes[recipe]->Recipe[i]->Type(), Recipes[recipe]->Recipe[i]->Amount());
    }
    game_hud->UpdateInvetory(inventory->myResources);
  }

}


void AAtqGameMode::SpawnEnviromentalProps() {
  int i_stone = 0;
  int i_trees = 0;
  int x_range = world_scale.X * 100000;
  int y_range = world_scale.Y * 100000;
  float z_pos = 10000.0f;
  float delta_z_spawn_pos = (rand() % 101 / 100.0f);
  FVector spawn_rot = FVector(0.0f, 0.0f, 0.0f);
  //Stones
  while (i_stone < numberStones) {    
    FVector spawn_pos = FVector(rand() % x_range, rand() % y_range, z_pos);
    FVector end_pos = spawn_pos;
    end_pos.Z = -10.0f;
    FHitResult hit;
    if (GetWorld()->LineTraceSingleByChannel(hit, spawn_pos, end_pos, ECC_WorldStatic)) {
      spawn_pos = hit.Location;
      spawn_pos.Z += delta_z_spawn_pos;
      i_stone++;
      //SPAWN

      //GetWorld()->SpawnActor<AEnviromentProp>(Tree, spawn_pos, spawn_rot);
      FTransform spawn_tr = FTransform(spawn_rot.Rotation(), spawn_pos, FVector(1.0f,1.0f, 1.0f ));
      GetWorld()->SpawnActorDeferred<AEnviromentProp>(Stone, spawn_tr);
    }
  }

  while (i_trees < numberTrees) {
    FVector spawn_pos = FVector(rand() % x_range, rand() % y_range, z_pos);
    FVector end_pos = spawn_pos;
    end_pos.Z = -10.0f;
    FHitResult hit;
    if (GetWorld()->LineTraceSingleByChannel(hit, spawn_pos, end_pos, ECC_WorldStatic)) {
      spawn_pos = hit.Location;
      FTransform spawn_tr = FTransform(spawn_rot.Rotation(), spawn_pos, FVector(1.0f,1.0f, 1.0f + (rand()%101 /100.0f)));
      GetWorld()->SpawnActorDeferred<AEnviromentProp>(Tree, spawn_tr);
      i_trees++;
    }
  }
}

void AAtqGameMode::SpawnDinos(){

  int i_Dino = 0;
  int x_range = world_scale.X * 100000;
  int y_range = world_scale.Y * 100000;
  float z_pos = 10000.0f;
  float delta_z_spawn_pos = (rand() % 101 / 100.0f);
  FVector spawn_rot = FVector(0.0f, 0.0f, 0.0f);

  //Dinos
  while (i_Dino < numberDinos) {

    FVector spawn_pos = FVector(rand() % x_range, rand() % y_range, z_pos);
    FVector end_pos = spawn_pos;
    end_pos.Z = -10.0f;
    FHitResult hit;

    if (GetWorld()->LineTraceSingleByChannel(hit, spawn_pos, end_pos, ECC_WorldStatic)) {

      spawn_pos = hit.Location;
      spawn_pos.Z += delta_z_spawn_pos;
      ++i_Dino;
      
      //SPAWN            
      FTransform spawn_tr = FTransform(spawn_rot.Rotation(), spawn_pos, FVector(1.0f, 1.0f, 1.0f));
      GetWorld()->SpawnActorDeferred<AEnviromentProp>(Dinos[rand()%5], spawn_tr);
    }
  }

}