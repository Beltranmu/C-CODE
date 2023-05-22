// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BaseEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TurboPlayer.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Player/DestinationPoint.h"
#include "NavigationSystem.h"

ABaseEnemy::ABaseEnemy(){

  //_mainCharacter = nullptr;
  characterLocation = FVector::ZeroVector;
  myLocation = FVector::ZeroVector;
  distanceVector = FVector::ZeroVector;
  _stop_distance = 100.0f;
  targetID = -1;
  hitByPlayerID = 0;
  _combatCooldown = 2.0f;

  hitUpperBodyActive = true;
  hitLowerBodyActive = true;

  PauseX = 2.f;
  PauseY = 2.f;
  PauseAttack = 2.f;
  _refreshLocationValue = 2.f;

  aiController = nullptr;
  GetCharacterMovement()->bUseRVOAvoidance = true;

  resetAttack = true;

  collisionAttackPreset = ECC_GameTraceChannel17;

  justEnteredDangerArea = false;

  pointHasBeenAssigned = false;
  edgePointHasBeenAssigned = false;
  pointOnAttackAssigned = false;

  playerIndex = 0;
  pointIndex = 0;
  edgePointIndex = -1;

  PauseRandomPoint = 1.f;
  rangeRandomPoint = 500.f;

  hairMesh = CreateDefaultSubobject<UStaticMeshComponent>("HairMesh");
  hairMesh->SetupAttachment(GetMesh(),FName("HeadComplementSocket"));
  hairMesh->SetCollisionProfileName("NoCollision");
  hairMesh->SetCanEverAffectNavigation(false);



  complementsMeshes.Add(hairMesh);


  

  complementHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("ComplementMesh");
  complementHeadMesh->SetupAttachment(GetMesh(), FName("HeadComplementSocket"));
  complementHeadMesh->SetCollisionProfileName("NoCollision");
  complementHeadMesh->SetCanEverAffectNavigation(false);
  complementsMeshes.Add(complementHeadMesh);



  middlePointWeight = 0;

  tmpLocation = FVector::ZeroVector;

  helpStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("HelpMesh");
  const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/SuperGrid/TutorialLevel/Utility/Meshes/Utility_ExclamationMark"));
  const ConstructorHelpers::FObjectFinder<UMaterial> MeshMat(TEXT("/Game/Materials/test/RedTest"));
  helpStaticMesh->SetupAttachment(RootComponent);
  helpStaticMesh->SetStaticMesh(MeshObj.Object);
  helpStaticMesh->SetMaterial(0, MeshMat.Object);
  helpStaticMesh->SetCanEverAffectNavigation(false);
  helpStaticMesh->SetCollisionProfileName("NoCollision");
  helpStaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
  
  helpStaticMesh->SetVisibility(false);
  helpStaticMesh->CastShadow = false;

}

void ABaseEnemy::BeginPlay(){
  
  Super::BeginPlay();  
  
  //Disable Mesh collision to not launch the main CH when an enemy dies
  GetMesh()->SetCollisionProfileName("EnemyBones");

  numberOfElements = pickupList.Num();
  health = maxHealth;

  aiController = Cast<AAIController>(GetController());
  GetCharacterMovement()->bUseRVOAvoidance = true;
  //GetWorld()->GetTimerManager().SetTimer(_timerCheckDangerAreaDistance, this, &ABaseEnemy::EnterInDangerZone, _timerCheckDangerAreaDistanceValue, true);
  maxDashCharges = -1;
  nameCollisionPreset = "Enemy";
  GetCapsuleComponent()->SetCollisionProfileName(nameCollisionPreset);

  int n = hairMeshes.Num();
  int m = 0;
  for (m; m < n; m++ ) {
    UMaterialInstanceDynamic* tmpMaterial = UMaterialInstanceDynamic::Create(hairMeshes[m]->GetMaterial(0), hairMeshes[m]);
    hairMeshesMaterials.Add(tmpMaterial);
  }

  n = complementHeadMeshes.Num();
  m = 0;
  for (m; m < n; m++) {
    UMaterialInstanceDynamic* tmpMaterial = UMaterialInstanceDynamic::Create(complementHeadMeshes[m]->GetMaterial(0), complementHeadMeshes[m]);
    complementHeadMeshesMaterials.Add(tmpMaterial);
  }

  n = complementHipMeshes.Num();
  m = 0;
  for (m; m < n; m++) {
    UMaterialInstanceDynamic* tmpMaterial = UMaterialInstanceDynamic::Create(complementHipMeshes[m]->GetMaterial(0), complementHipMeshes[m]);
    complementHipMeshesMaterials.Add(tmpMaterial);
  }  
  
  n = complementFacialHairMeshes.Num();
  m = 0;
  for (m; m < n; m++) {
    UMaterialInstanceDynamic* tmpMaterial = UMaterialInstanceDynamic::Create(complementFacialHairMeshes[m]->GetMaterial(0), complementFacialHairMeshes[m]);
    complementFacialHairMeshesMaterials.Add(tmpMaterial);
  }



  /*tmpMaterial = UMaterialInstanceDynamic::Create(complementHeadMesh->GetMaterial(0), this);
  complementHeadMesh->SetMaterial(0, tmpMaterial);
  meshesMaterial.Add(tmpMaterial);*/

}

