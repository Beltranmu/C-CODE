#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HUD/HUDGameLevel.h"
#include "TriggerDialog.generated.h"

class ATurboPlayer;

UCLASS()
class BEATEMUP_2122_API ATriggerDialog : public AActor
{
	GENERATED_BODY()
public:

	ATriggerDialog();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* myBody;

	AHUDGameLevel* hud_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int dialogToStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int dialogToEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int imageToShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		float slowValue;

	UPROPERTY()
		class UGameInstanceBEU* myGI_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		bool alreadyUsed;

	void continueGame();

	UFUNCTION()
		void PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
