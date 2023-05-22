// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/UpdatePhaseStatsTask.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/FinalBossCharacter.h"
#include "Enemies/FinalBossController.h"

UUpdatePhaseStatsTask::UUpdatePhaseStatsTask() {

	NodeName = "UpdatePhase";

}

EBTNodeResult::Type UUpdatePhaseStatsTask::ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node)
{

	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(treeComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* blackboard = treeComp.GetBlackboardComponent();

	blackboard->SetValueAsInt(FName("currentPhase"), boss->currentPhase);

	return EBTNodeResult::Type::Succeeded;
}
