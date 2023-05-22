// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/BaseCharacter.h"
#include "Player/CustomAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/BaseEnemy.h"
#include "Player/TurboPlayer.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  /** STATS */
  score = 0;
  maxHealth = 100.0f;
  health = maxHealth;
  blockArmorMultiplier = 0.5f;
  attackStats.attack_dmg = 0.0f;
  attackStats.attack_knockback= 0.0f;
  attackStats.yKnockBackForce= 0.0f; 
  attackStats.apply_hit = false;
  alive = true;
  canBeDamaged = true;

  /** JUMP */
  canJumpChar = true;
  jumpUnitsX = 500.f;
  jumpUnitsY = 250.f;
  jumpUnitsZ = 500.f;

  /** MOVEMENT */
  canMove = true;
  moveXSpd = 50.f;			// Vertically
  moveYSpd = 50.f;			// Horizontally
  rotationAngle = 0.f;
  fwDirection = FRotator::ZeroRotator;
  rotationVector = FRotator::ZeroRotator;
  velocityVector = FVector::ZeroVector;

  /** ANIMATION */
  _attackEnd = true;
  _bComboTime = false;
  _onCombo = false;
  _punching = false;
  _kicking = false;

  /**COMBAT*/
  bonus_charging_attack = 0.0f;
  canDash = true;

  /** BLOCK */
  maxStamina = 100.0f;
  stamina = maxStamina;
  blockIsActive = false;
  blockIsActive = false;
  parryIsActive = false;
  blockOnCD = false;
  parryTime = 0.1f;
  staminaLossPerSecond = 17.5f;
  staminaRegenPerSecond = 8.f;
  minimumStamina = 20.f;

  /** KNOCKBACK */
  knockBackUnits = FVector::ZeroVector;
  _airAttackOnce = false;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
  /** Movement */
  fwDirection = FRotator(0.0f, 90.0f, 0.0f);

  /** Animation */
  if (nullptr == _animInstance) {
    _animInstance = Cast<UCustomAnim>(GetMesh()->GetAnimInstance());
  }
  _punching = false;
  _kicking = false;


  materialCharacter = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
  GetMesh()->SetMaterial(0, materialCharacter);
  FHashedMaterialParameterInfo a;


  materialCharacter->SetScalarParameterValue(FName("EnableHolo"), 0);

  hit_color = FColor(255, 255, 0, 255);
  stunned_color = FColor(255, 128, 0, 255);
  knockdown_color = FColor(255, 0, 0, 255);

  


  //For slows not stacking/properly returning to default speed
  defaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
  currentDashCharges = maxDashCharges;

}

void ABaseCharacter::EndPlay(EEndPlayReason::Type EndReason)
{
  Super::EndPlay(EndReason);

  GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  rotationVector = FRotator(0.0f, rotationAngle, 0.0f);
  //Rotate player and scale mesh
  SetActorRotation(FQuat(rotationVector));
  FVector newScale = rotationAngle == 0.0f ? FVector(1.0f, 1.0f, 1.0f) : FVector(-1.0f, 1.0f, 1.0f);
  GetMesh()->SetRelativeScale3D(newScale);

  if (throwingBarrel) {
    GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

  }

  onAir();
  
  materialCharacter->SetScalarParameterValue(FName("Fade"), fade);
  materialCharacter->SetScalarParameterValue(FName("DeathFade"), dfade);
  //Block tick
  /*if (stamina <= maxStamina && !blockIsActive) {
    stamina += staminaRegenPerSecond * DeltaTime;
  }*/

}

