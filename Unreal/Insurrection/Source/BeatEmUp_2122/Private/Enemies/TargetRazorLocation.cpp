// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/TargetRazorLocation.h"
#include "Enemies/FinalBossController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Enemies/FinalBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UTargetRazorLocation::UTargetRazorLocation() {

    NodeName = "Target Razor Attack Location";

}

EBTNodeResult::Type UTargetRazorLocation::ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node)
{

    AFinalBossController* cont = Cast<AFinalBossController>(treeComp.GetAIOwner());
    AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(cont->GetPawn());

    cont->blackboardComp->SetValueAsVector("TargetLocation", boss->razorAttackBossLocation);
    

    return EBTNodeResult::Type::Succeeded;
}
