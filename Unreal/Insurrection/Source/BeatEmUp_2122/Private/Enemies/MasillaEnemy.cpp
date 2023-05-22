// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/MasillaEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "..\..\Public\Enemies\MasillaEnemy.h"

AMasillaEnemy::AMasillaEnemy() {
	_state2 = kState_GoToEdge;
  explosionBehaviour = false;
  timeChangeColor = 0.3f;
  enemyType = TypeOfEnemy_Masilla;

  complementHipMesh = CreateDefaultSubobject<UStaticMeshComponent>("HipComplementMesh");
  complementHipMesh->SetupAttachment(GetMesh(), FName("HipComplementSocket"));
  complementHipMesh->SetCollisionProfileName("NoCollision");
  complementHipMesh->SetCanEverAffectNavigation(false);
  complementsMeshes.Add(complementHipMesh);

  complementFacialHair1Mesh = CreateDefaultSubobject<UStaticMeshComponent>("FacialHair1Mesh");
  complementFacialHair1Mesh->SetupAttachment(GetMesh(), FName("HeadComplementSocket"));
  complementFacialHair1Mesh->SetCollisionProfileName("NoCollision");
  complementFacialHair1Mesh->SetCanEverAffectNavigation(false);
  complementsMeshes.Add(complementFacialHair1Mesh);

  complementFacialHair2Mesh = CreateDefaultSubobject<UStaticMeshComponent>("FacialHair2Mesh");
  complementFacialHair2Mesh->SetupAttachment(GetMesh(), FName("HeadComplementSocket"));
  complementFacialHair2Mesh->SetCollisionProfileName("NoCollision");
  complementFacialHair2Mesh->SetCanEverAffectNavigation(false);
  complementsMeshes.Add(complementFacialHair2Mesh);

  complementFacialHair3Mesh = CreateDefaultSubobject<UStaticMeshComponent>("FacialHair3Mesh");
  complementFacialHair3Mesh->SetupAttachment(GetMesh(), FName("HeadComplementSocket"));
  complementFacialHair3Mesh->SetCollisionProfileName("NoCollision");
  complementFacialHair3Mesh->SetCanEverAffectNavigation(false);
  complementsMeshes.Add(complementFacialHair3Mesh);

}

void AMasillaEnemy::BeginPlay()
{
	Super::BeginPlay();
  
  colorExplosion = 1;

}

void AMasillaEnemy::EndPlay(const EEndPlayReason::Type type)
{
  GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AMasillaEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StateMachine2(kInput_X);

  FRotator newRotator = rotationAngle == 0.0f ? FRotator(0.0f, 90.0f, 0.0f) : FRotator(0.0f, -90.0f, 0.0f);
  GetMesh()->SetRelativeRotation(newRotator);


}



void AMasillaEnemy::ResetEnemy() {
  Super::ResetEnemy();
  timeChangeColor = 0.3f;
}

void AMasillaEnemy::setNewStyle()
{
  Super::setNewStyle();

  int32 idHipComplement = FMath::RandRange(0, complementHipMeshes.Num());
  if(idHipComplement < complementHipMeshes.Num()){
 
    complementHipMesh->SetStaticMesh(complementHipMeshes[idHipComplement]);
    complementHipMesh->SetMaterial(0, complementHipMeshesMaterials[idHipComplement]);
    meshesMaterial.Add(complementHipMeshesMaterials[idHipComplement]);
  }

  int32 idFacialHairComplement = FMath::RandRange(0, 1);
  if (idFacialHairComplement == 0 && complementFacialHairMeshes.Num() > 0) {

    complementFacialHair1Mesh->SetStaticMesh(complementFacialHairMeshes[0]);
    complementFacialHair1Mesh->SetMaterial(0, complementFacialHairMeshesMaterials[0]);
    meshesMaterial.Add(complementFacialHairMeshesMaterials[0]);

  }


  idFacialHairComplement = FMath::RandRange(0, 1);
  if (idFacialHairComplement == 0 && complementFacialHairMeshes.Num() > 1) {

    complementFacialHair2Mesh->SetStaticMesh(complementFacialHairMeshes[1]);
    complementFacialHair2Mesh->SetMaterial(0, complementFacialHairMeshesMaterials[1]);
    meshesMaterial.Add(complementFacialHairMeshesMaterials[1]);
  }

  idFacialHairComplement = FMath::RandRange(0, 1);
  if (idFacialHairComplement == 0 && complementFacialHairMeshes.Num() > 2) {

    complementFacialHair3Mesh->SetStaticMesh(complementFacialHairMeshes[2]);
    complementFacialHair3Mesh->SetMaterial(0, complementFacialHairMeshesMaterials[2]);
    meshesMaterial.Add(complementFacialHairMeshesMaterials[2]);
  }

  
}


