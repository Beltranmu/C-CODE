#pragma once

#include "CoreMinimal.h"
#include "Core/BaseCharacter.h"
#include "Props/BossRazor.h"
#include "FinalBossCharacter.generated.h"

class UBehaviorTree;
class APointArrayActor;
class ARazorSpawnPoint;
class AEnemyCommander;
class AMasillaEnemy;
class ASpawnTrigger;
class UGameInstanceBEU;

UENUM()
enum BossPhase {
	kPhase1 = 0,
	kFlying1,
	kPhase2,
	kFlying2,
	kPhase3
};

USTRUCT()
struct FMissileAttack {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> missileBP;

	UPROPERTY(EditAnywhere)
	float delayToStart;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* missileAttackAnim;
};

USTRUCT()
struct FBasicAttack {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float dashDelay;
	UPROPERTY(EditAnywhere)
	float dashSpeed;
	UPROPERTY(EditAnywhere)
	float dashDuration;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* anim;


};

USTRUCT()
struct FChargeAttack {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		float dashDelay;
	UPROPERTY(EditAnywhere)
		float dashSpeed;
	UPROPERTY(EditAnywhere)
		float dashDuration;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* anim;
	UPROPERTY(EditAnywhere)
		float vulnerabilityTime;
	//-1 left, 1 right
	UPROPERTY(VisibleAnywhere)
	int dir;

};

USTRUCT()
struct FLaserAttack{

	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float width;

	UPROPERTY(EditAnywhere)
		int lasersToFire;

	UPROPERTY(EditAnywhere)
		float chargeTime;

	UPROPERTY(EditAnywhere)
		float lockOnTime;

	UPROPERTY(EditAnywhere)
		float fireTime;

	UPROPERTY(EditAnywhere)
		FAttackStats stats;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* anim;

	UPROPERTY(VisibleAnywhere)
	int dir;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateBossHPBar, float, currentHP, float, maxHP);

UCLASS()
class BEATEMUP_2122_API AFinalBossCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFinalBossCharacter();

	UGameInstanceBEU* myGI;

	FTimerHandle rotateTowardsCameraTimer;

	//We have 2 materials
	UMaterialInstanceDynamic* materialCharacter2ElectricBoogaloo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update HP")
		FUpdateBossHPBar UpdateBossHealthBar;

	//AI
	UPROPERTY(EditAnywhere, Category = "Boss Config | AI")
		UBehaviorTree* bTree;
	
	UPROPERTY(EditAnywhere, Category = "Boss Config | AI")
		int targetPlayerID;

	class UBlackboardComponent* blackboardComp;

	//Enemies
	UPROPERTY(EditAnywhere, Category = "Boss Config | Enemies")
		TSubclassOf<AEnemyCommander> enemyCommanderClass;

	AEnemyCommander* enemyCommander;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Enemies")
		TSubclassOf<AMasillaEnemy> masillaClass;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Enemies")
		TArray<ASpawnTrigger*> spawnTriggers;

	bool enemiesToSpawn;

	FTimerHandle enemySpawnDelayTimer;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Camera")
		FCameraLockConfig groundBossCameraConfig;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Camera")
		FCameraLockConfig flightBossCameraConfig;


	//Attacks
	UPROPERTY(EditAnywhere, Category = "Boss Config | Locations")
		APointArrayActor* missileAttackLocations;

	//Location where dash attack will start
	UPROPERTY(EditAnywhere, Category = "Boss Config | Locations")
		APointArrayActor* chargeAttackLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Config | Locations")
		APointArrayActor* razorSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Config | Locations")
		FVector razorAttackBossLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Config | Locations")
		APointArrayActor* laserAttackBossLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Config | Locations")
		APointArrayActor* flightLocation;
	
	UPROPERTY(EditAnywhere, Category = "Boss Config | Boss Attacks")
		TSubclassOf<AActor> razorBP;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Boss Attacks")
		FMissileAttack missileAttack;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Boss Attacks")
		FBasicAttack basicAttack;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Boss Attacks")
		FChargeAttack chargeAttack;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Boss Attacks")
		FRazorMovement razorConfig[6];

	UPROPERTY(EditAnywhere, Category = "Boss Config | Boss Attacks")
		class UAnimMontage* razorAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Boss Attacks")
		FLaserAttack laserAttack;


	UPROPERTY(EditAnywhere, Category = "Boss Config | Phase")
		TEnumAsByte<BossPhase> currentPhase;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Movement")
		bool flying;
	
	UPROPERTY(EditAnywhere, Category = "Boss Config | Movement")
		float flyingHeight = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Movement")
		float groundHeight = -90.0f;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Movement")
		float kFlyingUpLerp = 0.1f;

	float timer;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Movement")
		float flightDelay = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Phase")
		float phase2HPPercentage;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Phase")
		float phase3HPPercentage;

	UPROPERTY(EditAnywhere, Category = "Boss Config | Visual")
		float holoStrength = 2.0f;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();

	virtual void Dash(FVector dir) override;

	virtual void StopDash() override;

	//We update dash stats with those on the structs, easier than having two dash functions for boss
	//or changing dash in every class that inherits from base character.
	void UpdateDashStats(float duration, float speed);

	void ChargeAttack();

	void RazorAttack();

	//Should be used to switch between boss phases. Changes stats of attacks and timers to increase difficulty.
	void ChangePhase(TEnumAsByte<BossPhase> phase);

	void CheckPhaseChange();

	virtual void TakeAHit_Implementation(FVector knockback_force, FAttackStats& attack_info, bool hitFromBehind, AActor* hiter);

	void ToggleFlight();

	//Sets boolean to false, it is a function to call this with a timer
	void EnemyDelayBool();

	void ResetBossMaterial();

};
