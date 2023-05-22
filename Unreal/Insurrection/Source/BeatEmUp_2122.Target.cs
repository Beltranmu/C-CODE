// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BeatEmUp_2122Target : TargetRules
{
	public BeatEmUp_2122Target( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "BeatEmUp_2122" } );
        Type = TargetType.Game;
        bUsesSteam = true;
    }
        

   
}
