// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Player/BuildComponent.h"

#include "CollisionQueryParams.h"
#include "../../Public/Structure/Structure.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Components/PrimitiveComponent.h"
#include <Player/PlayerInventory.h>

// Sets default values for this component's properties
UBuildComponent::UBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
  build_height = 100.0f * 1.5f; //<- In meters
  build_distance = 100.0f * 3.5f; //<- In meters
  structure_type = 0;
  no_build_location = FVector(1000000.0f, 1000000.0f, -1000000.0f);

  max_build_comp = new_structure.Max();
  AStructure* tmp_structure;
  FTransform new_transform = FTransform(FVector(0.0f, 0.0f, 0.0f).Rotation(),
  no_build_location, FVector(1.0f, 1.0f, 1.0f));

  for (int i = 0; i < max_build_comp; ++i) {
    tmp_structure = GetWorld()->SpawnActorDeferred<AStructure>(new_structure[i], new_transform);
    preview_structure.Add(tmp_structure);
  }
  
  isSpawnable = false;
	
}

// Called every frame
void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuildComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  if (updateEvent.IsBound()) {
    updateEvent.RemoveAll(this);
  }
}

void UBuildComponent::WhereToBuild(FVector v_direction) {

  // Floor Raycast
  FCollisionQueryParams TraceParams(FName(TEXT("BuildTrace")), false);
  FHitResult Hit;
  build_location = GetOwner()->GetActorLocation() + v_direction * build_distance;
  FVector end = build_location + FVector::DownVector * build_height;

  // Temporal Variables
  FString tmpString;

  // Sphere Raycast
  FCollisionQueryParams CollisionParams(FName(TEXT("CheckStructures")), false); // Sphere Raycast Parameters
  CollisionParams.AddIgnoredActor((AActor*)this);
  FQuat deltaRot = FQuat(build_rotation); // Sphere Rotation in Quaternions
  TArray<FOverlapResult> overlapResult; // Raycast Storage 
  FCollisionResponseParams sphereCollision; // Respons Params
  FVector spawnedActorLocation; // Location of any structure detected
  FRotator spawnedActorRotation;  // Rotation of any structure detected
  FVector forwardVector;  // Forward Vector of any structure detected
  FVector tmpLocation;  // Vector between spawned and preview structures
  float tmpX, tmpY; // tmpLocation X and Y components in absolute value so that they can be compared

  uint32 ID;  // Spanwed structure ID/type (foundation, wall...)      
  AStructure* myStructure;  // Storages any structure detected in the map


  // Main Switch
  switch(structure_type){

    case 0: // Foundation

      if (GetWorld()->LineTraceSingleByChannel(Hit, build_location, end, ECC_GameTraceChannel2, TraceParams)) {

        preview_structure[structure_type]->SetMaterial(0, placeableMat);
        isSpawnable = true; // Can build
        buildLocationZ = Hit.Location.Z;        

        if (GetWorld()->OverlapMultiByChannel(overlapResult, build_location, deltaRot, ECC_GameTraceChannel3,
          FCollisionShape::MakeSphere(180.0f), CollisionParams, sphereCollision)) {
          GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Foundation");

          tmpLocation = build_location;          

          // Checks all structures storaged in overlapResult
          for (int i = 0; i < overlapResult.Num(); ++i) {
            spawnedActorLocation = overlapResult[i].GetActor()->GetActorLocation();
            tmpLocation -= spawnedActorLocation;
            tmpX = abs(tmpLocation.X);
            tmpY = abs(tmpLocation.Y);

            // Checks four sides of a foundation to define the position of the next one
            if (tmpX > tmpY) {
              if (tmpLocation.X >= 0) {
                build_location.X = spawnedActorLocation.X + 300.0f;
                build_location.Y = spawnedActorLocation.Y;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z;
              }
              else {
                build_location.X = spawnedActorLocation.X - 300.0f;
                build_location.Y = spawnedActorLocation.Y;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z;
              }
            }
            else if (tmpX < tmpY) {
              if (tmpLocation.Y >= 0) {
                build_location.Y = spawnedActorLocation.Y + 300.0f;
                build_location.X = spawnedActorLocation.X;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z;
              }
              else {
                build_location.Y = spawnedActorLocation.Y - 300.0f;
                build_location.X = spawnedActorLocation.X;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z;
              }
            }
            //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, tmpLocation.ToString());
          }

        }

        preview_structure[structure_type]->SetActorLocation(build_location);

      }else{

        preview_structure[structure_type]->SetActorLocation(no_build_location);
        isSpawnable = false;
        preview_structure[structure_type]->SetMaterial(0, notPlaceableMat);

      }

      break;

    case 1: // Walls
      
      if (GetWorld()->OverlapMultiByChannel(overlapResult, build_location, deltaRot, ECC_GameTraceChannel3,
          FCollisionShape::MakeSphere(75.0f), CollisionParams, sphereCollision)) {
        //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Walls");
                      
        tmpLocation = build_location;
        
        // Check all structures storaged in overlapResult
        for (int i = 0; i < overlapResult.Num(); ++i) {

          // Gets the Actor detected by the Sphere Collision
          spawnedActorLocation = overlapResult[i].GetActor()->GetActorLocation();
          // Actor casted to AStructure type
          myStructure = (AStructure*)overlapResult[i].GetActor();
          ID = myStructure->GetType();
          tmpString = FString::SanitizeFloat(ID);          
          tmpLocation -= spawnedActorLocation;
          tmpX = abs(tmpLocation.X);
          tmpY = abs(tmpLocation.Y);

          // Checks whether or not myStructure is a foundation
          if (ID == 0) {
            isSpawnable = true;
            preview_structure[structure_type]->SetMaterial(0, placeableMat);
            if (tmpX > tmpY) {
              if (tmpLocation.X >= 0) {
                build_location.X = spawnedActorLocation.X + 150.0f;
                build_location.Y = spawnedActorLocation.Y;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z + 150.0f;
                build_rotation.Yaw = 180.0f;
              }
              else {
                build_location.X = spawnedActorLocation.X - 150.0f;
                build_location.Y = spawnedActorLocation.Y;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z + 150.0f;
                build_rotation.Yaw = 0.0f;
              }
            }
            else if (tmpX < tmpY) {
              if (tmpLocation.Y >= 0) {
                build_location.Y = spawnedActorLocation.Y + 150.0f;
                build_location.X = spawnedActorLocation.X;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z + 150.0f;
                build_rotation.Yaw = -90.0f;
              }
              else {
                build_location.Y = spawnedActorLocation.Y - 150.0f;
                build_location.X = spawnedActorLocation.X;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z + 150.0f;
                build_rotation.Yaw = -270.0f;
              }
            }

          }
          else {
            isSpawnable = false;
            preview_structure[structure_type]->SetMaterial(0, notPlaceableMat);
          }
          //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, tmpLocation.ToString());
        }

      }else{

        preview_structure[structure_type]->SetMaterial(0, notPlaceableMat);
        ID = 99;
        preview_structure[structure_type]->SetActorLocation(no_build_location);
        isSpawnable = false;

      }

      preview_structure[structure_type]->SetActorLocation(build_location);
      preview_structure[structure_type]->SetActorRotation(build_rotation);

      break;

    case 2: // Doorframe
            
      if (GetWorld()->OverlapMultiByChannel(overlapResult, build_location, deltaRot, ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(180.0f), CollisionParams, sphereCollision)) {
        
        preview_structure[structure_type]->SetMaterial(0, placeableMat);
        isSpawnable = true;
        tmpLocation = build_location;

        // Check all structures storaged in overlapResult
        for (int i = 0; i < overlapResult.Num(); ++i) {

          spawnedActorLocation = overlapResult[i].GetActor()->GetActorLocation();
          myStructure = (AStructure*)overlapResult[i].GetActor();
          ID = myStructure->GetType();
          tmpString = FString::SanitizeFloat(ID);
          
          tmpLocation -= spawnedActorLocation;
          tmpX = abs(tmpLocation.X);
          tmpY = abs(tmpLocation.Y);

          if (ID == 0) {
            if (tmpX > tmpY) {
              if (tmpLocation.X >= 0) {
                build_location.X = spawnedActorLocation.X + 150.0f;
                build_location.Y = spawnedActorLocation.Y;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z + 150.0f;
                build_rotation.Yaw = 180.0f;
              }
              else {
                build_location.X = spawnedActorLocation.X - 150.0f;
                build_location.Y = spawnedActorLocation.Y;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z + 150.0f;
                build_rotation.Yaw = 0.0f;
              }
            }
            else if (tmpX < tmpY) {
              if (tmpLocation.Y >= 0) {
                build_location.Y = spawnedActorLocation.Y + 150.0f;
                build_location.X = spawnedActorLocation.X;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z + 150.0f;
                build_rotation.Yaw = -90.0f;
              }
              else {
                build_location.Y = spawnedActorLocation.Y - 150.0f;
                build_location.X = spawnedActorLocation.X;
                build_location.Z = buildLocationZ = spawnedActorLocation.Z + 150.0f;
                build_rotation.Yaw = -270.0f;
              }
            }

          }
          //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, tmpLocation.ToString());
        }

      }else{

        preview_structure[structure_type]->SetMaterial(0, notPlaceableMat);
        ID = 99;
        preview_structure[structure_type]->SetActorLocation(no_build_location);
        isSpawnable = false;

      }

      preview_structure[structure_type]->SetActorLocation(build_location);
      preview_structure[structure_type]->SetActorRotation(build_rotation);

      break;

    case 3: // Ceiling      

      if (GetWorld()->OverlapMultiByChannel(overlapResult, build_location, deltaRot, ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(50.0f), CollisionParams, sphereCollision)) {

        preview_structure[structure_type]->SetMaterial(0, placeableMat);
        isSpawnable = true;
        tmpLocation = build_location;

        // Check all structures storaged in overlapResult
        for (int i = 0; i < overlapResult.Num(); ++i) {

          spawnedActorLocation = overlapResult[i].GetActor()->GetActorLocation();
          spawnedActorRotation = overlapResult[i].GetActor()->GetActorRotation();
          forwardVector = overlapResult[i].GetActor()->GetActorForwardVector();
          myStructure = (AStructure*)overlapResult[i].GetActor();
          ID = myStructure->GetType();
          tmpString = FString::SanitizeFloat(ID);

          tmpLocation -= spawnedActorLocation;
          tmpX = abs(tmpLocation.X);
          tmpY = abs(tmpLocation.Y);

          if (ID == 1 || ID == 2) {
            
            build_location = forwardVector * 150.0f + spawnedActorLocation;
            build_location.Z = buildLocationZ = spawnedActorLocation.Z + 270.0f;

          }

          if (ID == 3) {

              spawnedActorLocation = overlapResult[i].GetActor()->GetActorLocation();
              tmpLocation -= spawnedActorLocation;
              tmpX = abs(tmpLocation.X);
              tmpY = abs(tmpLocation.Y);

              // Checks four sides of a foundation to define the position of the next one
              if (tmpX > tmpY) {
                if (tmpLocation.X >= 0) {
                  build_location.X = spawnedActorLocation.X + 300.0f;
                  build_location.Y = spawnedActorLocation.Y;
                }
                else {
                  build_location.X = spawnedActorLocation.X - 300.0f;
                  build_location.Y = spawnedActorLocation.Y;
                }
              }
              else if (tmpX < tmpY) {
                if (tmpLocation.Y >= 0) {
                  build_location.Y = spawnedActorLocation.Y + 300.0f;
                  build_location.X = spawnedActorLocation.X;
                }
                else {
                  build_location.Y = spawnedActorLocation.Y - 300.0f;
                  build_location.X = spawnedActorLocation.X;
                }
              }


            //build_location = forwardVector * 150.0f + spawnedActorLocation;
            build_location.Z = buildLocationZ = spawnedActorLocation.Z ;
          }
          GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, forwardVector.ToString());
        }

      }
      else {

        preview_structure[structure_type]->SetMaterial(0, notPlaceableMat);
        ID = 99;
        preview_structure[structure_type]->SetActorLocation(no_build_location);
        isSpawnable = false;

      }

      preview_structure[structure_type]->SetActorLocation(build_location);
      preview_structure[structure_type]->SetActorRotation(build_rotation);

      break;

    case 4: // Ramps

      if (GetWorld()->LineTraceSingleByChannel(Hit, build_location, end, ECC_GameTraceChannel2, TraceParams)) { // Hit with the ground
               
        buildLocationZ = Hit.Location.Z;

        if (GetWorld()->OverlapMultiByChannel(overlapResult, build_location, deltaRot, ECC_GameTraceChannel3,
          FCollisionShape::MakeSphere(120.0f), CollisionParams, sphereCollision)) {
          GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Ramps");

          tmpLocation = build_location;

          // Checks all structures storaged in overlapResult
          for (int i = 0; i < overlapResult.Num(); ++i) {
            spawnedActorLocation = overlapResult[i].GetActor()->GetActorLocation();
            tmpLocation -= spawnedActorLocation;
            tmpX = abs(tmpLocation.X);
            tmpY = abs(tmpLocation.Y);
            myStructure = (AStructure*)overlapResult[i].GetActor();
            ID = myStructure->GetType();

            // Checks four sides of a foundation to define the position of the next one
            if (ID == 0) {
              if (tmpX > tmpY) {
                if (tmpLocation.X >= 0) {
                  build_location.X = spawnedActorLocation.X + 150.0f;
                  build_location.Y = spawnedActorLocation.Y;
                  build_location.Z = buildLocationZ = spawnedActorLocation.Z;
                  build_rotation.Yaw = 180.0f;
                }
                else {
                  build_location.X = spawnedActorLocation.X - 150.0f;
                  build_location.Y = spawnedActorLocation.Y;
                  build_location.Z = buildLocationZ = spawnedActorLocation.Z;
                  build_rotation.Yaw = 0.0f;
                }
              }
              else if (tmpX < tmpY) {
                if (tmpLocation.Y >= 0) {
                  build_location.Y = spawnedActorLocation.Y + 150.0f;
                  build_location.X = spawnedActorLocation.X;
                  build_location.Z = buildLocationZ = spawnedActorLocation.Z;
                  build_rotation.Yaw = 270.0f;
                }
                else {
                  build_location.Y = spawnedActorLocation.Y - 150.0f;
                  build_location.X = spawnedActorLocation.X;
                  build_location.Z = buildLocationZ = spawnedActorLocation.Z;
                  build_rotation.Yaw = 90.0f;
                }
              }

              isSpawnable = true; // Can build
              preview_structure[structure_type]->SetMaterial(0, placeableMat);

            }
            else {
              isSpawnable = false; // Can build
              preview_structure[structure_type]->SetMaterial(0, notPlaceableMat);
            }
            //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, tmpLocation.ToString());
          }

        }
        else {
          isSpawnable = false; // Can build
          preview_structure[structure_type]->SetMaterial(0, notPlaceableMat);
        }

        preview_structure[structure_type]->SetActorLocation(build_location);
        preview_structure[structure_type]->SetActorRotation(build_rotation);
      } // End Hit with the ground
      else {

        preview_structure[structure_type]->SetActorLocation(no_build_location);
        isSpawnable = false;
        preview_structure[structure_type]->SetMaterial(0, notPlaceableMat);

      }

      break;

  }
  //Debug
  //GetWorld()->DebugDrawTraceTag = "BuildTrace";
  //GetWorld()->DebugDrawTraceTag = "CheckStructures";
}

