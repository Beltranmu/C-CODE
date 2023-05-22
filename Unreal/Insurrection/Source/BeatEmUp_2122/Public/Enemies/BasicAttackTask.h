// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BasicAttackTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BEATEMUP_2122_API UBasicAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBasicAttackTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node) override;

};
