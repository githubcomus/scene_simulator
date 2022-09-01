// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//#include "Protobuf/game.pb.h"
#include "EntityMaterialInfo.generated.h"

class APlaceViewLaserLightActor;

class UBoxComponent;
class UMaterialInstanceConstant;
class UWidgetComponent;
class UUserWidget;
//
//// 实例 命令控制 类型
//UENUM(BlueprintType)
//enum class EEntityCommandType : uint8 {
//	EntityCommandType_auto = 0 UMETA(DisplayName = "自动播放"),
//	EntityCommandType_command = 1 UMETA(DisplayName = "命令"),
//	EntityCommandType_interactive = 2 UMETA(DisplayName = "交互"),
//	EntityCommandType_trigger = 3 UMETA(DisplayName = "触发"),
//};
//
//// 实体 命令控制 命令
//USTRUCT(BlueprintType)
//struct FEntityCommandCommand {
//	GENERATED_BODY()
//
//public:
//
//	// 命令文本
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		FString command_name = TEXT("");
//
//	// 开始时间 
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		float start_time = 0;
//
//	// 结束时间 
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		float end_time = 0;
//
//};
//
//// 实体 命令控制 交互
//USTRUCT(BlueprintType)
//struct FEntityCommandInteractive {
//	GENERATED_BODY()
//
//public:
//	// 固定位置
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		bool is_fixed_position = false;
//
//	// 交互状态
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		bool is_open = false;
//
//	// 开始时间 
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		float start_time = 0;
//
//	// 结束时间 
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		float end_time = 0;
//};
//
//// 实体 命令控制 触发
//USTRUCT(BlueprintType)
//struct FEntityCommandTrigger {
//	GENERATED_BODY()
//
//public:
//	// 触发状态
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		bool is_open = false;
//
//	// 开始时间 
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		float start_time = 0;
//
//	// 结束时间 
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		float end_time = 0;
//};
//
//
//// [实例命令控制器] 信息
//USTRUCT(BlueprintType)
//struct FEntityCommandInfo
//{
//	GENERATED_BODY()
//
//public:
//
//	// 类型
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		EEntityCommandType type = EEntityCommandType::EntityCommandType_auto;
//
//	// 命令 key 唯一标记,value:信息
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		TArray<FEntityCommandCommand> command_list;
//
//	// 交互
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		FEntityCommandInteractive interactive;
//
//	// 触发
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		FEntityCommandTrigger trigger;
//};




