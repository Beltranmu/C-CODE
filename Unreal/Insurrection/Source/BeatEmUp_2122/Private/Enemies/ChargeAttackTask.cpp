// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/ChargeAttackTask.h"
#include "Enemies/FinalBossController.h"
#include "Enemies/FinalBossCharacter.h"
#include "NavigationSystem.h"
#include "Player/TurboPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/GameInstanceBEU.h"

UChargeAttackTask::UChargeAttackTask() {

	NodeName = "Charge Attack";

}

EBTNodeResult::Type UChargeAttackTask::ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node)
{

	AFinalBossController* controller = Cast<AFinalBossController>(treeComp.GetAIOwner());
	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(controller->GetPawn());
	if (boss) {
		boss->ChargeAttack();
	}
	FinishLatentTask(treeComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

}
