// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ToggleLightNotify.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum LightType
{
  HAND    UMETA(DisplayName = "HAND"),
  HEAD    UMETA(DisplayName = "HEAD"),
  ROOT    UMETA(DisplayName = "ROOT")
};

UCLASS()
class BEATEMUP_2122_API UToggleLightNotify : public UAnimNotify
{
	GENERATED_BODY()

	public:
		
		UPROPERTY(EditAnywhere, Category = "Light Type")
			TEnumAsByte<LightType> lightType;

	public:

		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
