#include "HUD/TriggerDialog.h"
#include "Player/TurboPlayer.h"
#include "Core/GameInstanceBEU.h"
#include "Kismet/GameplayStatics.h"

ATriggerDialog::ATriggerDialog()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled=false;
	myBody = CreateDefaultSubobject<UStaticMeshComponent>("MyTrigger");
	myBody->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);

	myBody->bHiddenInGame = true;

	dialogToStart = 0;
	dialogToEnd = 0;
	imageToShow = 0;

	alreadyUsed = false;

}

void ATriggerDialog::BeginPlay()
{

	Super::BeginPlay();

	myBody->OnComponentBeginOverlap.AddDynamic(this, &ATriggerDialog::PlayerOverlap);

	hud_ = Cast<AHUDGameLevel>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	myGI_ = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void ATriggerDialog::EndPlay(const EEndPlayReason::Type EndPlayReason) {

	Super::EndPlay(EndPlayReason);

	if (myBody->OnComponentBeginOverlap.Contains(this,TEXT("PlayerOverlap")))
	{

		myBody->OnComponentBeginOverlap.Remove(this, TEXT("PlayerOverlap"));

	}

}

void ATriggerDialog::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	continueGame();

}

void ATriggerDialog::PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	SetActorTickEnabled(true);
	ATurboPlayer* tmpPlayer = nullptr;
	tmpPlayer = Cast<ATurboPlayer>(OtherActor);
	
	APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeUIOnly auxInputHelper = FInputModeUIOnly();
	player0->SetInputMode(auxInputHelper);
	
	if (tmpPlayer && !alreadyUsed)
	{
	
		myGI_->nDialog = dialogToStart;
		myGI_->nImage = imageToShow;
	
		hud_->DialogBehaviour();
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), slowValue);
	
		alreadyUsed = true;
	
	}

}

void ATriggerDialog::continueGame()
{

	if (myGI_->nDialog == dialogToEnd)
	{

		hud_->DialogEnd();
		hud_->RemoveFromRoot();

		APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FInputModeGameOnly auxInputHelper = FInputModeGameOnly();
		player0->SetInputMode(auxInputHelper);

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		Destroy();

	}

}
