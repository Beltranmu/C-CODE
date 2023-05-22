// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IDamagable.h"
#include "CommonBEU.h"
#include "BaseCharacter.generated.h"

class UCustomAnim;
class UBoxComponent;
class UCharacterMovementComponent;


UCLASS()
class BEATEMUP_2122_API ABaseCharacter : public ACharacter, public IIDamagable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

  UPROPERTY(EditAnywhere, Category = "ID")
    uint32 characterID;


  /** Change color when hit, stunned,...*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    UMaterialInstanceDynamic* materialCharacter;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FLinearColor original_color;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FLinearColor hit_color;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FLinearColor stunned_color;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FLinearColor knockdown_color;

  /** HIT */
  UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Material")
    float timeResetMaterial = 0.33f;


  FTimerHandle timerHitMaterial;

  /** STATS */
  UPROPERTY(EditAnywhere, Category = "Stats")
    int score;

  UPROPERTY(EditAnywhere, Category = "Stats")
    float health;

	UPROPERTY(EditAnywhere, Category = "Stats")
    float maxHealth;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    bool alive;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    bool unostopable;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FVector defaultKnockbackForce = FVector(0.0f, 100.0f, 400.0f);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FAttackStats attackStats;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FCharacterStatus state;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
  FTimerHandle timerResetHit;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FTimerHandle timerResetKnockDown;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FTimerHandle timerResetStun;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FTimerHandle timerDeath;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float blockArmorMultiplier;

  UPROPERTY(EditAnywhere, Category = "Time")
    float deltaY;

  /** MOVEMENT */
  UPROPERTY(VisibleAnywhere, Category = "Movement")
    bool grounded;

  UPROPERTY(VisibleAnywhere, Category = "Movement")
    bool canMove;

  UPROPERTY(VisibleAnywhere, Category = "Movement")
      bool blockMovementRight;

  UPROPERTY(VisibleAnywhere, Category = "Movement")
      bool blockMovementLeft;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float moveXSpd;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float moveYSpd;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float rotationAngle;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float tmprotationAngle;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    FRotator fwDirection;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    FRotator rotationVector;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    FVector velocityVector;

  /** JUMP */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
    bool canJumpChar;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
    bool notLoadingAttack;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
    float jumpUnitsX;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
    float jumpUnitsY;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
    float jumpUnitsZ;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
    float cannotJump;

  /* ANIMATION */
  
  UPROPERTY()
    UCustomAnim* _animInstance;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _animationMontage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
      UAnimMontage* _airKickAnimationMontage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	  UAnimMontage* _highAttackAnimationMontage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	  UAnimMontage* _lowAttackAnimationMontage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* _powerAttackAnimtionMontage;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
	  bool _punching;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
	  bool _kicking;

  // Whether the character will be ready to attack or not
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    bool _attackEnd;

  // Whether the character will be ready to attack or not
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    bool _airAttackOnce;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    bool _airAttackDone;


  // Time the character has to perform a combo
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    bool _bComboTime;

  // Defines if the character's in the middle of a combo
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    bool _onCombo;

  // Whether the character is being attacked or not
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    bool _getAttacked;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    FName _nextYInput;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    FName _nextXInput;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
      TArray<TEnumAsByte<InputType>> inputArray;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
      int maxInputsStored = 1;

  /** HITBOX */
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HitBox|ArrayHit")
    TArray<AActor*> _charactersHit;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HitBox|BoneArrayHit")
    TArray<FName> _bonesHit;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
    bool hitUpperBodyActive;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
    bool hitLowerBodyActive;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = " ActorHit Box Preset")
    FName nameCollisionPreset;
  


  //Constructor fix
  USceneComponent* tmpScene;

  /** KNOCKBACK */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    FVector knockBackUnits;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
    bool knockbacked;

  /** SLOW MO */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlowMo")
    float scaleFactor = 0.5f;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlowMo")
    float timeResetSlomo = 0.25f;
  FTimerHandle timerSlowMo;

  /** COMBAT */

  FTimerHandle dashTimer;
  FTimerHandle dashRecharge;
  FTimerHandle dashContinue;
  FTimerDelegate dashContinueTimerDelegate;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
    int32 currentDashCharges;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
    int32 maxDashCharges = 3;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
    float dashAddChargeTime = 10.0f;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
      bool canDash;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
      float dashRechargeTime = 1.0f;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
      bool dashing;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
      float dashDuration = 0.4f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
      float dashSpeed = 2500.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Dash")
      float dashStaminaCost;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
      bool canBeDamaged;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat | Charged attack")
    float bonus_charging_attack;


  //UPROPERTY(VisibleAnywhere, Category = "Combat | Preset")
    ECollisionChannel collisionAttackPreset;

    UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Stats", DisplayName = "Stamina cost air Kick")
      float staminaCostAirKick = 15.0f;

    UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Stats", DisplayName = "Stamina recovered per Hit")
      float recoveryStaminaPerHit = 2.0f;



  /** BLOCK */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    bool blocks360 = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    float maxStamina;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    float stamina;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Block")
    bool blockIsActive;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Block")
    bool parryIsActive;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    float parryTime;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    float staminaLossPerSecond;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    float staminaRegenPerSecond;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    float minimumStamina;

  bool blockOnCD;

  FTimerHandle parryTimer;
  FTimerHandle blockCooldownTimer;


  /** THROWABLE(sorry not sorry por meterlo aqui)*/
  UPROPERTY(EditAnywhere, Category = "Stats")
      bool holdingObject = false;

  UPROPERTY(EditAnywhere, Category = "Stats")
    bool holdingObjectAnimation = false;

  UPROPERTY(EditAnywhere, Category = "Stats")
    bool throwingBarrel = false;



  UPROPERTY(VisibleAnywhere, Category = "Stats")
      float defaultWalkSpeed;

  FTimerHandle slowTimer;





  //Reset the physical impact
  FName bname;
  FTimerDelegate resetPhysicsTimerDelegate;
  FTimerHandle resetPhysicsTimer;


  //Death Particle
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    FParticleData deathParticle;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    FParticleData shieldParticle;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    FParticleData airKickParticle;


  //Die
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "state|Death")
    bool death;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "state|Death")
    bool isDiying; 
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "state|Death")
    float diyingTime;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "state|Spawn")
    float timeToDie = 3.0f;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "state|Spawn")
    bool isSpawning;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "state|Spawn")
    float spawningTime;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "state|Spawn")
    float timeToSpawn = 1.0f;



  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "state")
    int hitype;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "test")
    float fade = -1.0f;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "test")
    float dfade = -0.75f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
  virtual void EndPlay(EEndPlayReason::Type EndReason)override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** INTERFACE */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damagable")
    void TakeAHit(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter);
    virtual void TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter);
  /** MOVEMENT */
  UFUNCTION()
    void moveAxisX(float value);

  UFUNCTION()
    void moveAxisY(float value);

  // Gets Actor Velocity in every frame and determines its state
  //void ToggleCharacterState();
  UFUNCTION()
  void onAir();

  /** JUMP */
  UFUNCTION()
    void myJump();

  UFUNCTION()
    void myStopJump();

  UFUNCTION()
    void jumpLogic();

  UFUNCTION()
    virtual void Landed(const FHitResult& Hit) override;

	/** UI UPDATE*/
  

  /** COMBAT */
