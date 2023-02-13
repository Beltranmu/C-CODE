// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Structure.generated.h"

UCLASS()
class ARKTEQUERA_API AStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStructure();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* mesh;
  UPROPERTY(EditAnywhere, Category = "Mesh")
    uint32 type;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UFUNCTION()
    void ChangeCollisionPreset();
  UFUNCTION()
    uint32 GetType();
  UFUNCTION()
    void SetMaterial(uint32 index, UMaterial* material);
};
