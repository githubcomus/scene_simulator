// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include <vector>
#include "SelectedInfo.generated.h"

#define SelectedInstance ECC_GameTraceChannel1
#define BuildObject ECC_GameTraceChannel2

//  当前移动策略
UENUM(BlueprintType)
enum class ESelectedMoveStrategy : uint8
{
	/** None*/
	None UMETA(DisplayName = "None"),
	/** LocationX*/
	LocationX UMETA(DisplayName = "LocationX"),
	/** LocationY*/
	LocationY UMETA(DisplayName = "LocationY"),
	/** LocationZ*/
	LocationZ UMETA(DisplayName = "LocationZ"),
	/** rotatorPitch*/
	RotatorPitch UMETA(DisplayName = "RotatorPitch"),
	/** rotatorYaw*/
	RotatorYaw UMETA(DisplayName = "rotatorYawaw"),
	/** rotatorRoll*/
	RotatorRoll UMETA(DisplayName = "RotatorRoll"),
	/** ScaleX*/
	ScaleX UMETA(DisplayName = "ScaleX"),
	/** ScaleY*/
	ScaleY UMETA(DisplayName = "ScaleY"),
	/** ScaleZ*/
	ScaleZ UMETA(DisplayName = "ScaleZ"),
	/** planeMove*/
	planeMove UMETA(DisplayName = "MyPlaneMove")
};

// 选中实例新的位置数据
struct SelectInstanceInfo
{
	// 联网数据索引
	int32 entityId = 0;

	// 选中索引
	std::vector<int32> selectIndexList;

	// 移动前物体世界变换
	FTransform oldSpawnWTransform;

	// 移动前物体相对于中心物体的世界变换(便于多物体环绕中心物体旋转)
	FTransform rWithCoreTransform;
};

//  当前工作模式
UENUM(BlueprintType)
enum class ESelectedWorkMode : uint8
{
	/** 世界平移*/
	WorldLocation UMETA(DisplayName = "WorldLocation"),

	/** 世界旋转*/
	WorldRotator UMETA(DisplayName = "WorldRotator"),

	/** 缩放*/
	Scale UMETA(DisplayName = "Scale"),

	/** 相对平移*/
	RelativelyLocation UMETA(DisplayName = "RelativelyLocation"),

	/** 相对旋转*/
	RelativelyRotator UMETA(DisplayName = "RelativelyRotator"),

	/** 绕轴旋转,选中的卫星实体*/
	CoreRotator UMETA(DisplayName = "CoreRotator"),

	/** 平面移动*/
	// PlaneMove UMETA(DisplayName = "PlaneMove"),

	/** 世界形变*/
	WorldTransform UMETA(DisplayName = "WorldTransform"),

	/** 相对形变*/
	RelativelyTransform UMETA(DisplayName = "RelativelyTransform"),

};

//  当前UI命令
UENUM(BlueprintType)
enum class EEditUIBaseOrder : uint8
{
	/** None*/
	None UMETA(DisplayName = "None"),

	/** 世界平移*/
	WorldLocation UMETA(DisplayName = "WorldLocation"),

	/** 世界旋转*/
	WorldRotator UMETA(DisplayName = "WorldRotator"),

	/** 缩放*/
	Scale UMETA(DisplayName = "Scale"),

	/** 相对平移*/
	RelativelyLocation UMETA(DisplayName = "RelativelyLocation"),

	/** 相对旋转*/
	RelativelyRotator UMETA(DisplayName = "RelativelyRotator"),

	/** 切换速度*/
	ChangeSpeed UMETA(DisplayName = "ChangeSpeed"),

	/** 到达选中*/
	ComeSelected UMETA(DisplayName = "ComeSelected"),

	/** 开启移动网格*/
	OpenMoveItem UMETA(DisplayName = "OpenMoveItem"),

	/** 新增实例*/
	NewInstance UMETA(DisplayName = "NewInstance"),

	/** 复制实例*/
	CopyInstance UMETA(DisplayName = "CopyInstance"),

	/** 删除实例*/
	DeleteInstance UMETA(DisplayName = "DeleteInstance"),
};

// 多选数据
struct CvMultipleInstanceInfo
{
	// 多选
	bool isMultipleChoice = false;

	// 选中实例列表(最后一个为主实例)
	TArray<SelectInstanceInfo> selectedList;
};