void UBuildComponent::SpawnStructure() {

  if (isSpawnable && player_inv_ref->IsResource(invID2BuildID(structure_type),1)) {
    build_location.Z = buildLocationZ;
    AStructure* spawnedStructure = GetWorld()->SpawnActor<AStructure>(new_structure[structure_type], build_location, build_rotation);
    spawnedStructure->ChangeCollisionPreset();
    isSpawnable = false;
    player_inv_ref->SubResources(invID2BuildID(structure_type), 1);
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Spawneo");  

    //Update Inventory
    updateEvent.Broadcast(player_inv_ref->myResources);

  }

}

void UBuildComponent::ChangePreview()
{
  preview_structure[structure_type]->SetActorLocation(no_build_location);
  //structure_type = next_type;
  //structure_type += next_type;
  ++structure_type %= 5;
  if (structure_type < 0) {
    structure_type = max_build_comp - 1;
  }
  structure_type %= max_build_comp;
  preview_structure[structure_type]->SetActorLocation(build_location);
}


void UBuildComponent::ErasePreview() {
  preview_structure[structure_type]->SetActorLocation(no_build_location);
}

int UBuildComponent::invID2BuildID(int32 build_type) {
  int inv_type = 0;
  switch (build_type)
  {
    case 0: inv_type = 2; break; // Foundation
    case 1: inv_type = 3; break; // Wall
    case 2: inv_type = 6; break; // Doorfram
    case 3: inv_type = 4; break; // Celling
    case 4: inv_type = 5; break; // Ramp

  }
  return inv_type;
}


void UBuildComponent::SetPlayerInvRef(UPlayerInventory* ref) {
  player_inv_ref = ref;
}