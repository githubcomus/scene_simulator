// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SettingInfo.generated.h"


class UBoxComponent;
class UMaterialInstanceConstant;
class UWidgetComponent;
class UUserWidget;

// 设置条目类型
UENUM(BlueprintType)
enum class ESettingLineType : uint8 {
	ESettingLineType_None = 0 UMETA(DisplayName = "无"),
	ESettingLineType_Resolution = 1 UMETA(DisplayName = "分辨率"),
	ESettingLineType_Window = 2 UMETA(DisplayName = "窗口"),
	ESettingLineType_Quality = 3 UMETA(DisplayName = "品质"),
	ESettingLineType_Frames = 4 UMETA(DisplayName = "帧数"),
	ESettingLineType_Shadow = 5 UMETA(DisplayName = "阴影"),
	ESettingLineType_PostProcessing = 6 UMETA(DisplayName = "后处理"),
	ESettingLineType_VisualEffect = 7 UMETA(DisplayName = "视觉效果"),//
	ESettingLineType_Texture = 8 UMETA(DisplayName = "纹理"),
	ESettingLineType_ViewDistance = 9 UMETA(DisplayName = "视距"),
	ESettingLineType_AntiAliasing = 10 UMETA(DisplayName = "抗锯齿"),//
	ESettingLineType_VerticalSync = 11 UMETA(DisplayName = "垂直同步"),//
	ESettingLineType_DLSS = 12 UMETA(DisplayName = "DLSS"),//
	ESettingLineType_Vegetation = 13 UMETA(DisplayName = "植被"),//
};