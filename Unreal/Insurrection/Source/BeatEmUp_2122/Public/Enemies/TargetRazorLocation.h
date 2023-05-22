// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TargetRazorLocation.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UTargetRazorLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UTargetRazorLocation();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node) override;

};
