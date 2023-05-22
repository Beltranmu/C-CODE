// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TurboPlayer.h"
#include "Core/CameraTrigger.h"
#include "HUD/TriggerDialog.h"
#include "Props/ThrowableActor.h"
#include "Core/GameInstanceBEU.h"
#include "Components/CapsuleComponent.h"
#include "Interface/EnemyDataUI.h"
#include "Components/BoxComponent.h"
#include "Props/Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/DestinationPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Navigation/CrowdManager.h"
#include "Components/DecalComponent.h"
#include "Components/PointLightComponent.h"
#include "Enemies/FinalBossCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

ATurboPlayer::ATurboPlayer() {

  time_charging_attack = 0.0f;

  godMode_ = false;
  noDMGMode_ = false;
  distance_to_spawn = 100.0f;
  characterID = 1;

  dmgBoosted = false;

	UDestinationPoint* tmpSceneComp = nullptr;
  USpringArmComponent* tmpSpringArm = nullptr;

  tmpSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmFront");
  springArmArray.Add(tmpSpringArm);
  springArmArray[0]->TargetArmLength = 50.f;
  springArmArray[0]->SetupAttachment(RootComponent);

  tmpSceneComp = CreateDefaultSubobject<UDestinationPoint>("Target Point - Front");
  targetPoint.Add(tmpSceneComp);
  targetPoint[0]->SetupAttachment(springArmArray[0]);

  tmpSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmBack");
  springArmArray.Add(tmpSpringArm);
  springArmArray[1]->TargetArmLength = 50.f;
  springArmArray[1]->SetupAttachment(RootComponent);

  tmpSceneComp = CreateDefaultSubobject<UDestinationPoint>("Target Point - Back");
  targetPoint.Add(tmpSceneComp);
  targetPoint[1]->SetupAttachment(springArmArray[1]);

  tmpSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmOutside1");
  springArmArray.Add(tmpSpringArm);
  springArmArray[2]->TargetArmLength = 50.f;
  springArmArray[2]->SetupAttachment(RootComponent);

  tmpSceneComp = CreateDefaultSubobject<UDestinationPoint>("PerimeterPoint1");
  targetPoint.Add(tmpSceneComp);
  targetPoint[2]->SetupAttachment(springArmArray[2]);

  tmpSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmOutside2");
  springArmArray.Add(tmpSpringArm);
  springArmArray[3]->TargetArmLength = 50.f;
  springArmArray[3]->SetupAttachment(RootComponent);

  tmpSceneComp = CreateDefaultSubobject<UDestinationPoint>("PerimeterPoint2");
  targetPoint.Add(tmpSceneComp);
  targetPoint[3]->SetupAttachment(springArmArray[3]);

  tmpSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmOutside3");
  springArmArray.Add(tmpSpringArm);
  springArmArray[4]->TargetArmLength = 50.f;
  springArmArray[4]->SetupAttachment(RootComponent);

  tmpSceneComp = CreateDefaultSubobject<UDestinationPoint>("PerimeterPoint3");
  targetPoint.Add(tmpSceneComp);
  targetPoint[4]->SetupAttachment(springArmArray[4]);

  tmpSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmOutside4");
  springArmArray.Add(tmpSpringArm);
  springArmArray[5]->TargetArmLength = 50.f;
  springArmArray[5]->SetupAttachment(RootComponent);

  tmpSceneComp = CreateDefaultSubobject<UDestinationPoint>("PerimeterPoint4");
  targetPoint.Add(tmpSceneComp);
  targetPoint[5]->SetupAttachment(springArmArray[5]);

  tmpSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmOutside5");
  springArmArray.Add(tmpSpringArm);
  springArmArray[6]->TargetArmLength = 50.f;
  springArmArray[6]->SetupAttachment(RootComponent);

  tmpSceneComp = CreateDefaultSubobject<UDestinationPoint>("PerimeterPoint5");
  targetPoint.Add(tmpSceneComp);
  targetPoint[6]->SetupAttachment(springArmArray[6]);

  tmpSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmOutside6");
  springArmArray.Add(tmpSpringArm);
  springArmArray[7]->TargetArmLength = 50.f;
  springArmArray[7]->SetupAttachment(RootComponent);

  tmpSceneComp = CreateDefaultSubobject<UDestinationPoint>("PerimeterPoint6");
  targetPoint.Add(tmpSceneComp);
  targetPoint[7]->SetupAttachment(springArmArray[7]);

  collisionAttackPreset = ECC_GameTraceChannel16;

  interactIcon = CreateDefaultSubobject<UWidgetComponent>("Interact Icon");
  interactIcon->SetupAttachment(RootComponent);

	for(int i = 0; i < 4; ++i) {
		
		perimeterPointRelativeDistance[i] = FVector::ZeroVector;
		resetPoints[i] = false;

	}

	timerSendEnemiesTowardsPlayerValueFirstTime = 7.f;
	timerSendEnemiesTowardsPlayerValueRepeatedly = 1.5f;

  notLoadingAttack = true;

	simplePunch = false;

	_decalRoot = CreateDefaultSubobject<USceneComponent>("Root Decal");
	_decalRoot->SetupAttachment(RootComponent);

	_decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	_decal->SetupAttachment(_decalRoot);

	_powerfulAttackPointLight = CreateDefaultSubobject<UPointLightComponent>("PowerfulAttackPointLight");
	_powerfulAttackPointLight->SetupAttachment(GetMesh(), FName("RightHandPointLightSocket"));
	_powerfulAttackPointLight->SetCanEverAffectNavigation(false);

	_ultimateHeadPointLight = CreateDefaultSubobject<UPointLightComponent>("UltimateHeadPointLight");
	_ultimateHeadPointLight->SetupAttachment(GetMesh(), FName("HeadPointLightSocket"));
	_ultimateHeadPointLight->SetCanEverAffectNavigation(false);

	_ultimateRootPointLight = CreateDefaultSubobject<UPointLightComponent>("UltimateRootPointLight");
	_ultimateRootPointLight->SetupAttachment(GetMesh(), FName("RootPointLightSocket"));
	_ultimateRootPointLight->SetCanEverAffectNavigation(false);
	
}

