// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/NavigationConfig.h"

/**
 * 
 */
class BEATEMUP_2122_API FCustomNavigationConfig : public FNavigationConfig
{
public:
	FCustomNavigationConfig();
	virtual ~FCustomNavigationConfig();

	virtual EUINavigationAction GetNavigationActionForKey(const FKey& InKey) const override;
};
