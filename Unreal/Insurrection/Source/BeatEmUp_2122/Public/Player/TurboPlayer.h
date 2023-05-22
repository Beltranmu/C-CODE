
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CameraBEU.h"
#include "HUD/HUDGameLevel.h"
#include "Core/BaseCharacter.h"
#include "Navigation/CrowdAgentInterface.h"
#include "Components/CapsuleComponent.h"
#include "TurboPlayer.generated.h"

/**
 * 
 */
class AThrowableActor;
class UForceFeedbackEffect;
class USpringArmComponent;
class UDestinationPoint;
class UWidgetComponent;
class UDecalComponent;
class UPointLightComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnTriggerEvent, ASpawnTrigger*, trigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCatchEnemyOnHit, int, idEnemy,float, hpPercentage,int, idCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUpdatePlayersUI, int32, playerID, float, currentValue, float, maxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdatePlayersUITwoP, int32, playerID, int32,Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCameraShake, float, shakeForce);

UCLASS()
class BEATEMUP_2122_API ATurboPlayer : public ABaseCharacter, public ICrowdAgentInterface
{
	GENERATED_BODY()

	public:
		ATurboPlayer();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
      FParticleData healthParticle;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles", DisplayName = "Damage Up Particle")
      FParticleData dmgUpParticleR;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles", DisplayName = "Damage Up Particle")
      FParticleData dmgUpParticleL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles", DisplayName = "Invulnerable Particle")
      FParticleData invParticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TexturePlayer2")
        UTexture* BaseColorTexture2;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TexturePlayer2")
        UTexture* EmisiveTexture2;


    UPROPERTY(VisibleAnywhere, Category = "Particles")
      FVector shieldOffset;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cheats")
      bool unlimitedUltimate = false;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Cheats")
			bool godMode_ = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cheats")
      bool noDMGMode_ = false;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Player HP")
			FUpdatePlayersUI updateUIHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Player AOE ")
      FUpdatePlayersUI updateUIAOE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Player Stamina")
      FUpdatePlayersUI updateUIStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Player Lifes ")
      FUpdatePlayersUITwoP updateUILifes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Player Score ")
      FUpdatePlayersUITwoP updateUIScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Player Dashes ")
      FUpdatePlayersUITwoP updateUIDash;

	UPROPERTY(VisibleAnywhere, Category = "UI")
		UWidgetComponent* interactIcon;

		UPROPERTY()
			class UGameInstanceBEU* myGI;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
			AHUDGameLevel* hud_;

		 UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Hit Enemy Location")
			 FVector hitEnemyLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Event")
      FSpawnTriggerEvent triggerEvent;

		UPROPERTY(EditAnywhere, BLueprintReadWrite, Category="Vibration")
			TArray<UForceFeedbackEffect*> vibrations; 

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shake Event")
      FCameraShake cameraShakeEvent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shake Event")
      float shakeForce;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hit Event")
			FCatchEnemyOnHit catchEnemyDataOnHit;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
			float maxAOECharge = 100.0f;	


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
			UAnimMontage* _ultimateAnimationMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
      float staminaPerHit = 7.5f;


		/** POWER ATTACK */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK | Decal")
      USceneComponent* _decalRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK | Decal")
      UDecalComponent* _decal;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK | Point Light")
      UPointLightComponent* _powerfulAttackPointLight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK | Point Light")
      float powerfulAttackLightIntensityScale = 1000.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK | Point Light")
      float powerfulAttackLightIntensityReleaseScale = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK | Decal")
      float powerfulPunchScale;