void ATurboPlayer::BeginPlay() {

	Super::BeginPlay();

	UCrowdManager* CrowdManager = UCrowdManager::GetCurrent(this); if (CrowdManager) { CrowdManager->RegisterAgent(this); }

	myGI = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));
	hud_ = Cast<AHUDGameLevel>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	//myGI->playerAOEAttackCharge[characterID] = 0.0f;
	OnActorBeginOverlap.AddDynamic(this, &ATurboPlayer::PlayerOverlap);

	hitEnemyLocation = FVector(0.0f, 120.0f, 0.0f);


  //Dash infinite
  maxDashCharges = -1;

	//POWER ATTACK
	keyName = FName("O");
	buttonName = FName("Gamepad_FaceButton_Right");
	ultimateButtonName = FName("Gamepad_LeftShoulder");
  GetWorldTimerManager().SetTimer(dashReFillTimer, this, &ATurboPlayer::addChargeDash, dashAddChargeTime, true);
	perimeterPointRelativeDistance[0] = targetPoint[2]->GetComponentLocation() - GetActorLocation(); // Front-Right
	perimeterPointRelativeDistance[1] = targetPoint[4]->GetComponentLocation() - GetActorLocation(); // Front-Left
	perimeterPointRelativeDistance[2] = targetPoint[3]->GetComponentLocation() - GetActorLocation(); // Back-Right
	perimeterPointRelativeDistance[3] = targetPoint[5]->GetComponentLocation() - GetActorLocation(); // Back-Left

  nameCollisionPreset = "Player";
  GetCapsuleComponent()->SetCollisionProfileName(nameCollisionPreset);
	//Widget
	interactIcon->SetVisibility(false);
  materialCharacter->SetScalarParameterValue(FName("EnableHolo"), 0);

	_powerfulAttackPointLight->ToggleVisibility();
	_ultimateHeadPointLight->ToggleVisibility();
	_ultimateRootPointLight->ToggleVisibility();

  shieldOffset = shieldParticle.locationOffset;
  fade = 1.0f;

  if (characterID == 1) {
	materialCharacter->SetTextureParameterValue(FName("BASECOLOR"), BaseColorTexture2);
	materialCharacter->SetTextureParameterValue(FName("EMISIVE"), EmisiveTexture2);

  }

}

void ATurboPlayer::Tick(float deltaTime) {

	Super::Tick(deltaTime);

  updateUIStamina.Broadcast(characterID, stamina, maxStamina);
	UpdatePerimeterPoints();
	UpdateWidget();

  if (parryIsActive) {
    SpawnParticleSystem(shieldParticle); }

  InGameCheats();

}

void ATurboPlayer::SetupPlayerInputComponent(UInputComponent* playerInputComponent) {

	Super::SetupPlayerInputComponent(playerInputComponent);
	inputComponent = playerInputComponent;
	SetInputGame(true);

}

void ATurboPlayer::PlayerOverlap(AActor* self, AActor* other) {

	ACameraTrigger* trigger = Cast<ACameraTrigger>(other);
	
	if (trigger) {

		myGI->camera->LockCamera(trigger->lockConfig);

		if(trigger->destroyOnActivate)
			other->Destroy();

	}

}

