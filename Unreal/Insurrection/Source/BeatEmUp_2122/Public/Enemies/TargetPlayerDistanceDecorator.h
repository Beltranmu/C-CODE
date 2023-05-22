// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "TargetPlayerDistanceDecorator.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UTargetPlayerDistanceDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float distance = 80.0f;

	UTargetPlayerDistanceDecorator();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& treeComp, uint8* node) override;
	
};
