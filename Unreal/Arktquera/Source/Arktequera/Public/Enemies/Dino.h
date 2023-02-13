// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IDamagable.h"

#include "Dino.generated.h"


class UHealthComponent;
class USkeletalMeshComponent;
class UBoxComponent;
class UDinoMovementComponent;
class USphereComponent;


UCLASS()
class ARKTEQUERA_API ADino : public APawn, public  IIDamagable
{
  GENERATED_BODY()

public:
  // Sets default values for this pawn's properties
  ADino();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box Collider")
    UBoxComponent* box_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spherecast Detection")
    USphereComponent* sphereCollider_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
    USkeletalMeshComponent* dinoMesh_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
    UHealthComponent* myHealth_;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float initial_hp;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    UDinoMovementComponent* dinoMovement_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    AActor* target_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    float distance_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    float currentTime_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    float timeObjective_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    bool startMovement_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    FVector initialLocation_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    FVector currentLocation_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    FVector newDirection_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    FVector new_normal;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    FRotator currentRot_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    FRotator nextRot_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    int32 movementMode_;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    USceneComponent* dinoRaycastFront;
    
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    USceneComponent* dinoRaycastCenter; 
    
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    USceneComponent* dinoRaycastBack;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    FTimerHandle Timer_mov;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    float time_new_mov;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DinoPawn Movement")
    float new_time_to_move;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

private:

  UPROPERTY(VisibleAnywhere, Category = "Dino Stats")
    float speed_;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  UFUNCTION()
    void Walk(float DeltaTime);

  UFUNCTION()
    void DetectHuman(AActor* OverlappedActor, AActor* OtherActor);

  UFUNCTION()
    void StopChasing(AActor* OverlappedActor, AActor* OtherActor);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactuable")
    void NativeGetAttacked(float dmg);
  virtual void NativeGetAttacked_Implementation(float dmg);


  UFUNCTION()
    void TimerMov();
};