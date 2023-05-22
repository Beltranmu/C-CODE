// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BossAttackNotify.h"
#include "Enemies/FinalBossCharacter.h"
#include "Player/CustomAnim.h"

void UBossAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AFinalBossCharacter* boss = Cast<AFinalBossCharacter>(MeshComp->GetOwner());

	if (boss) {

		switch (attackType) {
		case kBasicAttack:
		case kChargeAttack:
			boss->Dash(boss->GetActorForwardVector());

		break;
		case kRazorAttack:
			boss->RazorAttack();
		break;
		}

	}

}
