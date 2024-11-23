// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class FTXUI : ModuleRules
{
	public FTXUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;
		Type = ModuleType.CPlusPlus;
		bEnableUndefinedIdentifierWarnings = false;
		ModuleIncludePathWarningLevel = WarningLevel.Warning;
		bWarningsAsErrors = false;
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

		PublicDependencyModuleNames.Add("Core");
	}
}
