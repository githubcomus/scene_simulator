// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Yxjs : ModuleRules
{
    public Yxjs(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // UE4默认的情况下不允许使用Exception,加入PB后报错C4530,该选项启用
        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore",
            "HeadMountedDisplay", "Json", "JsonUtilities",
            "UMG","Slate","SlateCore","RenderCore" ,"ImageWrapper","Protobuf"
            ,"AIModule","PhysXVehicles", "ChaosVehicles", "PhysicsCore", 
            //"DLSSBlueprint"
        });

        // 确保复制重写框架类的时候不会爆红
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore",
            "HeadMountedDisplay", "Json", "JsonUtilities",
            "UMG","Slate","SlateCore","RenderCore" ,"ImageWrapper","Protobuf"
            ,"AIModule", "PhysXVehicles","ChaosVehicles", "PhysicsCore" , 
            //"DLSSBlueprint"
        });

        PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            System.Console.WriteLine("###########################################################");
            System.Console.WriteLine(ModuleDirectory);
            System.Console.WriteLine("#####################End################################");

            //string pbInclude = "D:/Yxjs/Client/Yxjs/Source/Protobuf/ThirdParty/protobuf/include/";
            // PublicSystemIncludePaths.Add(pbInclude);

            PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, ""));

            // pb包含
            PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "../Protobuf/ThirdParty/protobuf/include"));

            // 测试静态库
            PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "../ThirdPart/bp/include"));
            PublicSystemLibraryPaths.Add(Path.Combine(ModuleDirectory, "../ThirdPart/bp/lib"));
            PublicSystemLibraries.Add("Project2d.lib");
            PublicSystemLibraries.Add("Project2r.lib");
        }

    }
}