void ATurboPlayer::Interact()
{

	if (alive) {

		//Throws the object if already picked
		if (holdingObject) {

			IThrowableInterface* interface = Cast<IThrowableInterface>(attachedActor);
			if (interface) {
				if (this->GetActorForwardVector().X > 0) {

					interface->Throw(FVector(1.0f,1.0f,1.0f));

				}
				else {

					interface->Throw(FVector(1.0f,-1.0f,1.0f));
					if (!GetWorldTimerManager().IsTimerActive(slowTimer)) {


					}

				}
				holdingObject = false;
				attachedActor = nullptr;
				throwingBarrel = true;
				_attackEnd = false;
				ClearSlow();
				
			}

		}
		//Checks the object the player is interacting with
		else {

			if (!dashing && _attackEnd && canMove && canJumpChar  && !holdingObjectAnimation) {
				
				//Raycast
				
				FHitResult hit;
				GetWorld()->SweepSingleByChannel(hit, this->GetActorLocation(), this->GetActorLocation(), FQuat(0, 0, 0, 0), ECC_GameTraceChannel9, FCollisionShape::MakeSphere(100));
				//Check if throwable
				
				
				//Throwables
				/*IThrowableInterface* interface = Cast<IThrowableInterface>(hit.GetActor());
				if (interface) {
					if(hit.GetActor()->GetActorLocation().Z + 115.0f > GetActorLocation().Z)
						interface->Pick(hit.GetActor(), this);
				}*/


				
				//Check if pickup
				//else {

					APickup* pickup = Cast<APickup>(hit.Actor);
					if (pickup) {

						switch (pickup->type) {

						case kType_HPSMALL:
						
							health += maxHealth * pickup->hpRecoveryPercentage / 100;
							updateUIHP.Broadcast(characterID, health, maxHealth);
              SpawnParticleSystem(healthParticle);

							statsLimiter();
							break;
					
						case kType_HPBIG:

							health = maxHealth;
							updateUIHP.Broadcast(characterID, health, maxHealth);
              SpawnParticleSystem(healthParticle);
							break;

						case kType_1UP:

							myGI->playerLife[characterID]++;
							updateUILifes.Broadcast(characterID, myGI->playerLife[characterID]);
							break;

						case kType_AOE:
							myGI->playerAOEAttackCharge[characterID] = maxAOECharge;
							updateUIAOE.Broadcast(characterID, myGI->playerAOEAttackCharge[characterID], maxAOECharge);

							break;

						case kType_DMGBOOST:

							dmgBoosted = true;
							percentageDmgBoost = pickup->dmgBoost / 100;
							GetWorldTimerManager().SetTimer(dmgBoostTimer, this, &ATurboPlayer::ClearDmgBoost, pickup->dmgBoostDuration);
              SpawnParticleSystem(dmgUpParticleR);
              SpawnParticleSystem(dmgUpParticleL);
							break;

						case kType_INVINCIBILITY:

							canBeDamaged = false;
							GetWorldTimerManager().SetTimer(invencibleTimer, this, &ATurboPlayer::ClearInvencibleBoost, pickup->invencibleDuration);
              SpawnParticleSystem(invParticle);
							break;

						case kType_MONEY:
							score += pickup->moneyAmmount * FMath::FRandRange(1.0f - pickup->moneyMultiplierVariance, 1.0f + pickup->moneyMultiplierVariance);
							updateUIScore.Broadcast(characterID, score);
							break;
						}

						pickup->Destroy();

					}

				//}

			}

		}

	}

}

void ATurboPlayer::DialogControl()
{

	myGI->nDialog++;

}

void ATurboPlayer::TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter){
  if (!godMode_) {
    Super::TakeAHit_Implementation(knockback_force, attack_info, hitFromBehind, hiter);
    if(canBeDamaged && !blockIsActive){
	  
	    //Drop throwable when hit
	    if (holdingObject) {
		    IThrowableInterface* interface = Cast<IThrowableInterface>(attachedActor);
		    if (interface) {

			    interface->Throw(FVector(0.0f, 0.0f, 0.0f));
			    holdingObject = false;
			    holdingObjectAnimation = false;
			    attachedActor = nullptr;
			    _attackEnd = false;
			    ClearSlow();
		    }
	    }
    }
      
    if (health <= 0) {
      UGameplayStatics::SetGlobalTimeDilation(GetWorld(), scaleFactor);
      GetWorld()->GetTimerManager().SetTimer(timerSlowMo, this, &ABaseCharacter::ResetSlowMo, timeResetSlomo, false);
    }
    //Send the event id of the player, current Hp and max Hp
    updateUIHP.Broadcast(characterID, health, maxHealth);

    GetWorld()->GetTimerManager().ClearTimer(holdAttackTimer);
    GetCharacterMovement()->MaxWalkSpeed = defaultWalkSpeed;
    time_charging_attack = 0.0f;

    //CameraShake
		cameraShakeEvent.Broadcast(shakeForce * attack_info.attack_dmg);
    _decalRoot->SetRelativeScale3D(FVector(1.f, 0.f, 1.f));
		_powerfulAttackPointLight->ToggleVisibility();
		_powerfulAttackPointLight->SetIntensity(0.f);

  }
}

