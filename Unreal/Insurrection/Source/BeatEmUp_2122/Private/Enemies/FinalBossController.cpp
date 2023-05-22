// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/FinalBossController.h"
#include "Enemies/FinalBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AFinalBossController::AFinalBossController() {

	bTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behaviour Tree"));
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

}



void AFinalBossController::OnPossess(APawn* p)
{

	Super::OnPossess(p);

	if (IsValid(blackboardComp)) {

		AFinalBossCharacter* agent = Cast<AFinalBossCharacter>(p);
		if (agent) {

			//Check just in case no behavior Tree was assigned to the blueprint
			if (IsValid(agent->bTree)) {

				if (IsValid(agent->bTree->BlackboardAsset)) {

					blackboardComp->InitializeBlackboard(*(agent->bTree->BlackboardAsset));
			
					agent->blackboardComp = blackboardComp;
					agent->blackboardComp->SetValueAsInt(FName("ArrayIndex"), 0);
					agent->blackboardComp->SetValueAsObject(FName("SelfActor"), p);
					agent->blackboardComp->SetValueAsVector(FName("InitPosition"), p->GetActorLocation());
				}
		
			}

			//Init variable values


			bTreeComp->StartTree(*(agent->bTree));

		}
	}


}

void AFinalBossController::OnUnPossess()
{

	//Stop the Behavior Tree when posses is finished 
	bTreeComp->StopTree();

}
