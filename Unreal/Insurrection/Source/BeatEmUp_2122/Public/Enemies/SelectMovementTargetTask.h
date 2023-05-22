// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SelectMovementTargetTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BEATEMUP_2122_API USelectMovementTargetTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float targetPlayerPercentage = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float randomTargetRange = 550.0f;

	USelectMovementTargetTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node) override;

};
