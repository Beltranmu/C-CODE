// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SelectChargePointTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BEATEMUP_2122_API USelectChargePointTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool selectLaserPoint;

	USelectChargePointTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node) override;
	
};
