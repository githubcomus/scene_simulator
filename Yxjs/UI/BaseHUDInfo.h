// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseHUDInfo.generated.h"

// 输入模式
UENUM(BlueprintType)
enum class ECharacterStatusType : uint8
{
	CharacterStatusType_None = 0   UMETA(DisplayName = "游客模式"),
	CharacterStatusType_EditEntity = 1   UMETA(DisplayName = "编辑实例"),
	CharacterStatusType_EditTimeline = 2   UMETA(DisplayName = "编辑时间轴"),
	CharacterStatusType_Preview = 3   UMETA(DisplayName = "实例预览"),
	CharacterStatusType_LockEntity = 4   UMETA(DisplayName = "锁定实例"),
	CharacterStatusType_Player_Mouse = 5   UMETA(DisplayName = "玩家鼠标点击"),

};

// 临时数据 类型
UENUM(BlueprintType)
enum class EUITempDateType : uint8
{
	UITempDateType_none,
	UITempDateType_EditEntity,
	UITempDateType_Timeline,
	UITempDateType_CommandTrigger,
};


// 主操作模式(决定多种子UI和多种输入模式)
UENUM(BlueprintType)
enum class EGameMainOperationMode : uint8
{
	game_main_operation_mode_none = 0   UMETA(DisplayName = "无"),
	game_main_operation_mode_base_character = 1   UMETA(DisplayName = "操作玩家"),
	game_main_operation_mode_racing = 2   UMETA(DisplayName = "操作赛车"),
	game_main_operation_mode_edit_etity = 3   UMETA(DisplayName = "编辑实例"),
};

// 子UI页面
UENUM(BlueprintType)
enum class EUIChildName : uint8
{
	ui_none = 0   UMETA(DisplayName = "空"),     //
	ui_name_mainui = 1   UMETA(DisplayName = "玩家界面"),     //
	ui_name_edit_entity = 3   UMETA(DisplayName = "编辑界面"),   //
	ui_name_entity_backpack = 4   UMETA(DisplayName = "资源背包界面"),   //
	ui_name_timeline = 5   UMETA(DisplayName = "时间轴"),   //
	ui_name_command_trigger = 11   UMETA(DisplayName = "命令触发"),//
	ui_name_command_button = 12   UMETA(DisplayName = "命令按钮"),//
	ui_name_entity_material = 13   UMETA(DisplayName = "实例材质"),   //
	ui_name_entity_storage = 14   UMETA(DisplayName = "实例收纳"),   //
	ui_name_sky_level = 15   UMETA(DisplayName = "场景切换"),   //
	ui_name_setting = 16   UMETA(DisplayName = "设置"),   //
	ui_name_character = 17   UMETA(DisplayName = "角色切换"),   //

	// 暂不使用
	ui_name_racing = 2   UMETA(DisplayName = "赛车界面"),     //

	// 等待开发
	ui_name_laser = 6   UMETA(DisplayName = "激光"),      //
	ui_name_sky = 7   UMETA(DisplayName = "天气"),        //
	ui_name_python = 9   UMETA(DisplayName = "python"),//
	ui_name_new_timeline = 10   UMETA(DisplayName = "新的时间轴"),//

	// 已经废弃
	ui_name_entity_merge = 8   UMETA(DisplayName = "实例合并"),
};


// 常驻UI页面
UENUM(BlueprintType)
enum class EPermanentName : uint8
{
	ui_name_tips = 0   UMETA(DisplayName = "提示信息"),     //
	ui_name_input_help = 1   UMETA(DisplayName = "操作帮助"),     //
};