void ABaseEnemy::Tick(float DeltaTime)
{
  
  Super::Tick(DeltaTime);

  // Change whenever the EnemyCommander is ready. EC will take control of everything
  // May be moved to the commander unit
  /*if (nullptr == _mainCharacter) {
    _mainCharacter = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
    _playerArray.Add(_mainCharacter);
    _target = _mainCharacter;
  }*/

  

  FVector newScale = rotationAngle == 0.0f ? FVector(1.0f, 1.0f, 1.0f) : FVector(-1.0f, 1.0f, 1.0f);
  for (int m = 0; m < complementsMeshes.Num(); ++m) {
    complementsMeshes[m]->SetRelativeScale3D(newScale);
  }

  for (int m = 0; m < meshesMaterial.Num(); ++m) {
    meshesMaterial[m]->SetScalarParameterValue(FName("Fade"), fade);
    meshesMaterial[m]->SetScalarParameterValue(FName("DeathFade"), dfade);
  }
  
  if (isDiying) {
    diyingTime += DeltaTime;
    float deltaT = diyingTime * (1.0f / timeToDie);
    dfade = FMath::Lerp(-0.75f, 0.75f, deltaT);
    if (spawningTime >= timeToSpawn) {
      diyingTime = 0;
      isDiying = false;
      dfade = 0.75f;
    }

  }

  if (isSpawning) {
    spawningTime += DeltaTime;
    float deltaT = spawningTime * (1.0f / timeToSpawn);
    fade = FMath::Lerp(-1.0f, 1.0f, deltaT);

    if (spawningTime >= timeToSpawn) {
      spawningTime = 0;
      isSpawning = false;
      ResetEnemy();

    }
  }
}

void ABaseEnemy::TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter) {
  Super::TakeAHit_Implementation(knockback_force, attack_info, hitFromBehind, hiter);
  if (!unostopable) {

    if (state.alteredStatus) {
      aiController->StopMovement();
      _timerPauseAttackDelegate.BindUFunction(this, FName("EndPause"), 2);
      GetWorldTimerManager().SetTimer(_timerPauseAttack, _timerPauseAttackDelegate, PauseAttack, false);
      GetWorldTimerManager().ClearTimer(timerToWaitAttack);
    }

    StopAnimMontage(_highAttackAnimationMontage);

  }
  
  updateUIEvent.Broadcast(health / maxHealth, enemyID, characterID, hitByPlayerID);
  helpStaticMesh->SetVisibility(false);


}

void ABaseEnemy::CatchUIDataEnemy_Implementation(int& id, float& percentage, int& points, int playerID)
{
  hitByPlayerID = playerID;
  id = enemyID;
  percentage = health/ maxHealth;
  points = score;
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type type){

  Super::EndPlay(type);
  GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
  
  if(_onDeath.IsBound()){
    _onDeath.RemoveAll(this);
  }

}

float ABaseEnemy::GetDistanceFromTarget(FVector targetLocation){

  FVector tmpVector = targetLocation - _myLocation;
  return tmpVector.Size();

}

int ABaseEnemy::givePoints(int32 points) {

  return points;

}

void ABaseEnemy::Death(){ 
  Super::Death();
  
  health = maxHealth;

  helpStaticMesh->SetVisibility(false);
  aiController->StopMovement();
  if (spawnPickUpActivated) {

    SpawnPickUp();
    
  }

  //alive = true;
  // Tell enemy commander I am dead
  _onDeath.Broadcast(this, targetID);
  // Spawn pick up

  //_mainCharacter = Cast<ATurboPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
  //_mainCharacter->score += 100;
  isDiying = false;
  fade = -1;
  dfade = -0.75f;
  resetStyle();
}

