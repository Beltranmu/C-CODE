// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Framework/Recipe.h"
#include "../../Public/Framework/Resources.h"

URecipe::URecipe() {
  

}

void URecipe::CreateRecipe() {
 
 // Recipe.Empty();
  //UResources new_resource;
  for (int i = 0; i < amount_resources; ++i) {
    UResources* new_resource = NewObject<UResources>();
    new_resource->SetAmountValue(Recipe_amount[i]);
    new_resource->SetType(Recipe_type[i]);
    Recipe.Add(new_resource);
  }
}

URecipe::~URecipe() {
  
  

}