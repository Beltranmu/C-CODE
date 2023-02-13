// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Enviroment/EnviromentProp.h"
#include <Player/HealthComponent.h>
#include <Engine/Engine.h>

// Sets default values
AEnviromentProp::AEnviromentProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  mesh = CreateDefaultSubobject<UStaticMeshComponent>("mesh");
  RootComponent = mesh;

  health_ = CreateDefaultSubobject<UHealthComponent>("Health");
  health_->setHealth(5 + rand()%6);
  
  drop_ = 1 + rand() % 6;

}

// Called when the game starts or when spawned
void AEnviromentProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnviromentProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnviromentProp::takeHealth(const int dmg){
  
  if (!health_->CanIDie()) {

    health_->setHealth(health_->Health() - dmg);

  }else{

    Destroy();

  }

}
