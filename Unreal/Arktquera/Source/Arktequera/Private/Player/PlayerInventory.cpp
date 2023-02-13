// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Public/Player/PlayerInventory.h"
#include "Engine/Engine.h"
#include <Framework/Resources.h>

//#include "../../Public/Framework/Resources.h"
// Sets default values for this component's properties
UPlayerInventory::UPlayerInventory()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  // ...



}

// Called when the game starts
void UPlayerInventory::BeginPlay()
{
  Super::BeginPlay();
  // resources.stone = 0;
   //resources.wood = 0;
     // ...



}

// Called every frame
void UPlayerInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void UPlayerInventory::AddResources(const int type, const float value) {
  
  int i = 0;
  bool indexFound = false;

  int32 myResourcesLength = myResources.Num();

  // Temporal UResource variable to add in myResources array
  UResources* tmpResource = NewObject<UResources>();
  tmpResource->SetType(type);
  tmpResource->SetAmount(value);

  if (myResourcesLength == 0){  // Array Empty

    myResources.Add(tmpResource);
    
  }else {  // There's at least one node in myResources array
    
    do {
      
      if (myResources[i]->Type() == type) {

        myResources[i]->SetAmount(value);
        indexFound = true;        
        
      }

      ++i;
      
    } while (i < myResourcesLength && !indexFound);

    if (false == indexFound) {

      myResources.Add(tmpResource);

    }
    
  }

}

void UPlayerInventory::CheckResources(){
  for (int i = 0; i < myResources.Num(); ++i) {
    if (myResources[i]->Amount() <= 0){
      myResources.RemoveAt(i);
    }
  }
}

void UPlayerInventory::SubResources(const int type, const float value){

  for (int i = 0; i < myResources.Num(); ++i) {

    if (myResources[i]->Type() == type) {
      myResources[i]->SetAmount( -value);
      if (myResources[i]->Amount() <= 0) {
        myResources.RemoveAt(i);
      }
    }

    
  }
}

bool UPlayerInventory::IsResource(const int type, const float amount) {

  for (int i = 0; i < myResources.Num(); ++i) {

    if (myResources[i]->Type() == type && myResources[i]->Amount() >= amount) {
      return true;
    }
  }
  return false;
}