void ABaseEnemy::OnDeathImpulse(){

  Super::OnDeathImpulse();
  aiController->StopMovement();
  canMove = false;
  FVector impulse = deathImpulse;

  impulse.Y *= -GetActorRightVector().Y  ;

  diyingTime = 0.0f;
    isDiying = true;
  LaunchCharacter(impulse, true, true);

}

void ABaseEnemy::SpawnPickUp() {
  uint32 i;
  int lower_limitP = 0;
  int randomN = FMath::RandRange(1, 100);
  for (i = 0; i < numberOfElements; ++i) {
    if(lower_limitP < randomN && randomN < pickupRatio[i] + lower_limitP){
      FActorSpawnParameters spawnParams;
      spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
      FHitResult ground;
      FCollisionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
      TraceParams.AddIgnoredActor(this);
      FVector spawnPosition = GetMesh()->GetSocketLocation("Center");
      spawnPosition += FVector(0.0f, 0.0f, 20.f);
      if(GetWorld()->LineTraceSingleByChannel(ground, GetActorLocation(), GetActorLocation() - FVector(0.0f, 0.0f, 500.0f),
                                           ECC_GameTraceChannel18, TraceParams)){

        //Ricky eres el mejor, te quiero mucho, no se lo digas a javi, es un celoso :D
        spawnPosition.Z = GetActorLocation().Z;
      
      }
      AActor* pickup = GetWorld()->SpawnActor<AActor>(pickupList[i], spawnPosition, FRotator::ZeroRotator, spawnParams);
      if (IsValid(pickup)) { pickup->SetLifeSpan(15.0f); }
    }
    lower_limitP += pickupRatio[i];
  }
 //  int32 i = FMath::RandRange(0, numberOfElements);

}

// Artificial Intelligence
void ABaseEnemy::ChooseTarget(){
 
  
}

void ABaseEnemy::RotateEnemy() {


  if (canMove && alive && !state.alteredStatus){
      if (_mainCharacter.Num() != 0) {
          float characterY = _mainCharacter[playerIndex]->GetActorLocation().Y;
          distanceVector.Y = characterY - GetActorLocation().Y;

          FRotator nextRotation = FRotator::ZeroRotator;

          if (distanceVector.Y > 10.f) {
              rotationAngle = 0.0f;
          }
          else if (distanceVector.Y < -10.f)
              rotationAngle = 180.0f;

      }
    
  }
}

void ABaseEnemy::GetLocationFromTarget(){
  if (IsValid(_target)) {
    _targetLocation = _target->GetActorLocation();
  }
}

void ABaseEnemy::ResetEnemy(){
  
  alive = true;

  canMove = true;
  _state2 = kState_StandBy;
  stamina = maxStamina;
  health = maxHealth;

  GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

  resetAttack = true;
  fade = 1.0f;

  materialCharacter->SetScalarParameterValue(FName("EnableHolo"), 0);

  //Reset the timer
  //GetWorld()->GetTimerManager().ClearTimer();
}


void ABaseEnemy::setNewStyle() {

  meshesMaterial.Reset();
  int numberHairs = hairMeshes.Num();
   int32 idHair = FMath::RandRange(0, numberHairs -1);
  if (numberHairs != 0) {
    hairMesh->SetStaticMesh(hairMeshes[idHair]);
    hairMesh->SetMaterial(0, hairMeshesMaterials[idHair]);
    meshesMaterial.Add(hairMeshesMaterials[idHair]);
  }
  
  int32 idComplement = FMath::RandRange(0, complementHeadMeshes.Num());
  if (idComplement < complementHeadMeshes.Num()) {
    complementHeadMesh->SetStaticMesh(complementHeadMeshes[idComplement]);
    complementHeadMesh->SetMaterial(0, complementHeadMeshesMaterials[idComplement]);
    meshesMaterial.Add(complementHeadMeshesMaterials[idComplement]);
  }

  int m = 0;
  int n = complementsMeshes.Num();
  for (m = 0; m < n; m++) {
    complementsMeshes[m]->SetVisibility(true);
  }

}

void ABaseEnemy::resetStyle() {
  int m = 0;
  for (m = 0; m < complementsMeshes.Num(); m++) {
    complementsMeshes[m]->SetVisibility(false);
  }
}