/** INTERFACE */
void ABaseCharacter::TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter){

    bool blocked = blockIsActive && (!hitFromBehind || blocks360) && attack_info.blockeable;
  if(blocked){
    
     stamina -= attack_info.attack_dmg;
     attack_info.attack_dmg = 0;
     if (parryIsActive) {
         ABaseEnemy* character = Cast<ABaseEnemy>(hiter);
         if (character) {
             
             FAttackStats parryStats;
             parryStats.apply_stun = true;
             parryStats.apply_hit = false;
             parryStats.time_apply_stun = 1.0f;
             parryStats.blockeable = false;
             parryStats.attack_dmg = 0.0f;

             attackStats = parryStats;

             character->hitByPlayerID = characterID;
             HitDamagable(character);

         }
     }

     if (stamina <= 0.0f) {
       stopBlock();
     }

  }

  
  if (canBeDamaged) {
    health -= attack_info.attack_dmg;
    if (attack_info.attack_dmg > 0) {
      materialCharacter->SetScalarParameterValue(FName("EnableHolo"), 1);
      GetWorld()->GetTimerManager().SetTimer(timerHitMaterial, this, &ABaseCharacter::ResetMaterial, timeResetMaterial, false);
    }
  }

    
    bonus_charging_attack = 0.f;
    //Still Alive
    if(health > 0 && alive){
      if (canBeDamaged && !unostopable) {
        if (!blocked) {
          stopBlock();
        }

        //HIT
        if(attack_info.apply_hit && (!blocked) && !state.hit){
          materialCharacter->SetVectorParameterValue(FName("BodyColor"), hit_color);
          state.hit = attack_info.apply_hit;
          canMove = false;
          timeResetMaterial = attack_info.time_apply_hit;
          state.alteredStatus = true;
          GetWorld()->GetTimerManager().SetTimer(timerResetHit, this, &ABaseCharacter::resetStateStats, attack_info.time_apply_hit, false);
        }

        //KNOCKDOWN
        if(attack_info.apply_knock_down && (!blocked) && !state.knockdown){
          materialCharacter->SetVectorParameterValue(FName("BodyColor"), knockdown_color);
          state.knockdown = attack_info.apply_knock_down;
          canMove = false;
          timeResetMaterial = attack_info.time_apply_knock_down;
          state.alteredStatus = true;
          GetCapsuleComponent()->SetCollisionProfileName("EnemyD");
          if (blockIsActive) {
              stopBlock();
          }
          GetWorld()->GetTimerManager().SetTimer(timerResetKnockDown, this, &ABaseCharacter::standUp, attack_info.time_apply_knock_down, false);
        }

        //STUNNED
        if(attack_info.apply_stun && (!blocked) && !state.stun){
          materialCharacter->SetVectorParameterValue(FName("BodyColor"), stunned_color);
          state.stun = attack_info.apply_stun;
          if (blockIsActive) {
              stopBlock();
          }
          canMove = false;
          timeResetMaterial = attack_info.time_apply_stun;
          state.alteredStatus = true;
          GetWorld()->GetTimerManager().SetTimer(timerResetStun, this, &ABaseCharacter::resetStateStats, attack_info.time_apply_stun, false);
        }
      }
    
        //stop the current attack/animation
	    if (state.alteredStatus) {

		    UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		    animInstance->StopAllMontages(0.0f);
        _attackEnd = true;
        _onCombo = false;
        _bComboTime = false;

	    }
      if (knockback_force != FVector::ZeroVector) {
        LaunchCharacter(FVector(0.0f, 2 * knockback_force.Y, abs(knockback_force.Z)), true, true);
      }
    }
    else { //DEATH

      //ATurboPlayer* auxPlayer = Cast<ATurboPlayer>(this);
      //if (!auxPlayer)
      OnDeathImpulse();

    }
      
}
 

void ABaseCharacter::Death() {
  ResetMaterial();
}

/** MOVEMENT */
void ABaseCharacter::moveAxisX(float value) {
  if (canMove && alive) {
    
    if (value != 0 && _attackEnd)
    {
      velocityVector = FRotationMatrix(fwDirection * -1.0f).GetUnitAxis(EAxis::Y);

      velocityVector.Normalize();

      AddMovementInput(velocityVector * moveXSpd, value);
    }
  }
}