void AMasillaEnemy::TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter) {
  if(attack_info.upper_body && !hitUpperBodyActive){
    attack_info.attack_dmg = 0;
  }
  if (attack_info.lower_body && !hitLowerBodyActive) {
    attack_info.attack_dmg = 0;
  }

  Super::TakeAHit_Implementation(knockback_force, attack_info, hitFromBehind, hiter);

}


void AMasillaEnemy::OnDeathImpulse() {

  if (explosionBehaviour /*&& (health < hpToExplode) && _state2 != ABaseEnemy::EnemyState::kState_SpecialBehaviour*/) {
    //Activar Explosion y Despues de x tiempo explota
    GetWorld()->GetTimerManager().SetTimer(timerExplosion, this, &AMasillaEnemy::Explode, timeToDie - 0.1f, false);
    GetWorld()->GetTimerManager().SetTimer(timerChangeColorExplosion, this, &AMasillaEnemy::ChangeColorExplosion, timeChangeColor , false, (timeToDie - timeOfExplosion));
    //_state2 = ABaseEnemy::EnemyState::kState_SpecialBehaviour;
  }
  Super::OnDeathImpulse();

}



void AMasillaEnemy::ChangeColorExplosion(){

  colorExplosion *= -1;
  if(colorExplosion > 0){
    SpawnParticleSystem(PSTemplateWarningExplsion1);
    PSTemplateWarningExplsion2.locationOffset.Z *= -1;
    PSTemplateWarningExplsion2.locationOffset.X *= FMath::RandRange(-15.0f, 15.0f);
    PSTemplateWarningExplsion2.locationOffset.Y *= FMath::RandRange(-15.0f, 15.0f);
  }
  else{
    SpawnParticleSystem(PSTemplateWarningExplsion2);
    PSTemplateWarningExplsion1.locationOffset.Z *= -1;
    PSTemplateWarningExplsion1.locationOffset.X *= FMath::RandRange(-15.0f, 15.0f);
    PSTemplateWarningExplsion1.locationOffset.Y *= FMath::RandRange(-15.0f, 15.0f);
  }
  // 2-> 0.86 1-> 0.81
  if(timeChangeColor >= 0.01666f){
    timeChangeColor *= 0.81f ;
  }
  GetWorld()->GetTimerManager().SetTimer(timerChangeColorExplosion, this, &AMasillaEnemy::ChangeColorExplosion, timeChangeColor, false);
}

void AMasillaEnemy::Explode(){

  FCollisionShape colSphere = FCollisionShape::MakeSphere(explosionRadius);

  //Particle Explosion

  //
  FVector explosionPosition = GetMesh()->GetSocketLocation("Center");
 


  SpawnParticleSystem(deathParticle);
    
  TArray<FHitResult> hittedActors;
  if(GetWorld()->SweepMultiByChannel(hittedActors, explosionPosition,GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel13, colSphere)){

    for(auto &hittedActor : hittedActors){

      AActor* actor =  hittedActor.GetActor();
      if(actor && actor != this){
        if (actor->GetClass()->ImplementsInterface(UIDamagable::StaticClass())) {

          float actorExplosionDirection = explositonDirection;
          float explosionPower =  explosionForce;
          if ((actor->GetActorLocation().Y - this->GetActorLocation().Y) < 0.0f) {
            actorExplosionDirection = 180 - explositonDirection;
          }
          float angle = FMath::DegreesToRadians(actorExplosionDirection);
          FVector explosionVector = FVector(0.0f, explosionPower * FMath::Cos(angle), explosionPower * FMath::Sin(angle));
         

          //HERE FOR MORE ACTION FOR THE EXPLOSION
          attackStats.attack_dmg = explosionDMG;
          attackStats.apply_stun = true;
          //attackStats.attack_knockback = explosionForce;
          ABaseEnemy* hitEnemy = Cast<ABaseEnemy>(actor);
          if (hitEnemy) {
            hitEnemy->hitByPlayerID = hitByPlayerID;
          }
          IIDamagable::Execute_TakeAHit(actor, explosionVector, attackStats, false, this);

        }
      }
    }
  }
  //interrumpir el timer de cambio de color
    GetWorld()->GetTimerManager().ClearTimer(timerChangeColorExplosion);
  //Super::Death();
};

void AMasillaEnemy::selectAttack() {

  if (rand() % 2 == 0) {
    _highAttackAnimationMontage = _fistOne;
  }
  else {
    _highAttackAnimationMontage = _fistTwo;
  }

}