void ABaseEnemy::ResetTimeDilatation() {

  UWorld* tmpWorld = GetWorld();
  UGameplayStatics::SetGlobalTimeDilation(tmpWorld, 1);

}

FVector ABaseEnemy::GetLocationTarget() {
  return _mainCharacter[playerIndex]->GetActorLocation();
}

void ABaseEnemy::EndPause(int32 type) {

  switch(type){

    case 0: _state2 = kState_MoveX; break;
    case 1:
      
      if(currentTriesBeforeChasing > triesBeforeChasing){

        _state2 = kState_Chase2;
        currentTriesBeforeChasing = 0;

      }else{

        if (abs(GetLocationTarget().X - GetActorLocation().X) <= 40.f) {

          _state2 = kState_MoveY;

        } else {

          _state2 = kState_GetLocation;
          ++currentTriesBeforeChasing;

        }
      }

     break;

    case 2:
      resetAttack = true;
      _state2 = kState_InRange;
      GetWorld()->GetTimerManager().SetTimer(_timerCheckDangerAreaDistance, this, &ABaseEnemy::EnterInDangerZone, _timerCheckDangerAreaDistanceValue, true);
    break;

    case 3: _state2 = kState_GetRandomPoint; break;

    case 4: _state2 = kState_GetRandomInteriorPoint; break;

  }
 
}

