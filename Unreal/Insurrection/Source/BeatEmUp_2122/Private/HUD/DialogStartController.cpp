
#include "HUD/DialogStartController.h"
#include "Core/GameInstanceBEU.h"
#include "Kismet/GameplayStatics.h"
#include "Core/CameraBEU.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ADialogStartController::ADialogStartController()
{

	PrimaryActorTick.bCanEverTick = true;

	dialogToStart = 0;
	dialogToEnd = 0;
	imageToShow = 0;
	isUsed = false;

}

void ADialogStartController::BeginPlay()
{
	Super::BeginPlay();

	hud_ = Cast<AHUDGameLevel>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	myGI_ = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	GetWorld()->GetTimerManager().SetTimer(startLevelTimer, this, &ADialogStartController::StartChangeToDialog, timeToShow * slowValue, false);

}

void ADialogStartController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (!isUsed)
	{
		
		isUsed = true;
		StartLevelImage();
	
	}

	EndStartLevelDialog();
	//
	//EndStartLevelDialog();

}

	//hud_->DialogBehaviour();

void ADialogStartController::StartLevelImage()
{

	myGI_->camera->leftWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	myGI_->camera->rightWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeUIOnly auxInputHelper = FInputModeUIOnly();
	player0->SetInputMode(auxInputHelper);

	myGI_->nDialog = dialogToStart;
	myGI_->nImage = imageToShow;

	hud_->StartLevelWidget();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), slowValue);

}

void ADialogStartController::StartChangeToDialog()
{

	hud_->DialogBehaviour();
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), slowValue);

}

void ADialogStartController::EndStartLevelDialog()
{

	if (myGI_->nDialog == dialogToEnd)
	{
		
		myGI_->camera->leftWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		myGI_->camera->rightWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		hud_->DialogEnd();
		hud_->RemoveFromRoot();

		APlayerController* player0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FInputModeGameOnly auxInputHelper = FInputModeGameOnly();
		player0->SetInputMode(auxInputHelper);

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		Destroy();
	
	}

}

