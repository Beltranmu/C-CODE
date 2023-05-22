// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/TargetPlayerDistanceDecorator.h"
#include "Enemies/FinalBossController.h"
#include "Enemies/FinalBossCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/TurboPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstanceBEU.h"


UTargetPlayerDistanceDecorator::UTargetPlayerDistanceDecorator() {

	NodeName = "Check player distance";
	bNotifyBecomeRelevant = true;

}

void UTargetPlayerDistanceDecorator::OnBecomeRelevant(UBehaviorTreeComponent& treeComp, uint8* node)
{

	AFinalBossController* cont = Cast<AFinalBossController>(treeComp.GetAIOwner());
	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(cont->GetPawn());

	ATurboPlayer* player = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UGameInstanceBEU* myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (myGI->numPlayers == 2) {

	}
		//TODO


}