    UPROPERTY(VisibleAnywhere, Category = "Simple Punch")
      bool simplePunch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|Value")
      float powerAttackCurrentHoldingTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|timer")
      FTimerHandle timerHoldingToStartLoad;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|time")
      float timeHoldingToStartLoad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|Value", meta = (ClampMin = "1.0", ClampMax = "4.0", UIMin = "1.0", UIMax = "4.0"))
      float maxMultiplierFactorPowerAttack = 2.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|Value")
      float minMultiplierFactorPowerAttack = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|Value", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
     float slowfactorPowerAttack = 0.5f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|key Name")
      FName keyName;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|key Name")
      FName buttonName;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|key Name")
      FName ultimateButtonName;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|key Name")
      bool startUltimate = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|timer")
      FTimerHandle holdAttackTimer;													
												

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|speed release", meta = (ClampMin = "1.0", ClampMax = "5.0", UIMin = "1.0", UIMax = "5.0"))
      float  releaseSpeed = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|time")
      float time_load_attack;																
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|time")
      float time_charging_attack;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "POWER ATTACK|Info")
      bool isLoading;


    //Ultimate
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ultimate | Point Light")
      UPointLightComponent* _ultimateHeadPointLight;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ultimate | Point Light")
      UPointLightComponent* _ultimateRootPointLight;

    FTimerHandle ultimateRestDMGTimer;


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnVar")
			float distance_to_spawn;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
			FLinearColor block_color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color|Timer")
      FTimerHandle changColorTimer;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
      float timeToRespawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn|Timer")
      FTimerHandle respawnTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn|Explosion", meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0.0", UIMax = "90.0"))
      float explosionDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn|Explosion", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
      float explosionForce;

		// ------- DESTINATIONS ------- //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
    TArray<UDestinationPoint*> targetPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
      TArray<USpringArmComponent*> springArmArray;

    UPROPERTY(VisibleAnywhere, Category = "Spawn Points")
      FVector perimeterPointRelativeDistance[4];

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
      FTimerHandle timerSendEnemiesTowardsPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
      float timerSendEnemiesTowardsPlayerValueFirstTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
      float timerSendEnemiesTowardsPlayerValueRepeatedly;

		bool resetPoints[4];

	UInputComponent* inputComponent;
	//Pickups

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	bool dmgBoosted;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float percentageDmgBoost;

	FTimerHandle dmgBoostTimer;

	FTimerHandle invencibleTimer;
  FTimerHandle dashReFillTimer;
  FTimerHandle holdingObjectAnimationTimer;


		virtual void BeginPlay() override;
		virtual void Tick(float deltaTime) override;


		virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

		void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

		UFUNCTION()
			void PlayerOverlap(AActor* self, AActor* other);

		UFUNCTION()
			void Interact();

		UFUNCTION()
			void DialogControl();

    /** INTERFACE */
    virtual void TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter) override;
	

		// Attributed

		UPROPERTY(EditAnywhere, Category = "Throwable")
		AActor* attachedActor;


    UFUNCTION()
      void playerKick();
		UFUNCTION()
		void playerPunch();

    UFUNCTION()
      void playerPowerfulPunch();

		UFUNCTION()
      void StartLoading();
		UFUNCTION()
      void LoadAttack();
		UFUNCTION()
      void EndLoad();


	  void AOEAttack();

    UFUNCTION()
      void StartUltimate();

    UFUNCTION()
      void StartUltimate2();

    UFUNCTION()
      void StartUltimateKeyboard();

		UFUNCTION()
		void callToUIEvent(int EnemyID, float hpPercentage, int enemyScore);

		UFUNCTION()
		void statsLimiter();

    UFUNCTION()
    void HideUI();

		UFUNCTION()
			void CharacterDash();

    UFUNCTION()
      void addChargeDash();

		UFUNCTION()
			void changeColor();

		UFUNCTION()
			void ClearInvencibleBoost();

		UFUNCTION()
			void ClearDmgBoost();

		void Death() override;
		
		UFUNCTION()
			void Respawn();

		UFUNCTION()
			void pauseGame();

		UFUNCTION()
		void SetInputGame(bool val);

		UFUNCTION()
		void SetInputDialog(bool val);

		UFUNCTION()
			void respawnExplosion();

		void HitDamagable(AActor* ohterActor) override;

		void HitUIUpdate(AActor* ohterActor) override;

		UFUNCTION()
			void UpdatePerimeterPoints();

		UFUNCTION()
			void UpdateWidget();



    /** @return current location of crowd agent */
    virtual FVector GetCrowdAgentLocation() const { return GetActorLocation(); }

    /** @return current velocity of crowd agent */
    virtual FVector GetCrowdAgentVelocity() const { return GetVelocity(); }

    /** fills information about agent's collision cylinder */
    virtual void GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const { CylinderRadius = GetCapsuleComponent()->GetScaledCapsuleRadius() + 25.f; CylinderHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();}

    /** @return max speed of crowd agent */
    virtual float GetCrowdAgentMaxSpeed() const { return 600.f; }

    /** Group mask for this agent */
    virtual int32 GetCrowdAgentAvoidanceGroup() const { return 1; }

    /** Will avoid other agents if they are in one of specified groups */
    virtual int32 GetCrowdAgentGroupsToAvoid() const { return MAX_int32; }

    /** Will NOT avoid other agents if they are in one of specified groups, higher priority than GroupsToAvoid */
    virtual int32 GetCrowdAgentGroupsToIgnore() const { return 0; }

    //key/button combination that can be used in case player gets stuck
    void InGameCheats();

};
