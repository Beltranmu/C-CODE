// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SelectMovementTargetTask.h"
#include "Enemies/FinalBossController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Enemies/FinalBossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Player/TurboPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/GameInstanceBEU.h"
#include "Enemies/FinalBossCharacter.h"

USelectMovementTargetTask::USelectMovementTargetTask() {

}

EBTNodeResult::Type USelectMovementTargetTask::ExecuteTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node)
{

	//Only works for player 1, once it works properly will test player 1 & 2
	ATurboPlayer* player = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()))->numPlayers > 1) {

		if (FMath::RandRange(0.0f, 100.0f) < 50.0f) {

			player = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1));
			if (!player->alive) {
				player = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			}

		}

	}
	AFinalBossController* controller = Cast<AFinalBossController>(ownerBTreeComp.GetAIOwner());
	FVector newLocation;

	if (FMath::RandRange(0.0f, 100.0f) < targetPlayerPercentage) {

		newLocation = player->GetActorLocation();
		controller->blackboardComp->SetValueAsVector("TargetLocation", newLocation);
		AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(controller->GetPawn());
		boss->targetPlayerID = player->characterID;

	}
	else {

		AFinalBossCharacter* self = Cast<AFinalBossCharacter>(controller->blackboardComp->GetValueAsObject("SelfActor"));
		newLocation = UNavigationSystemV1::GetRandomPointInNavigableRadius(this->GetWorld(),self->GetActorLocation(), randomTargetRange);

	}

	controller->blackboardComp->SetValueAsVector("TargetLocation", newLocation);


	FinishLatentTask(ownerBTreeComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

}
