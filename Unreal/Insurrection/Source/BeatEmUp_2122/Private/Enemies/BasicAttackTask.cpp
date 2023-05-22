// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BasicAttackTask.h"
#include "Enemies/FinalBossController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Enemies/FinalBossCharacter.h"
#include "NavigationSystem.h"
#include "Player/TurboPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/GameInstanceBEU.h"

UBasicAttackTask::UBasicAttackTask()
{
}

EBTNodeResult::Type UBasicAttackTask::ExecuteTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node)
{
	
	AFinalBossController* controller = Cast<AFinalBossController>(ownerBTreeComp.GetAIOwner());
	UObject* selfActor = ownerBTreeComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(selfActor);
	if (boss) {
		boss->Attack();

	}
	FinishLatentTask(ownerBTreeComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

}