void ATurboPlayer::HitDamagable(AActor* otherActor) {
  if (noDMGMode_) {
    attackStats.attack_dmg = 0;
  }
  Super::HitDamagable(otherActor);
  if (alive) {

    //Restore Stamina per hit
    if (stamina + staminaPerHit < maxStamina) {
      stamina += staminaPerHit;
    }
    else {
      stamina = maxStamina;
    }
    

    FForceFeedbackParameters param;
    param.bLooping = false;
    UGameplayStatics::GetPlayerController(GetWorld(), characterID)->ClientPlayForceFeedback(vibrations[0], param);
    if (!attackStats.ultimate) {
      if (Cast<ABaseCharacter>(otherActor) && !Cast<AFinalBossCharacter>(otherActor)) {
        FVector EndPosition = this->GetActorLocation() + this->GetActorRightVector() * hitEnemyLocation;
        EndPosition.Z = otherActor->GetActorLocation().Z;
        EndPosition.Y = otherActor->GetActorLocation().Y;

        otherActor->SetActorLocation(EndPosition, true);
      }
    }
    attackStats.attack_dmg += attackStats.attack_dmg * percentageDmgBoost;
  }
  
}

void ATurboPlayer::HitUIUpdate(AActor* otherActor) {

	Super::HitUIUpdate(otherActor);
	int32 type_enemy = 0;
		float hpPercentage = 0.0f;
		int32 points = 0;
		IEnemyDataUI::Execute_CatchUIDataEnemy(otherActor, type_enemy, hpPercentage, points, characterID);
}

void ATurboPlayer::playerKick() {
  if (!isLoading) {
    Kick();
  }
}

void ATurboPlayer::playerPunch(){
  if (!isLoading) {
		
		Punch();
		
  }
}

void ATurboPlayer::playerPowerfulPunch()
{
	if(!isLoading && _attackEnd) {
	
		GetWorld()->GetTimerManager().SetTimer(timerHoldingToStartLoad, this, &ATurboPlayer::StartLoading, timeHoldingToStartLoad, false);

	}

}

void ATurboPlayer::StartLoading(){

  if (_attackEnd && (1 == UGameplayStatics::GetPlayerController(GetWorld(), characterID)->GetInputAnalogKeyState(buttonName) ||
									  (1 == UGameplayStatics::GetPlayerController(GetWorld(), characterID)->GetInputAnalogKeyState(keyName)))
     && !holdingObject && !state.alteredStatus && alive) {
    PlayAnimMontage(_powerAttackAnimtionMontage, 2.0f / (time_load_attack + timeHoldingToStartLoad), "Charging");
    time_charging_attack = timeHoldingToStartLoad;
    GetWorld()->GetTimerManager().SetTimer(holdAttackTimer, this, &ATurboPlayer::LoadAttack, 0.1f, true);
    UCharacterMovementComponent* characterMovement = Cast<UCharacterMovementComponent>(GetMovementComponent());
    isLoading = true;
    canMove = false;
    notLoadingAttack = false;

		// Get start time
		powerAttackCurrentHoldingTime = GetWorld()->GetTimeSeconds();
  }
}

void ATurboPlayer::LoadAttack(){
	if(time_charging_attack < time_load_attack + timeHoldingToStartLoad){
		time_charging_attack += 0.1f;
		deltaY = (time_charging_attack - timeHoldingToStartLoad) / time_load_attack;
		_decalRoot->SetRelativeScale3D(FVector(1.f, deltaY*0.775f, 1.f));
		float lightIntensity = _powerfulAttackPointLight->Intensity;
		_powerfulAttackPointLight->SetIntensity(lightIntensity + deltaY * powerfulAttackLightIntensityScale);
		//_decalRoot->SetWorldScale3D(FVector(1.f, time_charging_attack*powerfulPunchScale, 1.f));
  }
}

void ATurboPlayer::EndLoad()
{
  if (_attackEnd) {
    float tmpTime = 0.f;
    float maxTime = time_load_attack + timeHoldingToStartLoad;
    float minTime = timeHoldingToStartLoad;

    if (time_charging_attack > timeHoldingToStartLoad) {
      // Get end time
      tmpTime = GetWorld()->GetTimeSeconds();
      powerAttackCurrentHoldingTime = tmpTime - powerAttackCurrentHoldingTime;

      //canMove = false;
      PlayAnimMontage(_powerAttackAnimtionMontage, releaseSpeed, "Released");
      //
      deltaY = (time_charging_attack - timeHoldingToStartLoad) / time_load_attack;
      bonus_charging_attack = minMultiplierFactorPowerAttack * (1 - deltaY) + maxMultiplierFactorPowerAttack * (deltaY);

			float lightIntensity = _powerfulAttackPointLight->Intensity;
			_powerfulAttackPointLight->SetIntensity(lightIntensity + deltaY * powerfulAttackLightIntensityScale * powerfulAttackLightIntensityReleaseScale);			

      UCharacterMovementComponent* characterMovement = Cast<UCharacterMovementComponent>(GetMovementComponent());
      _attackEnd = false;

    }
    else {

      StopAnimMontage(_powerAttackAnimtionMontage);
    }
    _decalRoot->SetRelativeScale3D(FVector(1.f, 0.f, 1.f));

    GetWorld()->GetTimerManager().ClearTimer(holdAttackTimer);
    time_charging_attack = 0.0f;
    isLoading = false;
    notLoadingAttack = true;
    canMove = false;
  }
  
}

