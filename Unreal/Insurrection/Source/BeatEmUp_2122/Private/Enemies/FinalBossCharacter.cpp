// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/FinalBossCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemies/PointArrayActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Props/BossRazor.h"
#include "Player/CustomAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstanceBEU.h"
#include "Enemies/SpawnTrigger.h"
#include "Enemies/EnemyCommander.h"
#include "Enemies/MasillaEnemy.h"
#include "Enemies/SpawnPoint.h"


// Sets default values
AFinalBossCharacter::AFinalBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFinalBossCharacter::BeginPlay()
{
	Super::BeginPlay();

    enemyCommander = Cast<AEnemyCommander>(UGameplayStatics::GetActorOfClass(GetWorld(), enemyCommanderClass));
    fade = 1.0f;

    materialCharacter2ElectricBoogaloo = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);
    GetMesh()->SetMaterial(1, materialCharacter2ElectricBoogaloo);

    myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

    for (int i = 0; i < spawnTriggers.Num(); i++) {

        spawnTriggers[i]->lockConfig = flightBossCameraConfig;

    }

    if (myGI->numPlayers < 1) {
        health *= 1.5f;
        maxHealth *= 1.5f;
    }

}

// Called every frame
void AFinalBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (canMove && !state.alteredStatus && !state.hit && !flying) {
		const FVector speed = GetVelocity();
		if (speed.Y > 0) {
			rotationAngle = 0;
            GetMesh()->SetRelativeRotation(FQuat(FRotator(0.0f, 90.0f, 0.0f)));
		}
		if (speed.Y < 0) {
			rotationAngle = 180;
            GetMesh()->SetRelativeRotation(FQuat(FRotator(0.0f, -90.0f, 0.0f)));
		}
	}

    //Fly on Z axis until correct height (up or down)
    float targetHeight;
    timer += DeltaTime;
    if (timer > flightDelay) {

        if (flying) {
            targetHeight = flyingHeight;
        }
        else {
            targetHeight = groundHeight;
        }
        FVector meshLocation = GetMesh()->GetRelativeLocation();
        float meshHeight = FMath::Lerp<float>(meshLocation.Z, targetHeight, kFlyingUpLerp * DeltaTime);
        meshLocation.Z = meshHeight;
        meshLocation.X = 0.f;
        meshLocation.Y = 0.f;
        GetMesh()->SetRelativeLocation(meshLocation);
    
    }

    CheckPhaseChange();
 
}

// Called to bind functionality to input
void AFinalBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFinalBossCharacter::Attack()
{

	PlayAnimMontage(basicAttack.anim);
    UpdateDashStats(basicAttack.dashDuration, basicAttack.dashSpeed);

}

void AFinalBossCharacter::Dash(FVector dir)
{

    if (canMove) {

        canMove = false;
        dashing = true;
        canDash = false;
        canJumpChar = false;

        float aux = dir.X;
        dir.X = dir.Y;
        dir.Y = aux;
        dir.Z = 0.0f;

        dir.Normalize();
        dir *= dashSpeed;

        LaunchCharacter(dir, true, false);
        GetCharacterMovement()->GroundFriction = 0.0f;
        GetCharacterMovement()->GravityScale = 0.0f;
        GetCapsuleComponent()->SetPhysicsLinearVelocity(dir, true);

        //GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterDashBehaviour"));
        GetWorldTimerManager().SetTimer(dashTimer, this, &ABaseCharacter::StopDash, dashDuration, false);
        dashContinueTimerDelegate.BindUFunction(this, FName("continueDash"), dir);
        GetWorldTimerManager().SetTimer(dashContinue, dashContinueTimerDelegate, 0.1f, true);
        
    }

}

void AFinalBossCharacter::StopDash()
{

    canMove = true;
    dashing = false;
    canJumpChar = true;
    //Won't stop without a launch character, it works so...
    LaunchCharacter(FVector(0.5f, 0.5f, 0.0f), true, false);
    GetCharacterMovement()->GroundFriction = 8.0f;
    GetCharacterMovement()->GravityScale = 1.0f;
    GetCapsuleComponent()->SetCollisionProfileName("Enemy");
    GetWorld()->GetTimerManager().ClearTimer(dashContinue);
    if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(chargeAttack.anim)) {
        StopAnimMontage(chargeAttack.anim);
    }

}

void AFinalBossCharacter::UpdateDashStats(float duration, float speed)
{

    dashDuration = duration;
    dashSpeed = speed;
    
}

void AFinalBossCharacter::ChargeAttack()
{

    //TODO: this may break when using final art, maybe mesh should be scaled too
    rotationAngle = chargeAttack.dir == 1 ? 180 : 0;
    if (rotationAngle != 0.0f) {
        rotationAngle = 180.f;
        GetMesh()->SetRelativeRotation(FQuat(FRotator(0.0f, -90.0f, 0.0f)));
    }
    else {
        rotationAngle = 0.f;
        GetMesh()->SetRelativeRotation(FQuat(FRotator(0.0f, 90.0f, 0.0f)));
    }
    //Attack functionality here, uwu
    PlayAnimMontage(chargeAttack.anim);
    UpdateDashStats(chargeAttack.dashDuration, chargeAttack.dashSpeed);    

}