/* Punch and Kick will both call Attack(int move)
*  but it will determine which input has been received */
  UFUNCTION()
    void Punch() ;

  UFUNCTION()
    void Kick();

  UFUNCTION()
    void Attack(InputType move);	// 0 Punch, 1 Kick (...)

  UFUNCTION()
    virtual void Dash(FVector dir);

  UFUNCTION()
    void continueDash(FVector dir);

  UFUNCTION()
    virtual void StopDash();

  UFUNCTION()
    void RechargeDash();

  UFUNCTION()
    void ResetSlowMo();



  /** THROWABLES */

  UFUNCTION()
      void ApplySlow(float slowPercentage, float duration);

  UFUNCTION()
      void ClearSlow();


  /** BLOCKING */

  UFUNCTION()
    void startBlock();

  UFUNCTION()
    void stopBlock();

  UFUNCTION()
    void stopParry();

  UFUNCTION()
    void stopBlockCooldown();

  //**STATS*/
  UFUNCTION()///Have to be modified if you want to add a new stats for the attacks
  void resetAttackStats();

  //**STATS*/
  UFUNCTION()///Have to be modified if you want to add a new stats for the attacks
    void resetStateStats();


  UFUNCTION()
    void standUp();

  UFUNCTION()
    virtual void Death();

  UFUNCTION()
    virtual void HitDamagable(AActor* hitedActor);

  UFUNCTION()
    virtual void HitUIUpdate(AActor* hitedActor);

  UFUNCTION()
  void resetPhysicBody();
  void startTimerResetPhysicBody(FName bname);

  UFUNCTION()
    virtual void OnDeathImpulse();

  UFUNCTION()
  virtual void SpawnParticleSystem(
    FParticleData& particleData, bool attachedToHit = false, FVector hitLocation = FVector::ZeroVector);


  UFUNCTION()
    virtual void ResetMaterial();
  
};
