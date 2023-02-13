// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Materials/Material.h"
#include "BuildComponent.generated.h"

class AStructure;
class UPlayerInventory;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInv, TArray<UResources*>, invetory);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARKTEQUERA_API UBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildComponent();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "B_settings")
    float build_height;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "B_settings")
    float build_distance;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "B_settings")
    int32 max_build_comp;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "B_settings")
    int32 structure_type;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "B_settings")
    FVector build_location;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "B_settings")
    FVector no_build_location;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "B_settings")
    FRotator build_rotation;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "B_settings")
    bool spawned;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "B_settings")
    bool isSpawnable;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "B_settings")
    TArray <AStructure*> preview_structure;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "B_settings")
   TArray <TSubclassOf <AStructure>> new_structure;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "B_Settings")
    UMaterial* placeableMat;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "B_Settings")
    UMaterial* notPlaceableMat;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Invetory")
    UPlayerInventory* player_inv_ref;

  UPROPERTY()
    FUpdateInv updateEvent;

  float buildLocationZ;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  UFUNCTION()
    void WhereToBuild(FVector v_direction);
  UFUNCTION()
    void SpawnStructure();
  UFUNCTION()
    void ChangePreview();

  UFUNCTION()
    void ErasePreview();

  UFUNCTION()
    void SetPlayerInvRef(UPlayerInventory* ref);

  UFUNCTION()
    int invID2BuildID(int32 build_type);

  
};

