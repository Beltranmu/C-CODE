// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SelectChargePointTask.h"
#include "Enemies/FinalBossController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Enemies/FinalBossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Enemies/PointArrayActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/GameInstanceBEU.h"

USelectChargePointTask::USelectChargePointTask() {

	NodeName = "Select Charge/Laser Point";

}

EBTNodeResult::Type USelectChargePointTask::ExecuteTask(UBehaviorTreeComponent& treeComp, uint8* node)
{
	
	AFinalBossController* controller = Cast<AFinalBossController>(treeComp.GetAIOwner());
	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(controller->GetPawn());
	APointArrayActor* points = nullptr;
	int dir = 1;
	float distance = 100000.0f;
	if (selectLaserPoint) {
		if (boss->laserAttackBossLocation)
			points = boss->laserAttackBossLocation;
	}
	else {
		if (boss->chargeAttackLocation)
			points = boss->chargeAttackLocation;

	}
	if (points) {
		//Select nearest point
		for (int i = 0; i < points->pointArray.Num(); i++) {

			FVector worldPoint = points->pointArray[i];
			worldPoint = points->GetTransform().TransformPosition(worldPoint);

			if (distance > FVector::Distance(boss->GetActorLocation(), worldPoint)) {

				distance = FVector::Distance(boss->GetActorLocation(), worldPoint);
				controller->blackboardComp->SetValueAsVector("TargetLocation", worldPoint);
				FVector initialPosition = controller->blackboardComp->GetValueAsVector(FName("InitPosition"));
				if (worldPoint.Y < initialPosition.Y) {
					dir = -1;
				}
				else {
					dir = 1;
				}

			}

		}

		if (selectLaserPoint) {
			boss->laserAttack.dir = dir;
			controller->blackboardComp->SetValueAsInt("LasersToFire", boss->laserAttack.lasersToFire);
			
		}
		else {
			boss->chargeAttack.dir = dir;
		}

		FinishLatentTask(treeComp, EBTNodeResult::Type::Succeeded);
		return EBTNodeResult::Type::Succeeded;
	}
	else {

		FinishLatentTask(treeComp, EBTNodeResult::Type::Failed);
		return EBTNodeResult::Type::Failed;
	}

}
