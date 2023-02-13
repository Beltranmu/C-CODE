// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnviromentProp.generated.h"

class UHealthComponent;

UCLASS()
class ARKTEQUERA_API AEnviromentProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnviromentProp();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* mesh;
  UPROPERTY(EditDefaultsOnly, Category = "Type")
    uint32 type_;
  UPROPERTY(EditDefaultsOnly, Category = "Type")
    uint32 drop_;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    UHealthComponent* health_;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UFUNCTION()
    void takeHealth(const int dmg);

};
