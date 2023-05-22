// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Pawn.h"
#include "Props/BossMissile.h"
#include "MissileAttackTask.generated.h"

//Well, this struct and playing with memory nodes was unnecessary, but at least i've learnt something
//and life is about learning things everyday.
struct MissileAttackMemory {
	float timer = 0.f; 
};

/**
 * 
 */
UCLASS(Blueprintable)
class BEATEMUP_2122_API UMissileAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Spawn")
		TSubclassOf<ABossMissile> classToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		UMaterialInterface* decalMaterial;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		float zOffset = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		float timeBetweenMissile = 0.15f;

	UMissileAttackTask();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node) override;
	
	void TickTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node, float DeltaSeconds) override;

	uint16 GetInstanceMemorySize() const override;

	EBTNodeResult::Type PerformTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node, float DeltaSeconds);
	

};
