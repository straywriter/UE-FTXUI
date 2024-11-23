using UnrealBuildTool;
 
public class FTXUIUnitTest : ModuleRules
{
	public FTXUIUnitTest(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
	        "Core",
	        "GoogleTest",
	        "FTXUI"
        });
 
		PublicIncludePaths.AddRange(new string[] {"FTXUIUnitTest/Public"});
		PrivateIncludePaths.AddRange(new string[] {"FTXUIUnitTest/Private"});
	}
}