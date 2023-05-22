// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BossStunTask.generated.h"

/**
 * 
 */
class AFinalBossCharacter;

UCLASS()
class BEATEMUP_2122_API UBossStunTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	//We are not using node so this timer is common for all tasks (should be irrelevant as
	//it is a boss and there is only one).
	UPROPERTY(VisibleAnywhere)
	float timer = 0.0f;
	UPROPERTY(VisibleAnywhere)
	float stunTime;

	UBossStunTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node) override;

	void TickTask(UBehaviorTreeComponent& treeComp, uint8* node, float DeltaTime) override;

	EBTNodeResult::Type PerformTask(UBehaviorTreeComponent& treeComp, uint8* node, float DeltaSeconds);
	
};
