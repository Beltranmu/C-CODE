// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/CommonBEU.h"
#include "ThrowableInterface.generated.h"


class ATurboPlayer;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UThrowableInterface : public UInterface
{
	GENERATED_BODY()

};

class BEATEMUP_2122_API IThrowableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION()
	virtual void Pick(AActor* self, ATurboPlayer* player);

	UFUNCTION()
	virtual void Throw(FVector dir);

	UFUNCTION()
		virtual void OverlapWithEnemy(AActor* self, AActor* other);
	

};