void ABaseEnemy::StateMachine2(InputType attackMove){

  float acceptanceRadius = -1.f;

  UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

  FNavLocation navLocation;

  bool result = false;

  if(alive && !state.alteredStatus){
    specialThink();
    PlayerDetection();
    FVector v1 = FVector::ZeroVector;
    FVector v2 = FVector::ZeroVector;
    FVector tmpDst = FVector::ZeroVector;

    RotateEnemy();

    switch(_state2){

      case ABaseEnemy::kState_StandBy:
      break;

      case ABaseEnemy::kState_GoToEdge:

        if (EPathFollowingRequestResult::AlreadyAtGoal == aiController->MoveToLocation(_targetLocation, acceptanceRadius, true, true, true)) {

          _state2 = kState_StandBy;
          //_state2 = kState_GetRandomPoint;
          _timerPauseRandomPointDelegate.BindUFunction(this, FName("EndPause"), 3);
          GetWorldTimerManager().SetTimer(_timerPauseRandomPoint, _timerPauseRandomPointDelegate, PauseRandomPoint, false);
          helpStaticMesh->SetVisibility(false);
        }

      break;

      case ABaseEnemy::kState_GetRandomPoint:

        _getNewPoint.Broadcast(this, 2, 0);
        _state2 = kState_GoToEdge;

      break;

      case ABaseEnemy::kState_GetRandomInteriorPoint:

        _getNewPoint.Broadcast(this, 3, 0);
        _state2 = kState_Chase2;

      break;

      case ABaseEnemy::kState_GetLocation:

        GetLocationFromTarget();                
        _state2 = kState_PauseX;

      break;

      case ABaseEnemy::kState_PauseX:

        _timerPauseXDelegate.BindUFunction(this, FName("EndPause"), 0);
        GetWorldTimerManager().SetTimer(_timerPauseX, _timerPauseXDelegate, PauseX, false);

        /*_timerPauseRethinkDelegate.BindUFunction(this, FName("EndPause"), 3);
        GetWorldTimerManager().SetTimer(_timerPauseRethink, _timerPauseRethinkDelegate, PauseRethink, false);*/

        _state2 = kState_StandBy;

      break;

      case ABaseEnemy::kState_PauseY:

        _timerPauseYDelegate.BindUFunction(this, FName("EndPause"), 1);
        GetWorldTimerManager().SetTimer(_timerPauseY, _timerPauseYDelegate, PauseY, false);

        /*_timerPauseRethinkDelegate.BindUFunction(this, FName("EndPause"), 3);
        GetWorldTimerManager().SetTimer(_timerPauseRethink, _timerPauseRethinkDelegate, PauseRethink, false);*/
        
        _state2 = kState_StandBy;

      break;

      case ABaseEnemy::kState_MoveX:
      
        tmpDst = _targetLocation;
        tmpDst.Y = GetActorLocation().Y;
        if(tmpDst.X < GetActorLocation().X){
          tmpDst.X -= 34.f;
        }else if(tmpDst.X > GetActorLocation().X){
          tmpDst.X += 34.f;
        }
        if (canMove) {
          if (EPathFollowingRequestResult::AlreadyAtGoal == aiController->MoveToLocation(tmpDst)) {
            _state2 = kState_PauseY;
          }
        }

      break;

      case ABaseEnemy::kState_MoveY:

        tmpDst = _targetLocation;                
        tmpDst.X = GetActorLocation().X;
        if (tmpDst.Y < GetActorLocation().Y) {
          tmpDst.Y += 50.f;
        }else if (tmpDst.Y > GetActorLocation().Y) {
          tmpDst.Y -= 50.f;
        }
        if (canMove) {
          if (EPathFollowingRequestResult::AlreadyAtGoal == aiController->MoveToLocation(tmpDst)) {
            _state2 = kState_InRange;

          }/*else{

            if (currentTriesBeforeChasing <= triesBeforeChasing) {

              _state2 = kState_GetLocation;
              ++currentTriesBeforeChasing;

            }else {

              _state2 = kState_Chase2;
              currentTriesBeforeChasing = 0;

            }
          }*/
        }
        
      break;

      case ABaseEnemy::kState_InRange:
        
        //currentTriesBeforeChasing = 0;
        _targetDistance = _mainCharacter[playerIndex]->GetActorLocation() - GetActorLocation();
                          
        if (_targetDistance.Size() <= 100.f) {
          _state2 = kState_Wait2Attack;
        } else {          
          //_state2 = kState_GetLocation;          
          _state2 = kState_RefreshLocation;          
        }

      break;

      case ABaseEnemy::kState_Wait2Attack:
        
        _state2 = kState_StandBy;
        GetWorld()->GetTimerManager().SetTimer(timerToWaitAttack, this, &ABaseEnemy::endWaitAttack, timeWaitToAttack, false);
        
       break;

      case ABaseEnemy::kState_Attack2:
        selectAttack();
        Attack(attackMove);
        _timerPauseAttackDelegate.BindUFunction(this, FName("EndPause"), 2);
        GetWorldTimerManager().SetTimer(_timerPauseAttack, _timerPauseAttackDelegate, PauseAttack + timeWaitToAttack, false);
        _state2 = kState_StandBy;

      break;

      case ABaseEnemy::kState_SpecialBehaviour:

        specialBehavior();

      break;

      case ABaseEnemy::kState_Chase2:

        result = navSystem->ProjectPointToNavigation(_targetLocation, navLocation);

        if(result) {

          tmpLocation = _targetLocation;
  
        }
          
        if (EPathFollowingRequestResult::AlreadyAtGoal == aiController->MoveToLocation(tmpLocation, acceptanceRadius, true, true, true)) {

        // Check interior points
          if(!pointOnAttackAssigned) {
            _state2 = kState_GetAttackPoint;
          }else{
            _state2 = kState_RefreshLocation;
          }

        }

      break;

      case ABaseEnemy::kState_RefreshLocation:

        _state2 = kState_StandBy;
        GetWorld()->GetTimerManager().SetTimer(_refreshLocationTimer, this, &ABaseEnemy::RefreshLocation, _refreshLocationValue, false);

      break;

      case ABaseEnemy::kState_GetAttackPoint:

        _onAttackEvent.Broadcast(this);

        if(pointOnAttackAssigned) {
          _state2 = kState_Chase2;
        }else{
          _state2 = kState_StandBy;
          _timerPauseRandomPointDelegate.BindUFunction(this, FName("EndPause"), 4);
          GetWorldTimerManager().SetTimer(_timerPauseRandomPoint, _timerPauseRandomPointDelegate, PauseRandomPoint, false);
        }

      break;

    }


  }

}

void ABaseEnemy::PlayerDetection() {

  FHitResult hitResult;
  FQuat tmpRot = FQuat(EForceInit::ForceInitToZero);
  FCollisionQueryParams collisionParams;
  collisionParams.TraceTag = "SphereTag";
  bool canDetect = _state2 != kState_ChargingAttack;
  canDetect = canDetect && resetAttack;
  canDetect = canDetect && resetAttack;
  float sizeDetection = 80.0f;
  float sizeRadiusDetection = 30.0f;
  if (canDetect) {
    if (GetWorld()->SweepSingleByChannel(hitResult,
      GetActorLocation() + (GetActorRightVector() * sizeDetection),
      GetActorLocation() + (GetActorRightVector() * sizeDetection),
      tmpRot, ECC_GameTraceChannel15, FCollisionShape::MakeSphere(sizeRadiusDetection), collisionParams) && resetAttack) {
      resetAttack = false;

      helpStaticMesh->SetVisibility(true);
      _state2 = kState_Wait2Attack;
      aiController->StopMovement();
      //GetWorldTimerManager().ClearTimer(_timerCheckDangerAreaDistance);
      GetWorldTimerManager().ClearTimer(_timerCheckDangerAreaDistance);
    }
  }
}

