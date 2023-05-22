// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/BaseCharacter.h"
#include "Interface/EnemyDataUI.h"
#include "Props/Pickup.h"
#include "BaseEnemy.generated.h"

class ATurboPlayer;
class AAIController;
class UStatictMeshComponent;
class UStatictMesh;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointAttackEvent, ABaseEnemy*, myEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeathEvent, ABaseEnemy*, self, int32, playerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FUdateUIEvent, float, percentageHP, int32, enemyType, int32, enemyID,  int32, playerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNewPointEvent, ABaseEnemy*, enemy, int32, pointType, int32, playerIndexTarget);

UENUM()
enum TypeOfEnemy {

	TypeOfEnemy_Masilla = 0,
	TypeOfEnemy_MasillaModified,
	TypeOfEnemy_Agile,
	TypeOfEnemy_AgileModified,
	TypeOfEnemy_Tank,
	TypeOfEnemy_TankModified,

};

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API ABaseEnemy : public ABaseCharacter, public IEnemyDataUI
{
	GENERATED_BODY()
	
	public: 
		ABaseEnemy();

		typedef enum{

			kState_StandBy = 0,
			kState_GoToEdge,
			kState_GetRandomPoint,
			kState_GetRandomInteriorPoint,
			kState_GetLocation,
			kState_RefreshLocation,
			kState_GetAttackPoint,
			kState_PauseX,
			kState_PauseY,
			kState_MoveX,
			kState_MoveY,
			kState_Chase2,
			kState_InPosition,
			kState_InRange,
			kState_Wait2Attack,
			kState_Attack2,
			kState_SpecialBehaviour,
			kState_ChargingAttack

		} EnemyState;

	// Event
	FDeathEvent _onDeath;
	FNewPointEvent _getNewPoint;
	FPointAttackEvent _onAttackEvent;

	TypeOfEnemy enemyType;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ID")
      bool justEnteredDangerArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ID")
		int32 hitByPlayerID;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IDA")
    int32 enemyID = 0;


    UMaterialInstanceDynamic* meshMaterial1;
    UMaterialInstanceDynamic* meshMaterial2;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special DMG", DisplayName = "Special Type")
    bool specialVarient;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special DMG", DisplayName = "Extra damage for special type")
    float specialExtraDMG;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Special DMG", DisplayName = "Extra damage per Level")
    float extraDMGLevel;


    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Event UpdateUI")
      FUdateUIEvent updateUIEvent;
			 
	// Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		TArray<ATurboPlayer*> _mainCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
      int32 targetID;

	FVector characterLocation;
	FVector myLocation;
	FVector distanceVector;
	FVector tmpLocation;
				   
    EnemyState _state2;

    FTimerHandle _timer;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
			FTimerHandle _timerStartMovingAfterSpawning;		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
			float timerStartMovingAfterSpawningValue = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FTimerHandle _timerPauseX;
		FTimerDelegate _timerPauseXDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float PauseX;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FTimerHandle _timerPauseY;
    FTimerDelegate _timerPauseYDelegate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float PauseY;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FTimerHandle _timerPauseAttack;
    FTimerDelegate _timerPauseAttackDelegate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float PauseAttack = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FTimerHandle _timerPauseRethink;
    FTimerDelegate _timerPauseRethinkDelegate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float PauseRethink;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
      FTimerHandle _timerPauseRandomPoint;
			FTimerDelegate _timerPauseRandomPointDelegate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
      float PauseRandomPoint;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
			float rangeRandomPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
      FTimerHandle _timerCheckDangerAreaDistance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
      float _timerCheckDangerAreaDistanceValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    int32 triesBeforeChasing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    int32 currentTriesBeforeChasing;

		bool resetAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Making")
      float _combatCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Making")
      float _time;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Making")
			float _stop_distance;

		// Enemy
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
			FVector _myLocation;
  
		AAIController* aiController;

		bool isLastEnemyOfTheWave = false;
		bool isHitByFinisher = false;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
			FVector deathImpulse;

		// Players
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
			TArray<ATurboPlayer*> _playerArray;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Players")
      ATurboPlayer* _target;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Players")
			FVector _targetLocation;

    UPROPERTY(VisibleAnywhere, Category = "Timer Refresh Location")
      int32 pointIndex;

    UPROPERTY(VisibleAnywhere, Category = "Timer Refresh Location")
      int32 playerIndex;

    UPROPERTY(VisibleAnywhere, Category = "Edge Point Index")
      int32 edgePointIndex;

		UPROPERTY(VisibleAnywhere, Category = "Timer Refresh Location")
			FTimerHandle _refreshLocationTimer;

		UPROPERTY(EditAnywhere, Category = "Timer Refresh Location")
			float _refreshLocationValue;

		UPROPERTY(VisibleAnywhere, Category = "Target Point")
			bool pointHasBeenAssigned;

    UPROPERTY(VisibleAnywhere, Category = "Target Point")
      bool edgePointHasBeenAssigned;

    UPROPERTY(VisibleAnywhere, Category = "Target Point")
      bool pointOnAttackAssigned;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Players")
      FVector _targetDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
      float dangerAreaDistance;

		UPROPERTY(EditAnywhere, Category = "Perimeter Points | Weight")
			int middlePointWeight;
  
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp|Spawn")
			bool spawnPickUpActivated;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp|Spawn")
			TArray<TSubclassOf<APickup>> pickupList;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp|Ratio")
      TArray<int32> pickupRatio;
		UPROPERTY(VisibleAnywhere,  Category = "PickUp|NumberElements")
			uint32 numberOfElements;


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time To Attack")
			float timeWaitToAttack =0.2f;
		UPROPERTY(VisibleAnywhere, Category = "Time To Attack")
			FTimerHandle timerToWaitAttack;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time To Attack|Color")
			FLinearColor attackColor;





    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularMesh")
      UStaticMeshComponent* hairMesh;
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UStaticMesh*> hairMeshes;
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UMaterialInstanceDynamic*> hairMeshesMaterials;



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularMesh")
      UStaticMeshComponent* complementHeadMesh;
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UStaticMesh*> complementHeadMeshes;
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UMaterialInstanceDynamic*> complementHeadMeshesMaterials;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularMesh")
      UStaticMeshComponent* complementHipMesh;
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UStaticMesh*> complementHipMeshes;
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UMaterialInstanceDynamic*> complementHipMeshesMaterials;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularMesh")
      UStaticMeshComponent* complementFacialHair1Mesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularMesh")
      UStaticMeshComponent* complementFacialHair2Mesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModularMesh")
      UStaticMeshComponent* complementFacialHair3Mesh;
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UStaticMesh*> complementFacialHairMeshes;
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UMaterialInstanceDynamic*> complementFacialHairMeshesMaterials;


   UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
      TArray<UStaticMeshComponent*> complementsMeshes;
   UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ModularMesh")
     TArray<UMaterialInstanceDynamic*> meshesMaterial;



   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HelpMesh")
     UStaticMeshComponent* helpStaticMesh;
		virtual void BeginPlay()override;

		virtual void Tick(float DeltaTime) override;

		virtual void EndPlay(const EEndPlayReason::Type type) override;

		virtual void TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter) override;
		
		void RotateEnemy();

    UFUNCTION()
    virtual void specialBehavior();
    UFUNCTION()
      virtual void specialThink();

		float GetDistanceFromTarget(FVector targetLocation);

		// Death
		virtual void Death() override;

		UFUNCTION()
			int givePoints(int32 points);

		UFUNCTION()
			virtual void SpawnPickUp();
		// Artificial Intelligence
		void ChooseTarget(); // Will depend on different parameters
		void GetLocationFromTarget(); // Will determine the target's location


		UFUNCTION()
		virtual void StateMachine2(InputType attackMove);

    UFUNCTION()
      void PlayerDetection();

    UFUNCTION()
      void EnterInDangerZone();

		UFUNCTION()
			void BecomeAlive();

		UFUNCTION()
			void RefreshLocation();

		UFUNCTION()
		virtual void EndPause(int32 type);

		UFUNCTION()
		virtual void ResetEnemy();

    UFUNCTION()
      virtual void setNewStyle();

    UFUNCTION()
      virtual void resetStyle();

    UFUNCTION()
      void ResetTimeDilatation();

		UFUNCTION()
			FVector GetLocationTarget();

		UFUNCTION()
			void endWaitAttack();

		UFUNCTION()
			virtual void selectAttack() {};


    virtual void OnDeathImpulse() override;


    /** INTERFACE */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UIEnemyData")
      void CatchUIDataEnemy(int& id, float& percentage, int& points, int playerID);
    void CatchUIDataEnemy_Implementation(int& id, float& percentage, int& points, int playerID);


    void HitDamagable(AActor* hitedActor) override;

};
