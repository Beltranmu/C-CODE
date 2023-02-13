// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Player/InteractComponent.h"

#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

#include "Components/BoxComponent.h"
// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;
  //Intreac_Area = CreateDefaultSubobject<UBoxComponent>("I Area");
  // ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
  Super::BeginPlay();

  interaction_lenght = 100.0f * 1.5f; // <- meters
  Interact_pos = GetOwner()->GetActorLocation();
  Interact_pos.X += 100.0f;
  can_interact = true;

  interact_cooldown_timer = 0.0f;
  interact_cooldown = 1.0f;

  GetWorld()->GetTimerManager().SetTimer(Interact_timer, this, &UInteractComponent::InteractTimer, 0.1f, true);
      
  // ...

}

void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  if (InteractEvent.IsBound()) {
    InteractEvent.RemoveAll(this);
  }
}

void UInteractComponent::InteractTimer() {

  //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::White, "Hola");
  if (false == can_interact) {
    interact_cooldown_timer += 0.1f;
    if (interact_cooldown_timer >= interact_cooldown) {
      can_interact = true;
      interact_cooldown_timer = 0.0f;
    }
  }

}

// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

bool UInteractComponent::CanInteract(FVector v_direction) {
  FCollisionQueryParams TraceParams(FName(TEXT("InteractionTrace")), false);

  FHitResult Hit;
  FVector Start = GetOwner()->GetActorLocation();
  FVector end = Start + v_direction * interaction_lenght;
  FVector end1 = Start + v_direction * interaction_lenght + FVector::UpVector * 30.0f;;
  FVector end2 = Start + v_direction * interaction_lenght + FVector::DownVector * 30.0f;;
  FVector end3 = Start + v_direction * interaction_lenght + FVector::RightVector * 30.0f;;
  FVector end4 = Start + v_direction * interaction_lenght + FVector::LeftVector * 30.0f;;

  GetWorld()->LineTraceSingleByChannel(Hit, Start, end, ECC_WorldStatic, TraceParams)
    && GetWorld()->LineTraceSingleByChannel(Hit, Start, end1, ECC_WorldStatic, TraceParams)
    && GetWorld()->LineTraceSingleByChannel(Hit, Start, end2, ECC_WorldStatic, TraceParams)
    && GetWorld()->LineTraceSingleByChannel(Hit, Start, end3, ECC_WorldStatic, TraceParams)
    && GetWorld()->LineTraceSingleByChannel(Hit, Start, end4, ECC_WorldStatic, TraceParams);
  GetWorld()->DebugDrawTraceTag = "InteractionTrace";

  return true;
}

void UInteractComponent::EVInteract(FVector v_direction) {
  
  if (true == can_interact) {
    FCollisionQueryParams CollisionParams(FName(TEXT("CheckEnvironment")), false); // Sphere Raycast Parameters
    CollisionParams.AddIgnoredActor(GetOwner());
    Interact_pos = GetOwner()->GetActorLocation() + 100 * v_direction;
    //Cooldown

    TArray<FOverlapResult> overlapResult; // Raycast Storage 
    if (GetWorld()->OverlapMultiByChannel(overlapResult, Interact_pos, FQuat(0.0f, 0.0f, 0.0f, 0.0f), ECC_GameTraceChannel5,
        FCollisionShape::MakeBox(FVector(100.0, 100.0f, 100.0f)), CollisionParams)) {
            
      InteractEvent.Broadcast(overlapResult[0].GetActor(), 1);

      //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "PAELLA>MOLLETE");

    }

    can_interact = false;

    //GetWorld()->DebugDrawTraceTag = "CheckEnvironment";

  }

}