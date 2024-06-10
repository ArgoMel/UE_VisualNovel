using UnrealBuildTool;

public class VisualNovel : ModuleRules
{
	public VisualNovel(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
            "DlgSystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
