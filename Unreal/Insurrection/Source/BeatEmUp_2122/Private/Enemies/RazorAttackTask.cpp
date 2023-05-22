// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/RazorAttackTask.h"
#include "Enemies/FinalBossController.h"
#include "Enemies/FinalBossCharacter.h"

URazorAttackTask::URazorAttackTask() {

	NodeName = "Razor Attack";
	
}

EBTNodeResult::Type URazorAttackTask::ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node) {


	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(treeComp.GetAIOwner()->GetPawn());


	if (boss) {

		boss->PlayAnimMontage(boss->razorAttackAnim);

	}

	FinishLatentTask(treeComp, EBTNodeResult::Type::Succeeded);
	return EBTNodeResult::Type::Succeeded;

}