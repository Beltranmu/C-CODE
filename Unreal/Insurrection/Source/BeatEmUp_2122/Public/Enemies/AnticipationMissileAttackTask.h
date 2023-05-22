// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AnticipationMissileAttackTask.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UAnticipationMissileAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	//We are not using node so this timer is common for all tasks (should be irrelevant as
	//it is a boss and there is only one).
	UPROPERTY(VisibleAnywhere)
		float timer = 0.0f;
	UPROPERTY(VisibleAnywhere)
		float delayTime;

	UAnticipationMissileAttackTask();

	void TickTask(UBehaviorTreeComponent& treeComp, uint8* node, float deltaTime) override;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node) override;

	EBTNodeResult::Type PerformTask(UBehaviorTreeComponent& treeComp, uint8* node, float DeltaSeconds);
	
};