void ATurboPlayer::AOEAttack()
{
	
	startUltimate = false;
  if (!isLoading) {
    PlayAnimMontage(_ultimateAnimationMontage, 1.f, "Ultimate");

    myGI->playerAOEAttackCharge[characterID] = 0.0f;
    updateUIAOE.Broadcast(characterID, myGI->playerAOEAttackCharge[characterID], maxAOECharge);
    _attackEnd = false;
    canBeDamaged = false;
    _ultimateAnimationMontage->CalculateSequenceLength();
    GetWorld()->GetTimerManager().SetTimer(ultimateRestDMGTimer, this, &ATurboPlayer::ClearInvencibleBoost, _ultimateAnimationMontage->CalculateSequenceLength(), false);
    if (blockIsActive) {
      stopBlock();
    }
  }

}

void ATurboPlayer::callToUIEvent(int id, float hpPercentage, int enemyScore)
{

	//catchEnemyDataOnHit.Broadcast(id, hpPercentage, characterID);
	if(hpPercentage <= 0){
		

    score += enemyScore;
		myGI->playerAOEAttackCharge[characterID] += 10.0f;
    updateUIAOE.Broadcast(characterID, myGI->playerAOEAttackCharge[characterID], maxAOECharge);
    updateUIScore.Broadcast(characterID, score);
    if (myGI->playerAOEAttackCharge[characterID] > 100.0f) {
			myGI->playerAOEAttackCharge[characterID] = 100.0f;
    }
	}
}

void ATurboPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
  if (catchEnemyDataOnHit.IsBound()) {
		catchEnemyDataOnHit.RemoveAll(this);
  }

	if(triggerEvent.IsBound()){
		triggerEvent.RemoveAll(this);
	}
}

void ATurboPlayer::statsLimiter()
{
	if (health >= maxHealth)
	{
		health = maxHealth;
    updateUIHP.Broadcast(characterID, health, maxHealth);
	}
	if(myGI->playerAOEAttackCharge[characterID] >= maxAOECharge)
	{
		myGI->playerAOEAttackCharge[characterID] = maxAOECharge;
    updateUIAOE.Broadcast(characterID, myGI->playerAOEAttackCharge[characterID], maxAOECharge);
	}
}

void ATurboPlayer::CharacterDash()
{
	float x, y;
	x = GetInputAxisValue("AxisX");
	y = GetInputAxisValue("AxisY");

	//I don't know, it just works
  bool haveDash = (currentDashCharges > 0 || maxDashCharges == -1);
  if (haveDash) {
    if(canDash  && grounded){
      Super::Dash(FVector(y, x, 0.0f));
      if (maxDashCharges > 0) { currentDashCharges--; }
      updateUIDash.Broadcast(characterID, currentDashCharges);
    }
  }
}

void ATurboPlayer::addChargeDash()
{
  if (currentDashCharges < maxDashCharges) {
    currentDashCharges++;
    updateUIDash.Broadcast(characterID, currentDashCharges);
  }

}

void ATurboPlayer::changeColor() {
	materialCharacter->SetVectorParameterValue(FName("BodyColor"), original_color);
}

void ATurboPlayer::ClearInvencibleBoost() {

	canBeDamaged = true;

  if (IsValid(invParticle.particleComponent)) {
    invParticle.particleComponent->DeactivateSystem();
  }
  if (IsValid(invParticle.particleComponentNiagara)) {
    invParticle.particleComponentNiagara->Deactivate();
  }

}

void ATurboPlayer::ClearDmgBoost() {

	dmgBoosted = false;


  if (IsValid(dmgUpParticleR.particleComponent)) {
    dmgUpParticleR.particleComponent->DeactivateSystem();
  }
  if (IsValid(dmgUpParticleL.particleComponent)) {
    dmgUpParticleL.particleComponent->DeactivateSystem();
  }


  if (IsValid(dmgUpParticleR.particleComponentNiagara)) {
    dmgUpParticleR.particleComponentNiagara->Deactivate();
  }
  if (IsValid(dmgUpParticleL.particleComponentNiagara)) {
    dmgUpParticleL.particleComponentNiagara->Deactivate();
  }




}

void ATurboPlayer::Death() {
  Super::Death();
	myGI->playerLife[characterID] -= 1;
	
	updateUILifes.Broadcast(characterID, myGI->playerLife[characterID]);

  GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyD"));
  canMove = false;
  alive = false;
	if (myGI->playerLife[characterID] > 0){
		
		//TE HAS QUEDADO PROBANDO LA EXPLOSION DE RESPAWN
		
		GetWorld()->GetTimerManager().SetTimer(respawnTimer, this, &ATurboPlayer::respawnExplosion, timeToRespawn, false);
		timeToRespawn -= 0.2f;
		SetActorEnableCollision(false);
	}
	else{
		

		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		SetActorHiddenInGame(true);
		DisableInput(GetWorld()->GetFirstPlayerController());
		blockMovementLeft = false;
		blockMovementRight = false;

		{
			myGI->deathplayer();
			hud_->ShowDeadMenu();
		}
		
	}

	

}

