// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeatEmUp_2122.h"
#include "Modules/ModuleManager.h"

#include "Internationalization/StringTableRegistry.h"
#include "Misc/Paths.h"

void FBeatEmUp_2122::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();

	//LOCTABLE_FROMFILE_GAME("MyDialogs", "AnyOldNamespace", "/Data/Dialogs.csv");

}


void FBeatEmUp_2122::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FBeatEmUp_2122, BeatEmUp_2122, "BeatEmUp_2122" );
