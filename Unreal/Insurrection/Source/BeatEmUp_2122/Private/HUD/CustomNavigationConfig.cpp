// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CustomNavigationConfig.h"

FCustomNavigationConfig::FCustomNavigationConfig() : FNavigationConfig()
{

	KeyEventRules.Emplace(EKeys::A, EUINavigation::Left);
	KeyEventRules.Emplace(EKeys::Gamepad_DPad_Left, EUINavigation::Left);

	KeyEventRules.Emplace(EKeys::D, EUINavigation::Right);
	KeyEventRules.Emplace(EKeys::Gamepad_DPad_Right, EUINavigation::Right);
	
	KeyEventRules.Emplace(EKeys::W, EUINavigation::Up);
	KeyEventRules.Emplace(EKeys::Gamepad_DPad_Up, EUINavigation::Up);
	
	KeyEventRules.Emplace(EKeys::S, EUINavigation::Down);
	KeyEventRules.Emplace(EKeys::Gamepad_DPad_Down, EUINavigation::Down);

	AnalogHorizontalKey = EKeys::Gamepad_LeftX;
	AnalogVerticalKey = EKeys::Gamepad_LeftY;

}

FCustomNavigationConfig::~FCustomNavigationConfig()
{

}

EUINavigationAction FCustomNavigationConfig::GetNavigationActionForKey(const FKey& InKey) const
{

	//Super::GetNavigationActionForKey(InKey);
	
	if (InKey == EKeys::Enter || InKey == EKeys::SpaceBar || InKey == EKeys::Virtual_Accept || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
	
		return EUINavigationAction::Accept;
	
	}
	else if (InKey == EKeys::Escape || InKey == EKeys::BackSpace || InKey == EKeys::Gamepad_FaceButton_Right ||InKey == EKeys::Virtual_Back)
	{

		return EUINavigationAction::Back;

	}
	
	return EUINavigationAction::Invalid;

}
