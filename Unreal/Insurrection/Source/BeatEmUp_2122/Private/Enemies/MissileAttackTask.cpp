// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/MissileAttackTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/FinalBossController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Enemies/PointArrayActor.h"
#include "Enemies/FinalBossCharacter.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

UMissileAttackTask::UMissileAttackTask(){
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	NodeName = "MissileAttack";
}

EBTNodeResult::Type UMissileAttackTask::ExecuteTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node)
{

	MissileAttackMemory* myMemory = reinterpret_cast<MissileAttackMemory*>(node);
	if (myMemory->timer > timeBetweenMissile) {
		myMemory->timer = 0.f;
		//get AI controller & agent
		UObject* selfActor = ownerBTreeComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
		AFinalBossCharacter* agent = Cast<AFinalBossCharacter>(selfActor);
		AFinalBossController* controller = Cast<AFinalBossController>(ownerBTreeComp.GetAIOwner());
		int index = ownerBTreeComp.GetBlackboardComponent()->GetValueAsInt(FName("ArrayIndex"));

		if (agent && controller) {
			if (agent->missileAttackLocations) {

				if(index <= agent->missileAttackLocations->pointArray.Num() -1) {

					FActorSpawnParameters spawnParams;
					FVector spawnLocation = agent->missileAttackLocations->pointArray[index];
					spawnLocation = agent->missileAttackLocations->GetActorTransform().TransformPosition(spawnLocation);
					ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(spawnLocation, FRotator());
					if (decal) {
						decal->SetDecalMaterial(decalMaterial);
						decal->SetLifeSpan(1.75f);
						decal->GetDecal()->DecalSize = FVector(32.0f, 32.0f, 20.0f);
					}
					spawnLocation.Z += zOffset;
					ABossMissile* spawnedMissile = Cast<ABossMissile>(GetWorld()->SpawnActor<AActor>(classToSpawn, spawnLocation, FRotator::ZeroRotator, spawnParams));
					spawnedMissile->limitZAxisHeight = spawnLocation.Z - zOffset;
					agent->missileAttackLocations->pointArray[index];
					index++;
					if (index == agent->missileAttackLocations->pointArray.Num()) {
						index = 0;
						ownerBTreeComp.GetBlackboardComponent()->SetValueAsInt(FName("ArrayIndex"), index);
						agent->StopAnimMontage(agent->missileAttack.missileAttackAnim);
						return EBTNodeResult::Type::Succeeded;
					}
					else {
						ownerBTreeComp.GetBlackboardComponent()->SetValueAsInt(FName("ArrayIndex"), index);
						return EBTNodeResult::Type::InProgress;
					}

				}
				else {
					return EBTNodeResult::Type::Aborted;
				}
			}
		}

		return EBTNodeResult::Type::Failed;

	}

	return EBTNodeResult::Type::InProgress;
	
}

void UMissileAttackTask::TickTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node, float DeltaSeconds)
{
	//Super::TickTask(ownerBTreeComp,node,DeltaSeconds);

	MissileAttackMemory* myMemory = reinterpret_cast<MissileAttackMemory*>(node);
	myMemory->timer += DeltaSeconds;
	
	EBTNodeResult::Type result = ExecuteTask(ownerBTreeComp, node);
	if (result != EBTNodeResult::Type::InProgress) {
	}
	FinishLatentTask(ownerBTreeComp, result);
}

uint16 UMissileAttackTask::GetInstanceMemorySize() const
{
	return sizeof(MissileAttackMemory);
}

EBTNodeResult::Type UMissileAttackTask::PerformTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node, float DeltaSeconds)
{
	return EBTNodeResult::InProgress;
}
