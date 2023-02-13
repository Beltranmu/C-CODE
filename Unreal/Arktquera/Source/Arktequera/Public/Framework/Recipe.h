// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Recipe.generated.h"

/**
 * 
 */
class UResources;
UCLASS(Blueprintable)
class ARKTEQUERA_API URecipe : public UObject
{
  GENERATED_BODY()
public:

    URecipe();
    ~URecipe();
    //REcipe outcome
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
      int32 type_out_resource;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
      int32 amount_out_resource;
    
    //Recipe details
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
     int32 amount_resources;
  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
    TArray<int32> Recipe_type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
    TArray<int32> Recipe_amount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
      TArray<UResources*>Recipe;


    UFUNCTION()
      void CreateRecipe();
};
