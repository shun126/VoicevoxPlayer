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
				"RuntimeAudioImporter",
            }
            );
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
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
            //PublicDelayLoadDLLs.Add(System.IO.Path.Combine(ModuleDirectory, "ThirdParty", "voicevox_core", "voicevox_core.dll"));
        }

        {
            string voicevoxCorePath = System.IO.Path.Combine(ModuleDirectory, "ThirdParty", "voicevox_core");
            string destinationPath = System.IO.Path.Combine(ModuleDirectory, "..", "..", "Binaries", "Win64");

            CopyDirectory(voicevoxCorePath, destinationPath, "model", true);
            CopyDirectory(voicevoxCorePath, destinationPath, "open_jtalk_dic_utf_8-1.11", true);
            CopyFile(voicevoxCorePath, destinationPath, "voicevox_core.dll");
            CopyFile(voicevoxCorePath, destinationPath, "onnxruntime.dll");
            CopyFile(voicevoxCorePath, destinationPath, "onnxruntime_providers_shared.dll");
        }
    }

    private void CopyFile(string sourcePath, string destinationPath, string filename)
    {
        string sourceFileName = System.IO.Path.Combine(sourcePath, filename);
        string destinationFileName = System.IO.Path.Combine(destinationPath, filename);
        try
        {
            System.IO.File.Copy(sourceFileName, destinationFileName, true);
        }
        catch (System.Exception ex)
        {
            System.Console.WriteLine("failed to copy file: {0} to {1}", sourceFileName, destinationFileName);
        }
    }

    static void CopyDirectory(string sourcePath, string destinationPath, string directoryName, bool recursive)
    {
        System.IO.DirectoryInfo sourceDirectory = new System.IO.DirectoryInfo(
            System.IO.Path.Combine(sourcePath, directoryName)
        );
        System.IO.DirectoryInfo destinationDirectory = new System.IO.DirectoryInfo(
            System.IO.Path.Combine(destinationPath, directoryName)
        );

        if (!sourceDirectory.Exists)
        {
            throw new System.IO.DirectoryNotFoundException("Source directory not found: " + sourceDirectory.FullName);
        }

        if (!destinationDirectory.Exists)
        {
            destinationDirectory.Create();
            destinationDirectory.Attributes = sourceDirectory.Attributes;
        }

        foreach (System.IO.FileInfo file in sourceDirectory.GetFiles())
        {
            string targetFilePath = System.IO.Path.Combine(destinationDirectory.FullName, file.Name);
            file.CopyTo(targetFilePath, true);
        }

        if (recursive)
        {
            foreach (System.IO.DirectoryInfo subDirectory in sourceDirectory.GetDirectories())
            {
                string newDestinationDir = System.IO.Path.Combine(destinationDirectory.FullName, subDirectory.Name);
                CopyDirectory(subDirectory.FullName, newDestinationDir, "", true);
            }
        }
    }
}
