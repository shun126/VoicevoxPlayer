// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxPlayer : ModuleRules
{
	public VoicevoxPlayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		// Add any macros that need to be set
		// PublicDefinitions.Add("WITH_MYTHIRDPARTYLIBRARY=1");

		{
			// Add any include paths for the plugin
			PublicIncludePaths.Add(System.IO.Path.Combine(ModuleDirectory, "ThirdParty", "voicevox_core"));

            // Add any import libraries or static libraries
            PublicAdditionalLibraries.Add(System.IO.Path.Combine(ModuleDirectory, "ThirdParty", "voicevox_core", "voicevox_core.lib"));

            //string exampleDllName = "ExampleDll.dll";
            //string exampleDllFullPath = Path.Combine(exampleDllPath, exampleDllName);
            //CopyDll(exampleDllName, exampleDllFullPath);
            //RuntimeDependencies.Add(System.IO.Path.Combine(ModuleDirectory, "ThirdParty", "voicevox_core"));
            //PublicDelayLoadDLLs.Add("voicevox_core.dll");
            PublicDelayLoadDLLs.Add(System.IO.Path.Combine(ModuleDirectory, "ThirdParty", "voicevox_core", "voicevox_core.dll"));
        }
    }


    // Binariesà»â∫Ç…dll ÇÉRÉsÅ[Ç∑ÇÈ
    private void CopyDll(string dllName, string dllFullPath)
    {
        if (!System.IO.File.Exists(dllFullPath))
        {
            System.Console.WriteLine("file {0} does not exist", dllName);
            return;
        }
        string binariesDir = System.IO.Path.Combine(ModuleDirectory, "../../Binaries/Win64/");
        if (!System.IO.Directory.Exists(binariesDir))
        {
            System.IO.Directory.CreateDirectory(binariesDir);
        }
        string binariesDllFullPath = System.IO.Path.Combine(binariesDir, dllName);
        if (System.IO.File.Exists(binariesDllFullPath))
        {
            System.IO.File.SetAttributes(binariesDllFullPath, System.IO.File.GetAttributes(binariesDllFullPath) & ~System.IO.FileAttributes.ReadOnly);
        }
        try
        {
            System.IO.File.Copy(dllFullPath, binariesDllFullPath, true);
        }
        catch (System.Exception ex)
        {
            System.Console.WriteLine("failed to copy file: {0}", dllName);
        }
    }
}