void ATurboPlayer::Respawn(){

  FVector aux = GetActorLocation();
  aux.Z += distance_to_spawn;
  SetActorLocation(aux);
	health = maxHealth;
	updateUIHP.Broadcast(characterID, health, maxHealth);
  alive = true;
	canMove = true;
  SetActorEnableCollision(true);
  SetActorTickEnabled(true);
  SetActorHiddenInGame(false);

  GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

}

void ATurboPlayer::pauseGame()
{

	hud_->PauseGame();

}

void ATurboPlayer::HideUI()
{

	hud_->HideAllUI();

}

void ATurboPlayer::SetInputGame(bool val)
{
	if (!inputComponent)
		return;
	if (val) {

		

		/** MOVEMENT */
		inputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::myJump);
		//inputComponent->BindAction("Jump", IE_Pressed, this, &ATurboPlayer::DialogControl);
		inputComponent->BindAxis("AxisX", this, &ABaseCharacter::moveAxisY);
		inputComponent->BindAxis("AxisY", this, &ABaseCharacter::moveAxisX);

		/** COMBAT */
		inputComponent->BindAction("Punch", IE_Pressed, this, &ATurboPlayer::playerPunch);
		inputComponent->BindAction("PowerfulPunch", IE_Pressed, this, &ATurboPlayer::playerPowerfulPunch);
 		inputComponent->BindAction("PowerfulPunch", IE_Released, this, &ATurboPlayer::EndLoad);
		inputComponent->BindAction("Kick", IE_Pressed, this, &ATurboPlayer::playerKick);
		inputComponent->BindAction("AOEAttack", IE_Pressed, this, &ATurboPlayer::StartUltimate);
		inputComponent->BindAction("AOEAttack2", IE_Pressed, this, &ATurboPlayer::StartUltimate2);
		inputComponent->BindAction("AOEAttack3", IE_Pressed, this, &ATurboPlayer::StartUltimateKeyboard);


		/** BLOCKING */
		inputComponent->BindAction("Block", IE_Repeat, this, &ABaseCharacter::startBlock);
		inputComponent->BindAction("Block", IE_Pressed, this, &ABaseCharacter::startBlock);
		inputComponent->BindAction("Block", IE_Released, this, &ABaseCharacter::stopBlock);

		/** SHOW INPUT */
		inputComponent->BindAction("PauseGame", IE_Released, this, &ATurboPlayer::pauseGame);

		/** HIDE UI COMMAND*/
		inputComponent->BindAction("HideUI", IE_Released, this, &ATurboPlayer::HideUI);


		/** PICK/THROW*/
		inputComponent->BindAction("Interact", IE_Pressed, this, &ATurboPlayer::Interact);

		/** DASH */
		inputComponent->BindAction("Dash", IE_Pressed, this, &ATurboPlayer::CharacterDash);

		/**TEST*/
		//inputComponent->BindAction("Test", IE_Pressed, this, &ATurboPlayer::Test);
	}
	else {
		/** MOVEMENT */
		inputComponent->RemoveActionBinding ("Jump", IE_Pressed);
		//inputComponent->BindAction("Jump", IE_Pressed, this, &ATurboPlayer::DialogControl);
	

		/** COMBAT */
		inputComponent->RemoveActionBinding("Punch", IE_Released);
		inputComponent->RemoveActionBinding("PowerfulPunch", IE_Pressed);
		inputComponent->RemoveActionBinding("PowerfulPunch", IE_Released);
		inputComponent->RemoveActionBinding("Kick", IE_Pressed);
		inputComponent->RemoveActionBinding("AOEAttack", IE_Pressed);


		/** BLOCKING */
		inputComponent->RemoveActionBinding("Block", IE_Repeat);
		inputComponent->RemoveActionBinding("Block", IE_Pressed);
		inputComponent->RemoveActionBinding("Block", IE_Released);

		/** PICK/THROW*/
		inputComponent->RemoveActionBinding("Interact", IE_Pressed);

		/** DASH */
		inputComponent->RemoveActionBinding("Dash", IE_Pressed);

		/**TEST*/
		//inputComponent->BindAction("Test", IE_Pressed, this, &ATurboPlayer::Test);
	}
}

void ATurboPlayer::SetInputDialog(bool val)
{
	if (val)
	{
		inputComponent->BindAction("Jump", IE_Pressed, this, &ATurboPlayer::DialogControl);
	}
	else
	{
		inputComponent->RemoveActionBinding("Jump", IE_Pressed);
	}

}

