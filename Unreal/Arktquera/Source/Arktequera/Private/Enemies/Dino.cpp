// Fill out your copyright notice in the Description page of Project Settings.

#include <Enemies/Dino.h>

#include <Components/SkeletalMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Engine.h>
#include <Enemies/DinoMovementComponent.h>
#include <Math/Vector.h>
#include <Math/UnrealMathUtility.h>
#include <Components/SphereComponent.h>
#include <Player/HealthComponent.h>

// Sets default values
ADino::ADino()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  dinoMesh_ = CreateDefaultSubobject<USkeletalMeshComponent>("Dino Mesh");
  box_ = CreateDefaultSubobject<UBoxComponent>("Dino Box");
  dinoMovement_ = CreateDefaultSubobject<UDinoMovementComponent>("Dino Movement");
  sphereCollider_ = CreateDefaultSubobject<USphereComponent>("My Sphere");
  myHealth_ = CreateDefaultSubobject<UHealthComponent>("Healt_dino");

  dinoRaycastFront = CreateDefaultSubobject<USceneComponent>("Dino Raycast Front");
  dinoRaycastCenter = CreateDefaultSubobject<USceneComponent>("Dino Raycast Center");
  dinoRaycastBack = CreateDefaultSubobject<USceneComponent>("Dino Raycast Back");

  RootComponent = box_;
  dinoMesh_->SetupAttachment(RootComponent);
  dinoMovement_->UpdatedComponent = box_;
  sphereCollider_->SetupAttachment(RootComponent);

  dinoRaycastFront->SetupAttachment(RootComponent);
  dinoRaycastCenter->SetupAttachment(RootComponent);
  dinoRaycastBack->SetupAttachment(RootComponent);

  // Movement
  speed_ = 600.0f;
  distance_ = 1000.f;
  initialLocation_ = FVector(ForceInitToZero);
  currentLocation_ = FVector(ForceInitToZero);
  currentRot_ = FRotator(ForceInitToZero);
  nextRot_ = FRotator(ForceInitToZero);
  new_normal = FVector(0.0f, 0.0f, 1.0f);

  currentTime_ = 0.f;
  timeObjective_ = 3.f;
  movementMode_ = 1;
  startMovement_ = true;
  new_time_to_move = (rand() % 5) + 5;

}

// Called when the game starts or when spawned
void ADino::BeginPlay()
{
  Super::BeginPlay();
  this->OnActorBeginOverlap.AddDynamic(this, &ADino::DetectHuman);
  this->OnActorEndOverlap.AddDynamic(this, &ADino::StopChasing);
  myHealth_->setHealth(initial_hp);


  GetWorld()->GetTimerManager().SetTimer(Timer_mov, this, &ADino::TimerMov, 1.0f, true);

}

// Called every frame
void ADino::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  Walk(DeltaTime);

}

void ADino::TimerMov() {
  time_new_mov += 1.0f;
  //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, "Ün sgundo menos");
}

// Called to bind functionality to input
void ADino::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADino::Walk(float DeltaTime) {

  switch (movementMode_) {

  case 1:

    if (startMovement_) {
      initialLocation_ = GetActorLocation();
      newDirection_ = dinoMovement_->getNextMovement();
      nextRot_ = newDirection_.Rotation();
      currentRot_ = GetActorRotation();
      currentTime_ = 0.f;
      startMovement_ = false;
    }

    break;

  case 2:

    currentLocation_ = GetActorLocation();
    FVector newDirection = target_->GetActorLocation() - currentLocation_;
    newDirection.Z = 0.0f;
    newDirection.Normalize();
    nextRot_ = newDirection.Rotation();

    break;

  }



  FHitResult hit;

  FVector spawn_pos = dinoRaycastFront->GetComponentLocation();
  FVector end_pos = spawn_pos + 500.0f * FVector(0.0f, 0.0f, -1.0f);

  if (GetWorld()->LineTraceSingleByChannel(hit, spawn_pos, end_pos, ECC_GameTraceChannel6)) {
    new_normal = hit.Normal;

    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, GetActorRotation().ToString());

  }
  float angle = acos(new_normal.DotProduct(new_normal, FVector(0.0f, 0.0f, 1.0f)));
  currentRot_.Pitch = angle * 180.0f / 3.1415f;

  //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "HOLAA");


  currentLocation_ = GetActorLocation();

  currentTime_ += DeltaTime;

  FVector newLocation = currentLocation_ - initialLocation_;
  float currentDistance = newLocation.Size();

  FString dst = FString::SanitizeFloat(currentDistance);
  currentTime_ = FMath::Clamp(currentTime_, 0.f, timeObjective_);
  SetActorRotation(FMath::Lerp(currentRot_, nextRot_, currentTime_ / timeObjective_));

  FString time = FString::SanitizeFloat(currentTime_);

  float deltaRot = currentRot_.Yaw - nextRot_.Yaw;

  // Move when new angle has been reached
  //if(deltaRot <= 0.5f && currentDistance <= distance_)
  dinoMovement_->AddInputVector(GetActorForwardVector() * speed_ * GetWorld()->GetDeltaSeconds());

  if (time_new_mov > new_time_to_move) {
    startMovement_ = true;
    time_new_mov = 0.0f;
    new_time_to_move = (rand() % 5) + 4;
  }
}

void ADino::DetectHuman(AActor* OverlappedActor, AActor* OtherActor) {

  movementMode_ = 2;
  target_ = OtherActor;
  currentRot_ = GetActorRotation();
  currentTime_ = 0.f;
  sphereCollider_->SetSphereRadius(50.f);

}

void ADino::StopChasing(AActor * OverlappedActor, AActor * OtherActor)
{

  movementMode_ = 1;
  target_ = nullptr;
  currentTime_ = 0.f;
  currentRot_ = GetActorRotation();
  sphereCollider_->SetSphereRadius(32.f);

}

void ADino::NativeGetAttacked_Implementation(float dmg) {
  myHealth_->setHealth(myHealth_->Health() - dmg);
  if (myHealth_->CanIDie() == true) {
    Destroy();
  }
  GEngine->AddOnScreenDebugMessage(-1, 3, FColor::White, "EIEIE, te quito vida");
}