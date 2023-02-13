// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Structure/Structure.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AStructure::AStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
  RootComponent = mesh;

}

// Called when the game starts or when spawned
void AStructure::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStructure::ChangeCollisionPreset() {

  mesh->SetCollisionProfileName("CheckStructuresWithSphere");

}

uint32 AStructure::GetType() {
  return type;
}

void AStructure::SetMaterial(uint32 index, UMaterial* material) {
  
  mesh->SetMaterial(index, material);
}
