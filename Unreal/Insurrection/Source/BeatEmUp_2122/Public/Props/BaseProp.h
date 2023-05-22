// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/IDamagable.h"

#include "BaseProp.generated.h"


UENUM()
namespace ESpanwObjectTypes {
  enum Type {
    SO_NONE = 0			   UMETA(DisplayName = "NONE"),
    SO_APPLE = 1			 UMETA(DisplayName = "Apple"),
    SO_CHICKEN = 2	   UMETA(DisplayName = "Chicken"),
    SO_FULL_CHK = 3	   UMETA(DisplayName = "KFC MENU")
  };
}

/** The type of object the interactable is */


class UStaticMeshComponent;
UCLASS()
class BEATEMUP_2122_API ABaseProp : public AActor, public IIDamagable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* body;

	//Type spawn when destroy
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "INTERACTABLE")
    TEnumAsByte<ESpanwObjectTypes::Type> ObjectToSpawn;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
