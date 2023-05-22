// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/LaserAttackTask.h"
#include "Enemies/FinalBossCharacter.h"
#include "Enemies/FinalBossController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/GameInstanceBEU.h"
#include "Player/TurboPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TurboPlayer.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"


ULaserAttackTask::ULaserAttackTask() {

	NodeName = "Laser Attack";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

}

EBTNodeResult::Type ULaserAttackTask::ExecuteTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node)
{
	//Init variables
	myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));
		targetIndex = 0;
	if (myGI->numPlayers == 2) {
	
		bool player1Alive, player2Alive;
		player1Alive = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->alive;
		player2Alive = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1))->alive;
		if (player1Alive && player2Alive) {
			targetIndex = FMath::RandRange(0, 1);
		}
		else {
			if (player1Alive) {
				targetIndex = 0;
			}
			else {
				targetIndex = 1;
			}
		}

	}
	status = kStatus_LOCK_ON;
	boss = Cast<AFinalBossCharacter>(ownerBTreeComp.GetAIOwner()->GetPawn());
	//Rotate toward the players
	boss->rotationAngle = boss->laserAttack.dir == 1 ? 180 : 0;
	if (boss->rotationAngle != 0.0f) {
		boss->rotationAngle = 180.f;
		boss->GetMesh()->SetRelativeRotation(FQuat(FRotator(0.0f, -90.0f, 0.0f)));
	}
	else {
		boss->rotationAngle = 0.f;
		boss->GetMesh()->SetRelativeRotation(FQuat(FRotator(0.0f, 90.0f, 0.0f)));
	}

	//boss->SetActorRotation(FQuat(FRotator(0.0f, boss->rotationAngle, 0.0f)));
	collParams.ClearIgnoredActors();
	particleSpawnLocation = boss->GetActorLocation();
	particleSpawnLocation.Z += 70.0f;
	niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), lockBeam, particleSpawnLocation);
	
	boss->PlayAnimMontage(boss->laserAttack.anim);

	//Delay for first attack (so player can prepare properly)
	AFinalBossController* cont = Cast<AFinalBossController>(ownerBTreeComp.GetAIOwner());
	int lasersLeft = cont->blackboardComp->GetValueAsInt("LasersToFire");
	if (lasersLeft == boss->laserAttack.lasersToFire) {
		lockOnTime = firstLaserDelay + boss->laserAttack.lockOnTime;
	}
	else {
		lockOnTime = boss->laserAttack.lockOnTime;
	}

	return EBTNodeResult::InProgress;

}

