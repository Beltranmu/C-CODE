// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteract, AActor*, Actor_material, int, dmg);

//class UBoxComponent;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARKTEQUERA_API UInteractComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  UInteractComponent();

  /* UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction_settings")
     UBoxComponent* Intreac_Area;*/

  UPROPERTY(BlueprintAssignable, Category = "Gameplay")
    FInteract InteractEvent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction_settings")
    bool can_interact;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction_settings")
    float interact_cooldown_timer;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction_settings")
    float interact_cooldown;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction_settings")
    float interaction_lenght;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction_settings")
    FVector Interact_pos;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coin")
    FTimerHandle Interact_timer;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  UFUNCTION()
    bool CanInteract(FVector v_direction);
  UFUNCTION()
    void InteractTimer();
  UFUNCTION()
    void EVInteract(FVector v_direction); // Enviroment Interaction

};
