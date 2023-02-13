// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Resources.generated.h"

/**
 * 
 */
UCLASS()
class ARKTEQUERA_API UResources : public UObject
{
	GENERATED_BODY()
  
  // Identifies what sort of resources has been harvested  

  public:

    UResources();

    enum ResourceType {

      kType_Wood = 0,
      kType_Stone,

      kType_Wooden_Foundation,
      kType_Wooden_Wall,
      kType_Wooden_Celling,
      kType_Wooden_Ramp,
      kType_Wooden_Door_Frame,

      kType_Spear,

      kType_Berries,
      kType_Meat,

    };

    UResources* Init();
    UResources* Init(const int type, const float value);

    // Amount Variable - Setter & Getter
    void SetType(int value);
    int Type();

    // Amount Variable - Setter & Getter
    void SetAmount(float value);
    void SetAmountValue(float value);
    float Amount();

    //UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Inv")
  private:
    float amount_;

    int type_;
	
};
