// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "GameViewportClientBEU.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_2122_API UGameViewportClientBEU : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	bool InputKey(const FInputKeyEventArgs& EventArgs) override;


	bool InputAxis(FViewport* Viewport_p, int32 ControllerId, 
								 FKey Key, float Delta, float DeltaTime, 
								 int32 NumSamples = 1, bool bGamepad = false) override;

};
