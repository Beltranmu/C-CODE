#include "Map/ChangeLevelTrigger.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstanceBEU.h"
#include "Player/TurboPlayer.h"
#include "TimerManager.h"

AChangeLevelTrigger::AChangeLevelTrigger()
{

	PrimaryActorTick.bCanEverTick = true;

  mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
  RootComponent = mesh;

  mesh->bHiddenInGame = true;

	dialogToStart = 0;
	dialogToEnd = 0;
	imageToShow = 0;

	alreadyUsed = false;

}

void AChangeLevelTrigger::BeginPlay()
{

	Super::BeginPlay();

	hud_ = Cast<AHUDGameLevel>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	myGI_ = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

  mesh->OnComponentBeginOverlap.AddDynamic(this, &AChangeLevelTrigger::TriggerChangeLevel);
	
}

void AChangeLevelTrigger::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (myGI_->nDialog == dialogToEnd && !GetWorld()->GetTimerManager().IsTimerActive(timerCL))
	{

		continueGame();

	}

}

void AChangeLevelTrigger::changeLevel()
{

  UGameplayStatics::OpenLevel(this, FName(mapName), false);

}

void AChangeLevelTrigger::TriggerChangeLevel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ATurboPlayer* tmpPlayer = nullptr;
	tmpPlayer = Cast<ATurboPlayer>(OtherActor);

	if (tmpPlayer && !alreadyUsed)
	{

		APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FInputModeUIOnly auxInputHelper = FInputModeUIOnly();
		player0->SetInputMode(auxInputHelper);

		myGI_->nDialog = dialogToStart;
		myGI_->nImage = imageToShow;

		hud_->DialogBehaviour();

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), slowValue);

		alreadyUsed = true;

	}
  
}

void AChangeLevelTrigger::continueGame()
{
	
	hud_->DialogEnd();
	hud_->RemoveFromRoot();

	APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly auxInputHelper = FInputModeGameOnly();
	player0->SetInputMode(auxInputHelper);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	GetWorld()->GetTimerManager().SetTimer(timerCL, this, &AChangeLevelTrigger::changeLevel, timeCL, false);

}

void AChangeLevelTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);

	if (mesh->OnComponentBeginOverlap.IsBound())
	{

		mesh->OnComponentBeginOverlap.RemoveAll(this);

	}

}