void ABaseEnemy::EnterInDangerZone() {

  FVector enemyLocation = FVector::ZeroVector;
  FVector chLocation = FVector::ZeroVector;
  float currentDistance = 0.f;

  for (int i = 0; i < _mainCharacter.Num(); ++i) {

    enemyLocation = GetActorLocation();
    chLocation = _mainCharacter[i]->GetActorLocation();
    currentDistance = (enemyLocation - chLocation).Size();

    if (currentDistance <= dangerAreaDistance) {

      if (!pointHasBeenAssigned) {
        _getNewPoint.Broadcast(this, 1, 0);
        _state2 = kState_Chase2;
        GetWorldTimerManager().ClearTimer(_timerPauseRandomPoint);
      }

    }
    else {

      if (!edgePointHasBeenAssigned && pointHasBeenAssigned) {
        _state2 = kState_GoToEdge;
        _getNewPoint.Broadcast(this, 0, 0);
        pointHasBeenAssigned = false;
        pointOnAttackAssigned = false;
        _mainCharacter[playerIndex]->targetPoint[pointIndex]->isFree = true;
        GetWorld()->GetTimerManager().ClearTimer(_refreshLocationTimer);
      }

    }

  }
}

void ABaseEnemy::BecomeAlive() {

  GetWorld()->GetTimerManager().SetTimer(_timerCheckDangerAreaDistance, this, &ABaseEnemy::EnterInDangerZone, _timerCheckDangerAreaDistanceValue, true);
  _getNewPoint.Broadcast(this, 0, 0);
  _state2 = ABaseEnemy::kState_GoToEdge;  

}

void ABaseEnemy::RefreshLocation() {

  FVector tmpLocation2 = FVector::ZeroVector;
  float zLocation = 0.f;

  // Raycast to the ground
  FHitResult ground;
  FCollisionQueryParams TraceParams(FName(TEXT("SpawnTrace")), false);
  TraceParams.AddIgnoredActor(this);

  if(!(GetActorLocation() - _mainCharacter[playerIndex]->targetPoint[pointIndex]->GetComponentLocation()).IsNearlyZero(50.f)) {
    _targetLocation = _mainCharacter[playerIndex]->targetPoint[pointIndex]->GetComponentLocation();

    tmpLocation2 = _mainCharacter[playerIndex]->targetPoint[pointIndex]->GetComponentLocation();

    if (GetWorld()->LineTraceSingleByChannel(ground, tmpLocation2, tmpLocation2 - FVector(0.0f, 0.0f, 500.0f),
      ECC_GameTraceChannel5, TraceParams)) {

      zLocation = ground.GetActor()->GetActorLocation().Z;
    }

    _targetLocation = FVector(tmpLocation2.X, tmpLocation2.Y, zLocation);

    if(pointIndex == 0 || pointIndex == 1) {

      if(_mainCharacter[playerIndex]->GetActorLocation().X < GetActorLocation().X) {

        _targetLocation.X -= 50.f;

      } else if (_mainCharacter[playerIndex]->GetActorLocation().X > GetActorLocation().X) {

        _targetLocation.X += 50.f;

      }

    }

  }
  
  _state2 = kState_Chase2;

}

void ABaseEnemy::endWaitAttack() {

  _state2 = kState_Attack2;

  helpStaticMesh->SetVisibility(false);
}

void ABaseEnemy::specialThink(){}
void ABaseEnemy::specialBehavior(){}

void ABaseEnemy::HitDamagable(AActor* hitedActor) {
  //attackStats.knockbackForce.Y +=  defaultKnockbackForce.Y * GetActorRightVector().Y;
  attackStats.attack_knockback +=  defaultKnockbackForce.Y;
  attackStats.yKnockBackForce +=  defaultKnockbackForce.Z;
  if (specialVarient) {
    attackStats.attack_dmg += specialExtraDMG;
  }
 attackStats.attack_dmg += extraDMGLevel;
  Super::HitDamagable(hitedActor);
}