void ATurboPlayer::respawnExplosion() {
	float explosionRadius = 200.0f;
	FCollisionShape colSphere = FCollisionShape::MakeSphere(explosionRadius);
	TArray<FHitResult> hittedActors;

	if (GetWorld()->SweepMultiByChannel(hittedActors, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel13, colSphere)) {

		for (auto& hittedActor : hittedActors) {

			AActor* actor = hittedActor.GetActor();


      if (actor->GetClass()->ImplementsInterface(UEnemyDataUI::StaticClass())) {
				HitUIUpdate(actor);
      }
			if (actor->GetClass()->ImplementsInterface(UIDamagable::StaticClass())) {
				float actorExplosionDirection = explosionDirection;
				float explosionPower = (1.0f/(actor->GetActorLocation() - this->GetActorLocation()).Size()) * explosionForce * 100;
				if((actor->GetActorLocation().Y - this->GetActorLocation().Y)< 0.0f){
					actorExplosionDirection = 180 - explosionDirection;
				}
				float angle = FMath::DegreesToRadians(actorExplosionDirection);
				FVector explosionVector = FVector(0.0f, explosionPower * FMath::Cos(angle), explosionPower * FMath::Sin(angle));

				attackStats.attack_dmg = 0;
				attackStats.time_apply_knock_down = true;
				IIDamagable::Execute_TakeAHit(actor, explosionVector , attackStats, false, this);
			}
		}
	}
  //deathParticle.locationOffset = GetMesh()->GetSocketLocation("Cintunra");
  SpawnParticleSystem(deathParticle);
	GetWorld()->GetTimerManager().SetTimer(respawnTimer, this, &ATurboPlayer::Respawn, timeToRespawn, false);
}

void ATurboPlayer::UpdatePerimeterPoints() {

	FHitResult hitResult;
	FCollisionQueryParams hitParams(FName(TEXT("Botijo")));
	hitParams.AddIgnoredActor(this);

  FHitResult hitResult2;
  FCollisionQueryParams hitParams2(FName(TEXT("Batman")));
  hitParams2.AddIgnoredActor(this);
		
	if (GetWorld()->LineTraceSingleByChannel(hitResult, springArmArray[2]->GetComponentLocation(),
																					 springArmArray[2]->GetComponentLocation() + springArmArray[2]->GetForwardVector() * -350.f,
																					 ECC_GameTraceChannel14, hitParams)) {
	
		springArmArray[2]->AddWorldRotation(FRotator(0.f, -5.f, 0.f));
		springArmArray[2]->TargetArmLength -= 10.f;
		resetPoints[0] = true;
	
	}else{
	
		if(resetPoints[0]) {
      if(!GetWorld()->LineTraceSingleByChannel(hitResult2, GetActorLocation(),
																					 GetActorLocation() + perimeterPointRelativeDistance[0],
																					 ECC_GameTraceChannel14, hitParams2)) {

				springArmArray[2]->SetWorldRotation(FRotator(0.f, 320.f, 0.f));
				springArmArray[2]->TargetArmLength = 350.f;
				resetPoints[0] = false;

			}
		}
	
	}
	
  if (GetWorld()->LineTraceSingleByChannel(hitResult, springArmArray[4]->GetComponentLocation(),
																					 springArmArray[4]->GetComponentLocation() + springArmArray[4]->GetForwardVector() * -350.f,
																					 ECC_GameTraceChannel14, hitParams)) {
		
    springArmArray[4]->AddWorldRotation(FRotator(0.f, 5.f, 0.f));
		springArmArray[4]->TargetArmLength -= 10.f;
    resetPoints[1] = true;
	
  }else{
	
    if (resetPoints[1]) {
      if (!GetWorld()->LineTraceSingleByChannel(hitResult2, GetActorLocation(),
        GetActorLocation() + perimeterPointRelativeDistance[1],
        ECC_GameTraceChannel14, hitParams2)) {
	
        springArmArray[4]->SetWorldRotation(FRotator(0.f, 40.f, 0.f));
				springArmArray[4]->TargetArmLength = 350.f;
        resetPoints[1] = false;

      }
    }
	
  }
	
  if (GetWorld()->LineTraceSingleByChannel(hitResult, springArmArray[5]->GetComponentLocation(),
																					 springArmArray[5]->GetComponentLocation() + springArmArray[5]->GetForwardVector() * -350.f,
																					 ECC_GameTraceChannel14, hitParams)) {
	
    springArmArray[5]->AddWorldRotation(FRotator(0.f, -5.f, 0.f));
		springArmArray[5]->TargetArmLength -= 10.f;
    resetPoints[2] = true;
	
  }else{
	
    if (resetPoints[2]) {
      if (!GetWorld()->LineTraceSingleByChannel(hitResult2, GetActorLocation(),
        GetActorLocation() + perimeterPointRelativeDistance[2],
        ECC_GameTraceChannel14, hitParams2)) {
	
        springArmArray[5]->SetWorldRotation(FRotator(0.f, 140.f, 0.f));
				springArmArray[5]->TargetArmLength = 350.f;
        resetPoints[2] = false;

      }
    }
	
  }
	
  if (GetWorld()->LineTraceSingleByChannel(hitResult, springArmArray[3]->GetComponentLocation(),
																					 springArmArray[3]->GetComponentLocation() + springArmArray[3]->GetForwardVector() * -350.f,
																					 ECC_GameTraceChannel14, hitParams)) {
	
    springArmArray[3]->AddWorldRotation(FRotator(0.f, 5.f, 0.f));
		springArmArray[3]->TargetArmLength -= 10.f;
    resetPoints[3] = true;
	
  }else{
	
    if (resetPoints[3]) {
      if (!GetWorld()->LineTraceSingleByChannel(hitResult2, GetActorLocation(),
        GetActorLocation() + perimeterPointRelativeDistance[3],
        ECC_GameTraceChannel14, hitParams2)) {
	
        springArmArray[3]->SetWorldRotation(FRotator(0.f, 220.f, 0.f));
				springArmArray[3]->TargetArmLength = 350.f;
        resetPoints[3] = false;

      }
    }
	
  }

}