void ABaseCharacter::moveAxisY(float value) {
  if (canMove && alive) {

    if (value < 0) {
        tmprotationAngle = 180.0f;
      if (blockMovementLeft) {
          value = 0.f;
          GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
      }
    }
    if (value > 0) {
      tmprotationAngle = 0.0f;
      if (blockMovementRight) {
          value = 0.f;
          GetMesh()->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
      }
    }


    if (_attackEnd) {
      if (value < 0) {
        GetMesh()->SetRelativeRotation(FRotator(0.0f,-90.0f, 0.0f));
      }
      if (value > 0) { 
        GetMesh()->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
      }
      rotationAngle = tmprotationAngle;
      if (value != 0) {
        velocityVector = FRotationMatrix(fwDirection).GetUnitAxis(EAxis::X);
        AddMovementInput(velocityVector * moveYSpd, value);
      }
    }
  }
}

void ABaseCharacter::onAir(){

  FHitResult hitObj;
  bool firstTime = false;

  FCollisionQueryParams TraceParams(FName(TEXT("CheckGround"), false));
  TraceParams.AddIgnoredActor(this);

  FCollisionShape colSphere = FCollisionShape::MakeSphere(37.0f);
  FVector raycastPosition = GetActorLocation() + FVector(0.0f, 0.0f, -76.0f);
  grounded = (GetWorld()->SweepSingleByChannel
  (hitObj, raycastPosition, raycastPosition,
    FQuat::Identity, ECC_GameTraceChannel2, colSphere, TraceParams));
  canJumpChar = grounded;
  if(grounded){

    if (_airAttackOnce == true) {
      PlayAnimMontage(_airKickAnimationMontage, 1.f, FName("EndAirKick"));
      _airAttackOnce = false;
      if (_airAttackDone) { SpawnParticleSystem(airKickParticle);
        _airAttackDone = false;
      }
    }

    firstTime = false;
    FName hitCollisionProfileName = hitObj.GetComponent()->GetCollisionProfileName();
    if("GroundProp" == hitCollisionProfileName || "DestructibleBehaviour" == hitCollisionProfileName
      || "ThrowableBehaviour" == hitCollisionProfileName)
    {
      cannotJump = true;
      canMove = true;
      firstTime = true;
    }
    if (!dashing && !state.alteredStatus && !blockIsActive && notLoadingAttack && !firstTime ) {

      canJumpChar = true;
      canMove = true;
      //knockbacked = false;

      _airAttackOnce = false;
      cannotJump = false;
    }
  }
  //canMove = grounded;
}



/** JUMP */
void ABaseCharacter::myJump() {
  bool canJump = notLoadingAttack && canMove && _attackEnd && canJumpChar && !attackStats.apply_hit && !holdingObject && alive && grounded && !cannotJump;
  if (canJump) {
    jumpLogic();
  }
}

void ABaseCharacter::myStopJump() {
  StopJumping();
}

void ABaseCharacter::jumpLogic()
{
  FVector velocity = GetVelocity();
  velocity.Normalize();

  FVector auxJumpVector = FVector(velocity.X * jumpUnitsX, velocity.Y * jumpUnitsY, jumpUnitsZ);

  //auxJumpVector.Normalize();

  LaunchCharacter(auxJumpVector, true, true);
  canJumpChar = false;
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
  Super::Landed(Hit);
    
  //if(!dashing && !state.alteredStatus){
  //  
  //    canJumpChar = true;
  //    canMove = true;
  //    //knockbacked = false;
  //    _airAttackOnce = false;
  //}
  
}

/** COMBAT */
void ABaseCharacter::Punch() {
  if (grounded) {
    Attack(kInput_X);
  }
}
  

void ABaseCharacter::Kick() { Attack(kInput_Y); }