void AFinalBossCharacter::RazorAttack()
{

    int maxIndex;

    switch (currentPhase) {
    case kPhase1:
        maxIndex = 2;
        break;
    case kPhase2:
        maxIndex = 4;
        break;
    case kPhase3:
        maxIndex = 6;
        break;
    }

    if (maxIndex <= razorSpawnLocation->pointArray.Num()) {

        for (int i = 0; i < maxIndex; i++) {

            //spawn razon on point[i] with stats razorStats[i]
            ABossRazor* spawnedRazor = Cast<ABossRazor>(GetWorld()->SpawnActor<AActor>(razorBP, 
                razorSpawnLocation->GetTransform().TransformPosition(razorSpawnLocation->pointArray[i]), 
                FRotator::ZeroRotator));
            if(spawnedRazor)
                spawnedRazor->movementData = razorConfig[i];

        }

    }


}

void AFinalBossCharacter::ChangePhase(TEnumAsByte<BossPhase> phase)
{

    //No need to case for phase 1 as it is the default phase.
    switch (phase) {

    case kFlying1:
        ToggleFlight();
        if (spawnTriggers.Num() > 0 && enemyCommander) {
            enemyCommander->GetSpawnTrigger(spawnTriggers[0]);
        }

        break;
    
    case kPhase2:

        //Laser upgrade
        laserAttack.chargeTime = 0.85;
        laserAttack.lasersToFire = 3;
        laserAttack.fireTime = 0.85;
        laserAttack.lockOnTime = 0.85;
        ToggleFlight();
        myGI->camera->LockCamera(groundBossCameraConfig);

        break;

    case kFlying2:

        ToggleFlight();
        if (spawnTriggers.Num() > 1 && enemyCommander) {
            enemyCommander->GetSpawnTrigger(spawnTriggers[1]);
        }
        break;

        break;
    case kPhase3:

        //Laser upgrade
        laserAttack.chargeTime = 0.6;
        laserAttack.lasersToFire = 5;
        laserAttack.fireTime = 0.6;
        laserAttack.lockOnTime = 0.6;
        ToggleFlight();
        myGI->camera->LockCamera(groundBossCameraConfig);
        break;

    }

    currentPhase = phase;
    if (blackboardComp) {
        blackboardComp->SetValueAsInt(FName("currentPhase"), currentPhase);
    }

}

void AFinalBossCharacter::CheckPhaseChange()
{
    if (!_animInstance->IsAnyMontagePlaying()) {

        switch (currentPhase) {
        case kPhase1:
            //Condition to switch phase
            if ((health * 100 / maxHealth) < phase2HPPercentage) {
                ChangePhase(kFlying1);
            }
        break;

        case kFlying1:
            if (enemyCommander) {

                if (!enemyCommander->spawningEnemies) {
                    ChangePhase(kPhase2);
                }

            }
        break;
        case kPhase2:
            //Condition to switch phase
            if ((health * 100 / maxHealth) < phase3HPPercentage) {
                ChangePhase(kFlying2);
            }
        break;
        case kFlying2:
            if (!enemyCommander->spawningEnemies) {
                ChangePhase(kPhase3);
            }
        break;
        case kPhase3:
            //Death?
        break;
        }

    }

}

void AFinalBossCharacter::TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter)
{

    //Inmunity to cc
    if (canBeDamaged) {
        if (attack_info.ultimate) {
            health -= attack_info.attack_dmg * 2.5f;
        }
        else {
            health -= attack_info.attack_dmg;
        }
        UpdateBossHealthBar.Broadcast(health, maxHealth);
        materialCharacter->SetScalarParameterValue(FName("EnableHolo"), holoStrength);
        materialCharacter2ElectricBoogaloo->SetScalarParameterValue(FName("EnableHolo"), holoStrength);
        GetWorld()->GetTimerManager().SetTimer(timerHitMaterial, this, &AFinalBossCharacter::ResetBossMaterial, timeResetMaterial, false);

    }

}

void AFinalBossCharacter::ToggleFlight()
{

    //Stop flying
    if (flying) {

        flying = false;
        GetCapsuleComponent()->SetCollisionProfileName("Enemy");
    
    }
    //Start flying
    else {

        flying = true;
        GetCapsuleComponent()->SetCollisionProfileName("EnemyD");
        enemiesToSpawn = true;
        GetWorldTimerManager().SetTimer(enemySpawnDelayTimer, this, &AFinalBossCharacter::EnemyDelayBool, 1.0f, false);
        //Reset flight up/down delay
        timer = 0.0f;

    }

}

void AFinalBossCharacter::EnemyDelayBool() {

    enemiesToSpawn = false;

}

void AFinalBossCharacter::ResetBossMaterial(){

    materialCharacter->SetScalarParameterValue(FName("EnableHolo"), 0.f);
    materialCharacter2ElectricBoogaloo->SetScalarParameterValue(FName("EnableHolo"), 0.f);


}
