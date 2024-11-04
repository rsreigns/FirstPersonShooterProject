// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSProject : ModuleRules
{
	public FPSProject(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","HTTP", "Json", "JsonUtilities","GameplayTasks","UMG"});
        PrivateDependencyModuleNames.AddRange(new string[] { "HTTP" });
    }

}