void ABaseCharacter::Attack(InputType move) {

  bonus_charging_attack = 0.0f;

  if (blockIsActive) {
      stopBlock();
  }
  //stun check must be changed using altered effects
  if (!holdingObject && !dashing && !state.alteredStatus && alive) {

    if (_attackEnd) {

      _attackEnd = false;

	  //Air behaviour

      if (!grounded) {
        if (!_airAttackOnce){

            _airAttackOnce = true;
          if(move == kInput_Y && stamina >= staminaCostAirKick) {
            stamina -= staminaCostAirKick;
            _airAttackDone = true;
            //Strong air kick
            PlayAnimMontage(_airKickAnimationMontage, 1.f, FName("AirKick"));

            //Forward vector is rotated, which is really fun so i have to make this.
            LaunchCharacter(FVector(0.0f, GetActorForwardVector().X * 1150.0f, -350.0f), true, true);

            //Just to remember, if the attack is not executed _attackEnd will never be false again
            //The character won't attack again if that happens.

          }
        }
        else{
          _attackEnd = true;
        }
      }
      else {

		  //Grounded behaviour

          switch (move) {

          case 0:
            PlayAnimMontage(_highAttackAnimationMontage, 1.f, _nextXInput);
			_punching = true;
            break;

          case 1:
            PlayAnimMontage(_lowAttackAnimationMontage, 1.f, _nextYInput);
			_punching = false;
            break;

          default:
            break;
          }

      }


    }
    else {

		//Attack has not finished yet, but can combo
      if (_bComboTime) {

		  _onCombo = true;
          
          //Limited to X buttons in the buffer
          if(inputArray.Num() < maxInputsStored)
            inputArray.Add(move);

      }

    }

  }

  //If we are holding some type of throwable we throw it forward
  if (holdingObject) {

	  ATurboPlayer* character;
	  character = Cast<ATurboPlayer>(this);
	  if (character) {
		  character->Interact();
	  }

  }

}

void ABaseCharacter::Dash(FVector dir)
{
    if (blockIsActive) {
        stopBlock();
    }
    bool canIDash = !dashing && canJumpChar
      && !holdingObject && canMove
      && canDash && (stamina >= dashStaminaCost)
      && alive && grounded;
    //if (!dashing && canJumpChar && !holdingObject && canMove && canDash && stamina >= dashStaminaCost) {
    if (canIDash) {

      canMove = false;
        canBeDamaged = false;
        dashing = true;
        canDash = false;

        if (blockIsActive) {
            stopBlock();
        }

        dir.Z = 0.0f;

        if (dir.IsNearlyZero()) {
            dir = this->GetActorForwardVector();

            //Forward vector is rotated, which is really fun so i have to make this.

            float aux = dir.X;
            dir.X = dir.Y;
            dir.Y = aux;

        }

        dir.Normalize();
        dir *= dashSpeed ;

        LaunchCharacter(dir, true, false);
        GetCharacterMovement()->GroundFriction = 0.0f;
        GetCapsuleComponent()->SetPhysicsLinearVelocity(dir,true);

        
        GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterDashBehaviour"));
        GetWorldTimerManager().SetTimer(dashTimer, this, &ABaseCharacter::StopDash, dashDuration, false);
        GetWorldTimerManager().SetTimer(dashRecharge, this, &ABaseCharacter::RechargeDash, dashRechargeTime, false);
        dashContinueTimerDelegate.BindUFunction(this, FName("continueDash"), dir);
        GetWorldTimerManager().SetTimer(dashContinue, dashContinueTimerDelegate, 0.1f, true);

        stamina -= dashStaminaCost;
        
       

    }

}

void ABaseCharacter::continueDash(FVector dir){
  LaunchCharacter(dir, true, false);
}

void ABaseCharacter::StopDash()
{
    
    canBeDamaged = true;    
    canMove = true;
    dashing = false;
    canJumpChar = true;
	//Won't stop without a launch character, it works so...
    LaunchCharacter(FVector(0.5f,0.5f,0.0f), true, false);
    GetCharacterMovement()->GroundFriction = 8.0f;
    GetCapsuleComponent()->SetCollisionProfileName(nameCollisionPreset);
    GetWorld()->GetTimerManager().ClearTimer(dashContinue);
}

void ABaseCharacter::RechargeDash()
{

    canDash = true;

}