void ULaserAttackTask::TickTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node, float DeltaSeconds)
{
	timer += DeltaSeconds;
	EBTNodeResult::Type result = EBTNodeResult::Type::InProgress;
	switch (status) {
	case kStatus_LOCK_ON:

		//Stuff
		targetPos = UGameplayStatics::GetPlayerCharacter(GetWorld(), targetIndex)->GetActorLocation();
		targetPos.Z = boss->GetActorLocation().Z;
		targetPos = boss->GetActorLocation() - targetPos;
		targetPos.Normalize();
		targetPos = boss->GetActorLocation() - (targetPos * 1000);
		niagaraComp->SetWorldRotation((targetPos - boss->GetActorLocation()).Rotation());
		niagaraComp->SetNiagaraVariableVec3(FString("EndPoint"), GetLockEndLocation());

		if (timer > lockOnTime) {
			status = kStatus_CHARGING;
			timer = 0.f;
		}

		break;
	case kStatus_CHARGING:

		
		niagaraComp->SetNiagaraVariableVec3(FString("EndPoint"), GetLockEndLocation());

		if (timer > boss->laserAttack.chargeTime) {
			status = kStatus_FIRING;
			timer = 0.f;
			niagaraComp->DestroyComponent();
			//niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), damageBeam, particleSpawnLocation);
			niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(damageBeam, Cast<USceneComponent>(boss->GetMesh()), FName("laserSocket"),
				FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.f),
				EAttachLocation::Type::KeepRelativeOffset, false);
			niagaraComp->SetUsingAbsoluteRotation(true);
			if (targetPos.Y < boss->GetActorLocation().Y) {
				niagaraComp->SetWorldRotation((boss->GetActorLocation() - targetPos).Rotation());
			}
			else {
				niagaraComp->SetWorldRotation((targetPos - boss->GetActorLocation()).Rotation());
			}
			boss->PlayAnimMontage(boss->laserAttack.anim,1.0f,FName("Release"));
		}
		break;
	case kStatus_FIRING:
		
		//Keep laser aiming for the point despite movement in bone
		FVector laserPos = boss->GetMesh()->GetSocketLocation(FName("laserSocket"));
		//We use same Z for everything as the laser should be straight
		laserPos.Z = boss->GetActorLocation().Z;
		if (targetPos.Y < boss->GetActorLocation().Y) {
			niagaraComp->SetWorldRotation((laserPos - targetPos).Rotation());
		}
		else {
			niagaraComp->SetWorldRotation((targetPos - laserPos).Rotation());
		}

		//make 2 raycast, one for each limit of the laser (left/right);
		//Get Vector between init point and target
		FVector aux = laserPos - targetPos;
		//Get perpendicular vector
		aux = FVector::CrossProduct(aux, FVector::UpVector);
		aux.Normalize();
		FVector target1, target2, startingPos1, startingPos2;

		startingPos1 = boss->GetMesh()->GetSocketLocation(FName("laserSocket")) + (aux * boss->laserAttack.width);
		startingPos2 = boss->GetMesh()->GetSocketLocation(FName("laserSocket")) - (aux * boss->laserAttack.width);
		startingPos1.Z  = particleSpawnLocation.Z;
		startingPos2.Z  = particleSpawnLocation.Z;

		target1 = targetPos + (aux * boss->laserAttack.width);
		target1.Z = particleSpawnLocation.Z;
		target2 = targetPos - (aux * boss->laserAttack.width);
		target2.Z = particleSpawnLocation.Z;

		TArray<FHitResult> hitActors, hitActors2;

		GetWorld()->LineTraceMultiByChannel(hitActors, startingPos1, target1, ECC_GameTraceChannel17, collParams);
		GetWorld()->LineTraceMultiByChannel(hitActors2, startingPos2, target2, ECC_GameTraceChannel17, collParams);

		for (int i= 0; i < hitActors.Num(); i++) {

			ATurboPlayer* playerChar = Cast<ATurboPlayer>(hitActors[i].GetActor());
			if (playerChar) {

				collParams.AddIgnoredActor(hitActors[i].GetActor());
				IIDamagable::Execute_TakeAHit(playerChar, FVector::ZeroVector, boss->laserAttack.stats, false, boss);

			}

		}
		for (int i = 0; i < hitActors2.Num(); i++) {

			ATurboPlayer* playerChar = Cast<ATurboPlayer>(hitActors2[i].GetActor());
			if (playerChar) {

				collParams.AddIgnoredActor(hitActors2[i].GetActor());
				IIDamagable::Execute_TakeAHit(playerChar, FVector::ZeroVector, boss->laserAttack.stats, false, boss);

			}

		}



		//Attack finished
		if (timer > boss->laserAttack.fireTime) {
			result = EBTNodeResult::Type::Succeeded;
			timer = 0.f;
			niagaraComp->DestroyComponent();
			boss->StopAnimMontage(boss->laserAttack.anim);
			AFinalBossController* cont = Cast<AFinalBossController>(ownerBTreeComp.GetAIOwner());
			int lasersLeft = cont->blackboardComp->GetValueAsInt("LasersToFire");
			lasersLeft--;
			cont->blackboardComp->SetValueAsInt("LasersToFire", lasersLeft);
		}

		break;
	}
	FinishLatentTask(ownerBTreeComp, result);
}

EBTNodeResult::Type ULaserAttackTask::PerformTask(UBehaviorTreeComponent& ownerBTreeComp, uint8* node, float DeltaSeconds)
{
	return EBTNodeResult::InProgress;
}

FVector ULaserAttackTask::GetLockEndLocation()
{
	
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(boss);
	GetWorld()->LineTraceSingleByChannel(hit, boss->GetActorLocation(), targetPos, ECollisionChannel::ECC_Visibility, params);
	if (hit.bBlockingHit) {
		//apply 40.0f as offset, otherwise ray stops at capsule before touching player
		return FVector(FVector::Dist(boss->GetActorLocation(), hit.ImpactPoint) + 40.0f, 0.0f, 0.0f);
	}

	return FVector(3000.0f, 0.0f, 0.0f);

}
