// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RazorAttackTask.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API URazorAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	URazorAttackTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node) override;
	
};