void ABaseCharacter::ResetSlowMo(){
  UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ABaseCharacter::ResetMaterial() {
  materialCharacter->SetScalarParameterValue(FName("EnableHolo"), 0);
}


void ABaseCharacter::ApplySlow(float slowPercentage, float duration)
{

    //Apply slow
    //Slows do not stack, max slow is applied
    float newSpeed = defaultWalkSpeed - (defaultWalkSpeed * slowPercentage);
    if (GetCharacterMovement()->MaxWalkSpeed > newSpeed) {
        GetCharacterMovement()->MaxWalkSpeed = newSpeed;
        GetWorldTimerManager().ClearTimer(slowTimer);
        GetWorldTimerManager().SetTimer(slowTimer, this, &ABaseCharacter::ClearSlow, duration, false);

    }

    //First we clear the timer to reset the slow in case we had it applied before but has not finished yet.
    GetWorldTimerManager().ClearTimer(slowTimer);
    GetWorldTimerManager().SetTimer(slowTimer, this, &ABaseCharacter::ClearSlow, duration, false);

}

void ABaseCharacter::ClearSlow()
{
    if (!holdingObject) {
        GetCharacterMovement()->MaxWalkSpeed = defaultWalkSpeed;
    }
    else {
        //do something with the held object to apply its slow
    }
}

/** BLOCKING */

void ABaseCharacter::startBlock()
{

	inputArray.Empty();
	_onCombo = false;
    inputArray.Add(kInput_Block);

  if (!blockIsActive && !blockOnCD && canMove && _attackEnd && stamina >= minimumStamina && !holdingObject && canJumpChar) {

    blockIsActive = true;
    parryIsActive = true;
    canMove = false;
    blockOnCD = true;

    GetWorldTimerManager().SetTimer(parryTimer, this, &ABaseCharacter::stopParry, parryTime, false);
    GetWorldTimerManager().SetTimer(blockCooldownTimer, this, &ABaseCharacter::stopBlockCooldown, 1.0f, false);

  }


}

void ABaseCharacter::stopBlock()
{

    if (blockIsActive) {

        blockIsActive = false;
        canMove = true;
        parryIsActive = false;

    }

    //Clean input queue
    for (int i = 0;i < inputArray.Num(); i++) {

        if (inputArray[i] == kInput_Block) {
            inputArray.Empty();
            break;
        }

    }

    if (IsValid(shieldParticle.particleComponent)) {
      shieldParticle.particleComponent->DeactivateSystem();
    }
    if (IsValid(shieldParticle.particleComponentNiagara)) {
      shieldParticle.particleComponentNiagara->Deactivate();
    }

}
  
void ABaseCharacter::stopParry()
{

  parryIsActive = false;

}

void ABaseCharacter::stopBlockCooldown()
{

    blockOnCD = false;

}

///Have to be modified if you want to add a new stats for the attacks
void ABaseCharacter::resetAttackStats(){
  attackStats.apply_hit = false;
  attackStats.apply_knock_down = false;
  attackStats.apply_stun = false;
  attackStats.attack_dmg = 0;
  attackStats.attack_knockback = 0.0f;
  attackStats.yKnockBackForce = 0.0f;
}

void ABaseCharacter::resetStateStats() {
  state.hit = false;
  state.knockdown = false;
  state.stun = false;
  state.standUp = false;
  canMove = true;
  canJumpChar = true;
  state.alteredStatus = false;
  _attackEnd = true;

  materialCharacter->SetScalarParameterValue(FName("EnableHolo"), 0);
  
  ATurboPlayer* player = Cast<ATurboPlayer>(this);
  if (player) {
      GetCapsuleComponent()->SetCollisionProfileName("Player");
  }
  else {
      GetCapsuleComponent()->SetCollisionProfileName("Enemy");
  }

}

void ABaseCharacter::standUp() {
  state.standUp = true;
}

void ABaseCharacter::HitDamagable(AActor* otherActor){
 
  if(alive){
    stamina += recoveryStaminaPerHit;
    knockBackUnits = otherActor->GetActorLocation() - GetActorLocation();
    knockBackUnits.Normalize();
    knockBackUnits *= attackStats.attack_knockback;
    knockBackUnits.Z = attackStats.yKnockBackForce;
    if (bonus_charging_attack > 0) {
      attackStats.attack_dmg *= (bonus_charging_attack);
    }

    bool hitFromBehind = (truncf(GetActorRotation().Euler().Z) == truncf(otherActor->GetActorRotation().Euler().Z));

    IIDamagable::Execute_TakeAHit(otherActor, knockBackUnits,
      attackStats, hitFromBehind, this);
  }

}

void ABaseCharacter::HitUIUpdate(AActor* otherActor) {


}

void ABaseCharacter::resetPhysicBody() {
  GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(bname, 0.0f);
  GetMesh()->SetAllBodiesBelowSimulatePhysics(bname, false);
}

void ABaseCharacter::startTimerResetPhysicBody(FName){
  GetWorld()->GetTimerManager().SetTimer(resetPhysicsTimer, this, &ABaseCharacter::resetPhysicBody, 0.75f, false);
  
}

void ABaseCharacter::OnDeathImpulse()
{
  //Start burn material


  alive = false;
  canMove = false;

  GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyD"));
  GetWorld()->GetTimerManager().SetTimer(timerDeath, this, &ABaseCharacter::Death, timeToDie, false);
}



void ABaseCharacter::SpawnParticleSystem(FParticleData &particleData, bool attachedToHit, FVector hitLocation) {

  USkeletalMeshComponent* MeshComp = GetMesh();
  
  UNiagaraSystem* NiagaraTemplate = particleData.PSTemplateNiagara;
  UParticleSystem* SystemTemplate = particleData.PSTemplate;
  FVector Location = particleData.locationOffset;
  FRotator Rotation = particleData.rotationOffset;
  FVector Scale = particleData.scale;
  bool Attached = particleData.attached;
  FName Socket = particleData.socketName;
  if (particleData.particleComponentNiagara) {
    particleData.particleComponentNiagara->ResetSystem();
  }
  if (particleData.particleComponent) {
    particleData.particleComponent->ResetParticles();
  }

  if (attachedToHit) {
    Attached = false;
    Location = hitLocation;
  }

  if (NiagaraTemplate)
  {


    if (Attached)
    {
      particleData.particleComponentNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraTemplate,
        MeshComp, Socket, Location, Rotation, Scale, 
        EAttachLocation::KeepRelativeOffset,
        true, ENCPoolMethod::AutoRelease, true, true);
    }
    else
    {

      const FTransform MeshTransform = MeshComp->GetSocketTransform(Socket);
      FVector particleLocation = MeshTransform.TransformPosition(Location);
      if (attachedToHit) {
        particleLocation = Location;
      }
      particleData.particleComponentNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(),
        NiagaraTemplate, particleLocation,
        (MeshTransform.GetRotation() * FQuat(Rotation)).Rotator(),
        Scale, true, true, ENCPoolMethod::None, true);
    }


  }
 

  //CascadeSystem
  if (SystemTemplate)
  {


    if (Attached)
    {
      particleData.particleComponent =
      UGameplayStatics::SpawnEmitterAttached(SystemTemplate,
        MeshComp, Socket, Location, Rotation, Scale,
        EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease, true);
    }
    else
    {
      const FTransform MeshTransform = MeshComp->GetSocketTransform(Socket);
      FVector particleLocation = MeshTransform.TransformPosition(Location);
      if (attachedToHit) {
        particleLocation = Location;
      }
      FTransform SpawnTransform;
      SpawnTransform.SetLocation(particleLocation);
      SpawnTransform.SetRotation(MeshTransform.GetRotation() * FQuat(Rotation));
      SpawnTransform.SetScale3D(Scale);
      particleData.particleComponent = UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), SystemTemplate,
        SpawnTransform, 
        true, EPSCPoolMethod::AutoRelease, true);

    }
  }
}

