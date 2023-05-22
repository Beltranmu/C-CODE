// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BossAttackNotify.generated.h"

UENUM()
enum AttackType {
	kBasicAttack = 0,
	kChargeAttack,
	kRazorAttack,
};

UCLASS()
class BEATEMUP_2122_API UBossAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TEnumAsByte<AttackType> attackType;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
