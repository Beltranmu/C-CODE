// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerInventory.generated.h"

class UResources;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARKTEQUERA_API UPlayerInventory : public UActorComponent
{
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  UPlayerInventory();
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources")
    TArray<UResources*> myResources;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  
  UFUNCTION()
    void AddResources(const int type, const float value);

  UFUNCTION()
    void CheckResources();

  UFUNCTION()
    void SubResources(const int type, const float value);

  UFUNCTION()
    bool IsResource(const int type, const float value);

};