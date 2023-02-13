// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ArktequeraCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Public/Player/BuildComponent.h"
#include "Public/Player/InteractComponent.h"
#include <Player/PlayerInventory.h>
#include "Engine/Engine.h"

//////////////////////////////////////////////////////////////////////////
// AArktequeraCharacter

AArktequeraCharacter::AArktequeraCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

  buildMode = CreateDefaultSubobject<UBuildComponent>("raycast");
  interact = CreateDefaultSubobject<UInteractComponent>("interact");
  myInventory = CreateAbstractDefaultSubobject<UPlayerInventory>("Inventory");
  buildMode->SetPlayerInvRef(myInventory);
  playerMode = 1;
  playerMode_total = 2;

}

//////////////////////////////////////////////////////////////////////////
// Input

void AArktequeraCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
  PlayerInputComponent->BindAction("Build", IE_Pressed, this, &AArktequeraCharacter::LeftMouseAction);
  PlayerInputComponent->BindAction("ChangeStructure", IE_Pressed, this, &AArktequeraCharacter::ChangeStructureType);

	PlayerInputComponent->BindAxis("MoveForward", this, &AArktequeraCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArktequeraCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AArktequeraCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AArktequeraCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AArktequeraCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AArktequeraCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AArktequeraCharacter::OnResetVR);


  InputComponent->BindAction("PlayerModeAdd", EInputEvent::IE_Pressed, this, &AArktequeraCharacter::AddPlayerMode);
  InputComponent->BindAction("PlayerModeSub", EInputEvent::IE_Pressed, this, &AArktequeraCharacter::SubPlayerMode);
}


void AArktequeraCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AArktequeraCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AArktequeraCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AArktequeraCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AArktequeraCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AArktequeraCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AArktequeraCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AArktequeraCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  switch (playerMode) {
    case 0: // Build Mode
      buildMode->WhereToBuild(FollowCamera->GetForwardVector());
      //buildMode->SpawnStructure();
      break;
    case 1: //Normal
      //interact->EVInteract(GetActorForwardVector());
      break;
  }
}

void AArktequeraCharacter::Action() {
  GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green,
    "ACTION");
}

void AArktequeraCharacter::LeftMouseAction() {

  switch (playerMode) {

    case 0: // Build Mode
      buildMode->SpawnStructure();
      break;

    case 1:
      interact->EVInteract(GetActorForwardVector());
      break;
  }

}

void AArktequeraCharacter::ChangeStructureType() {

  FString tmpString;

  if (playerMode == 0) {
    //++buildMode->structure_type %= 5;
    buildMode->ChangePreview();
    tmpString = FString::SanitizeFloat(buildMode->structure_type);
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange,
      tmpString);
  }

}

void AArktequeraCharacter::AddPlayerMode() {
  if (playerMode == 0) {
    buildMode->ErasePreview();
  }
  playerMode++;
  playerMode = playerMode % playerMode_total;
}

void AArktequeraCharacter::SubPlayerMode() {
  if (playerMode == 0) {
    buildMode->ErasePreview();
  }
  playerMode--;
  if (playerMode < 0) {
    playerMode = playerMode_total-1;
  }
}