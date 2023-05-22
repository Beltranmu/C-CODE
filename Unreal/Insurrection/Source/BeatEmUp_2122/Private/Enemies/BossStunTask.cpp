// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BossStunTask.h"
#include "Enemies/FinalBossCharacter.h"
#include "Enemies/FinalBossController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/CustomAnim.h"

UBossStunTask::UBossStunTask() {

	NodeName = "Stun";
	bNotifyTaskFinished = true;
	bNotifyTick = true;
	bCreateNodeInstance = false;

}

EBTNodeResult::Type UBossStunTask::ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node)
{

	timer = 0.0f;
	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(treeComp.GetAIOwner()->GetPawn());
	boss->state.stun = true;
	boss->state.alteredStatus = true;
	stunTime =  boss->chargeAttack.vulnerabilityTime + boss->chargeAttack.dashDuration + boss->chargeAttack.dashDelay; 
	return EBTNodeResult::InProgress;

}

void UBossStunTask::TickTask(UBehaviorTreeComponent& treeComp, uint8* node, float DeltaTime)
{
	timer += DeltaTime;
	if (timer > stunTime) {

		Cast<AFinalBossCharacter>(treeComp.GetAIOwner()->GetPawn())->resetStateStats();
		FinishLatentTask(treeComp, EBTNodeResult::Type::Succeeded);
	}

	FinishLatentTask(treeComp, EBTNodeResult::Type::InProgress);

}

EBTNodeResult::Type UBossStunTask::PerformTask(UBehaviorTreeComponent& treeComp, uint8* node, float DeltaSeconds)
{
	return EBTNodeResult::InProgress;
}
