// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MenuGameModeBEU.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/NavigationConfig.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AMenuGameModeBEU::AMenuGameModeBEU() {

	PrimaryActorTick.bCanEverTick = true;

}

void AMenuGameModeBEU::BeginPlay() {

	Super::BeginPlay();

	//TMap <FKey, EUINavigation>& KeysForFocus = FSlateApplication::Get().GetNavigationConfig()->KeyEventRules;
	//
	//KeysForFocus.Emplace(EKeys::W, EUINavigation::Up);
	//KeysForFocus.Emplace(EKeys::S, EUINavigation::Down);
	//KeysForFocus.Emplace(EKeys::A, EUINavigation::Left);
	//KeysForFocus.Emplace(EKeys::D, EUINavigation::Right);
	//UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->PossessedBy(player1);
	//UGameplayStatics::GetPlayerCharacter(GetWorld(), 1)->PossessedBy(player2);

}

void AMenuGameModeBEU::Tick(float DeltaTime)
{
	
	/*APlayerController* player1 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APlayerController* player2 = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	APlayerController* temp;
	temp = player1;
	player1 = player2;
	player1 = temp;
	*/

}