void ATurboPlayer::UpdateWidget()
{

	bool interactuableFound = false;
	if (alive && !holdingObject) {

		FHitResult hit;
		GetWorld()->SweepSingleByChannel(hit, this->GetActorLocation(), this->GetActorLocation(), FQuat(0, 0, 0, 0), ECC_GameTraceChannel9, FCollisionShape::MakeSphere(100));

		//Disable throwables
		//IThrowableInterface* interface = Cast<IThrowableInterface>(hit.GetActor());
		APickup* pickup = Cast<APickup>(hit.Actor);
		if (/*interface ||*/ pickup) {
			interactuableFound = true;
		}
	}

	interactIcon->SetVisibility(interactuableFound);

}

void ATurboPlayer::InGameCheats()
{

	APlayerController* cont = UGameplayStatics::GetPlayerController(GetWorld(), characterID);
	if (cont->GetInputAnalogKeyState(FName("Gamepad_RightShoulder")) >= 1.0f &&
		cont->GetInputAnalogKeyState(FName("Gamepad_LeftShoulder")) >= 1.0f &&
		cont->GetInputAnalogKeyState(FName("Gamepad_RightTrigger")) >= 1.0f &&
		cont->GetInputAnalogKeyState(FName("Gamepad_LeftTrigger")) >= 1.0f) {

		//Teleport player to arena center
		if (cont->GetInputAnalogKeyState(FName("Gamepad_FaceButton_Bottom")) >= 1.0f) {
			FVector newLocation = myGI->camera->GetActorLocation();
			newLocation.Z += 20.0f;
			newLocation.Y += 50.0f;
			SetActorLocation(newLocation);
			_attackEnd = true;
			canJumpChar = true;
		}

		if (cont->GetInputAnalogKeyState(FName("Gamepad_FaceButton_Right")) >= 1.0f) {
			myGI->camera->UnlockCamera();
		}

	}

}


void ATurboPlayer::StartUltimate() {

  ultimateButtonName = FName("Gamepad_LeftShoulder");
  bool canUltimate = canJumpChar && !holdingObject && !dashing && alive;
	if (canUltimate) {

		if (_attackEnd) {

			if (maxAOECharge <= myGI->playerAOEAttackCharge[characterID] || unlimitedUltimate) {

				if (!startUltimate &&
					 (1 == UGameplayStatics::GetPlayerController(GetWorld(), characterID)->GetInputAnalogKeyState(ultimateButtonName))) {

					startUltimate = true;

					AOEAttack();

				}

			}		
	
		}

	}

}

void ATurboPlayer::StartUltimate2() {

	ultimateButtonName = FName("Gamepad_RightShoulder");

  bool canUltimate = canJumpChar && !holdingObject && !dashing && alive;
  if (canUltimate) {

		if (_attackEnd) {

			if (maxAOECharge <= myGI->playerAOEAttackCharge[characterID] || unlimitedUltimate) {

				if (!startUltimate &&
					(1 == UGameplayStatics::GetPlayerController(GetWorld(), characterID)->GetInputAnalogKeyState(ultimateButtonName) ||
						1 == UGameplayStatics::GetPlayerController(GetWorld(), characterID)->GetInputAnalogKeyState(FName("R")))) {

					startUltimate = true;

					AOEAttack();

				}

			}

		}

	}

}

void ATurboPlayer::StartUltimateKeyboard() {

  bool canUltimate = canJumpChar && !holdingObject && !dashing && alive;
  if (canUltimate) {

		if (_attackEnd) {

			if (maxAOECharge <= myGI->playerAOEAttackCharge[characterID] || unlimitedUltimate) {

				if (!startUltimate) {

					startUltimate = true;

					AOEAttack();

				}

			}

		}

	}

}