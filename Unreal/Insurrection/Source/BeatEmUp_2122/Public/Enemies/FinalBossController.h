// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FinalBossController.generated.h"

/**
 * 
 */

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class BEATEMUP_2122_API AFinalBossController : public AAIController
{
	GENERATED_BODY()

public:
	AFinalBossController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		UBehaviorTreeComponent* bTreeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		UBlackboardComponent* blackboardComp;

	virtual void OnPossess(APawn* p) override;

	virtual void OnUnPossess() override;

	
};
