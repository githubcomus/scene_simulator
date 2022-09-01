// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Protobuf/game.pb.h"
#include "TimelineInfo.generated.h"

class APlaceViewLaserLightActor;

class UBoxComponent;
class UMaterialInstanceConstant;
class UWidgetComponent;
class UUserWidget;


// ui数据
struct CvTimelineUIInfo
{
	FVector location;
	FRotator rotator;
	FVector color;
	int highlight;
	bool locationTeleport;
	bool colorTeleport;
	float time;
};

//// [时间轴主页面] 信息
//USTRUCT(BlueprintType)
//struct FTimelineMainUIInfo
//{
//	GENERATED_BODY()
//
//public:
//
//	// 平移
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		FVector location;
//
//	// 旋转
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		FRotator rotation;
//
//	// 颜色
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		FVector color;
//
//	// 高光
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		bool isHighlight = false;
//
//	// 位置瞬移
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		bool locationTeleport = false;
//
//	// 颜色瞬移
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		bool colorTeleport = false;
//
//};


// 时间轴通用操作
enum ETimelineOperationType
{
	TimelineOperation_UpdateEntityDataToUIData_create,
	TimelineOperation_UpdateEntityDataToUIData_remove,
	TimelineOperation_UpdateEntityDataToUIData_update,

	// 鼠标右键拖动
	TimelineOperation_ViewMove_MouseRightMove,
	// 鼠标左键键拖动
	TimelineOperation_ViewMove_MouseLeftMove,
	// game选中移动
	TimelineOperation_ViewMove_GameMove,
	// init
	TimelineOperation_ViewMove_Init,

	// 鼠标右键拖动
	TimelineOperation_PointMove_MouseRightMove,
	// 鼠标左键键拖动
	TimelineOperation_PointMove_MouseLeftMove,
	// game选中移动
	TimelineOperation_PointMove_GameMove,
	// init
	TimelineOperation_PointMove_Init,
};

//  点类型
enum ETimelinePointType
{
	TimelinePointType_location,

	TimelinePointType_rotator
};

// [实例时间轴]时间间隔数据
USTRUCT(BlueprintType)
struct FTimelineTimeInfo
{
	GENERATED_BODY()

public:

	// 位移x
	float x = 0;

	// 格子索引
	float itemIndex = 0;

	// 时间间隔
	TWeakObjectPtr<UUserWidget> timeUserWidget = nullptr;

	// 待删除
	bool isDelete = false;
};

// [实例时间轴]实例节点数据
USTRUCT(BlueprintType)
struct FTimelineEnterPointInfo
{
	GENERATED_BODY()

public:

	// 位移x
	float x = 0;

	// 位移y
	float y = 0;

	// 实例id
	int entityId = 0;

	// 时间轴显示时间
	float showTime = 0;

	// sync数据keytime
	int keyTime = 0;

	// 本地唯一标记
	int uniqueMark = 0;

	// 点
	TWeakObjectPtr<UUserWidget> pointUserWidget = nullptr;

	// 选中
	bool isSelect = false;
};

// [实例时间轴]实例数据
USTRUCT(BlueprintType)
struct FTimelineEntityLineInfo
{
	GENERATED_BODY()

public:

	// 所在索引
	int timelineIndex = 0;

	// 所在高度(遍历数据是否显示)
	float y = 0;

	// 开始时间
	float start = 0;

	// 结束时间
	float end = 0;

	// 点数据
	TArray<FTimelineEnterPointInfo> pointInfoList;

	// 左侧的行
	TWeakObjectPtr<UUserWidget> leftLineUserWidget = nullptr;

	// 中间的行
	TWeakObjectPtr<UUserWidget> centerLineUserWidget = nullptr;

	//
	int32 entityId = 0;

	// 选中实例
	bool isSelect = false;
};

// 选中点的索引
struct FTimelinePointIndex
{
	// entityLineInfoMap key
	int32 entityId = 0;

	// 唯一值
	int uniqueMark = 0;

	// 是否删除
	bool isDeleted = false;

public:

	FTimelinePointIndex() {};

	FTimelinePointIndex(int32 entityId_, int uniqueMark_)
	{
		entityId = entityId_;
		uniqueMark = uniqueMark_;
	}
};
