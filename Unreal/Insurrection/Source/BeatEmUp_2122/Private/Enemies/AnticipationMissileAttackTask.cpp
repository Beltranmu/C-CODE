// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AnticipationMissileAttackTask.h"
#include "Enemies/FinalBossCharacter.h"
#include "Enemies/FinalBossController.h"
#include "BehaviorTree/BlackboardComponent.h"

UAnticipationMissileAttackTask::UAnticipationMissileAttackTask() {

	NodeName = "Missile attack anticipation";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

}

void UAnticipationMissileAttackTask::TickTask(UBehaviorTreeComponent& treeComp, uint8* node, float deltaTime)
{

	timer += deltaTime;
	
	if (timer > delayTime) {
		FinishLatentTask(treeComp, EBTNodeResult::Succeeded);
	}

	FinishLatentTask(treeComp, EBTNodeResult::InProgress);

}

EBTNodeResult::Type UAnticipationMissileAttackTask::ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node)
{

	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(treeComp.GetAIOwner()->GetPawn());
	timer = 0.0f;
	delayTime = boss->missileAttack.delayToStart;
	boss->PlayAnimMontage(boss->missileAttack.missileAttackAnim);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UAnticipationMissileAttackTask::PerformTask(UBehaviorTreeComponent& treeComp, uint8* node, float DeltaSeconds)
{
	return EBTNodeResult::InProgress;
}
