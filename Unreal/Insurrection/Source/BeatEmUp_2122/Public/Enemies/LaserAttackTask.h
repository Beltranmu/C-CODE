// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "LaserAttackTask.generated.h"

class AFinalBossCharacter;
class UGameInstanceBEU;
class UNiagaraSystem;
class UNiagaraComponent;

UENUM()
enum LaserTaskStatus {

	kStatus_LOCK_ON = 0,
	kStatus_CHARGING,
	kStatus_FIRING

};

UCLASS(Blueprintable)
class BEATEMUP_2122_API ULaserAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	//These will be common to all tasks, need to have this in mind
	TEnumAsByte<LaserTaskStatus> status = kStatus_LOCK_ON;

	float timer;

	UPROPERTY(EditAnywhere, Category = "Config")
	int targetIndex;
	UPROPERTY(EditAnywhere, Category = "Config")
	float firstLaserDelay = 1.0f;
	AFinalBossCharacter* boss;
	UGameInstanceBEU* myGI;
	FVector targetPos;
	FCollisionQueryParams collParams;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	UNiagaraSystem* lockBeam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	UNiagaraSystem* damageBeam;
	UNiagaraComponent* niagaraComp;
	FVector particleSpawnLocation;
	float lockOnTime;

	ULaserAttackTask();
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node) override;

	void TickTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node, float DeltaSeconds) override;

	EBTNodeResult::Type PerformTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node, float DeltaSeconds);

	FVector GetLockEndLocation();

};
