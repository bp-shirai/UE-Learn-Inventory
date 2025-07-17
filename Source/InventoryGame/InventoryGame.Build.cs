// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InventoryGame : ModuleRules
{
	public InventoryGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"InventoryGame/Public",
			"InventoryGame/Public/Variant_Platforming",
			"InventoryGame/Public/Variant_Combat",
			"InventoryGame/Public/Variant_Combat/AI",
			"InventoryGame/Public/Variant_SideScrolling",
			"InventoryGame/Public/Variant_SideScrolling/Gameplay",
			"InventoryGame/Public/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
