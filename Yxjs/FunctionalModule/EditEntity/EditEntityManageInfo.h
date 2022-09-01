// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//#include "Widget.h"
#include "Protobuf/game.pb.h"
#include "EditEntityManageInfo.generated.h"

class APlaceViewLaserLightActor;
class AEditEntityActor;
class AEditEntityActor;

class UBoxComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UMaterialInstanceConstant;
class UWidgetComponent;

#define BuildObject ECC_GameTraceChannel2


#define PreviewEntityID -10


// 本地静态网格数据
USTRUCT()
struct FLocalMeshInfo
{
	GENERATED_BODY()

public:

	// 静态网格世界变换
	FTransform wTransform;

	// 静态网格相对变换(只是xls表格中相对原点的变换,而非世界变换与组件位置的相对变换)
	FTransform rTransform;

};

// 本地碰撞点数据
USTRUCT()
struct FLocalCollisionInfo
{
	GENERATED_BODY()

public:

	// 碰撞盒体世界变换
	//FTransform wTransform;

	// 碰撞盒体相对变换(只是xls表格中相对原点的变换,而非世界变换与组件位置的相对变换)
	FTransform rTransform;

};


//----------------------

// 统统废弃


// 本地激光束数据
USTRUCT()
struct FLocalLaserBeamInfo
{
	GENERATED_BODY()

public:

	// 是否开启预览
	bool isOpenPreview = false;

	// 开启预览时间
	float previewTime = 0;

	// 播放速度
	float speed = 0;

	// 当前播放序列
	int playIndex = 0;

	// 上一个执行到的时间节点
	int lastTimeRun = -1;

	// 网格组件名称
	FString meshName = TEXT("");

	// 静态网格组件(多个实例对应一个组件)
	TWeakObjectPtr<UHierarchicalInstancedStaticMeshComponent> meshComponent = nullptr;

	// 静态网格世界变换(灯座位置)
	FTransform meshWTransform;

	// 静态网格相对变换(只是xls表格中相对原点的变换,而非世界变换与组件位置的相对变换)
	FTransform meshRTransform;

	// 静态网格真实的世界变换(灯光束位置)
	FTransform laserWTransform;

	// 静态网格索引
	int32 meshInstanceIndex = 0;

	// 选中网格 名称
	FString selectMeshName = TEXT("");

	// 选中网格 指针(选中时生成/销毁)
	UPROPERTY()
		UStaticMeshComponent* selectMesh = nullptr;

	// 选中网格 静态网格资源
	UPROPERTY()
		UStaticMesh* selectStaticMesh = nullptr;
};

// 本地照片数据
USTRUCT()
struct FLocalImageInfo
{
	GENERATED_BODY()

public:

	// 静态网格资源正面
	UPROPERTY()
		UWidgetComponent* widgetPositiveComponent = nullptr;

	// 静态网格资源背面
	UPROPERTY()
		UWidgetComponent* widgetBackComponent = nullptr;

	// 图片基准大小
	FVector2D imageSize;

	// url,本地缓存用于跟sync对比
	FString url = TEXT("");;
};

// 实例本地数据
USTRUCT()
struct FLocalEntityInfo
{
	GENERATED_BODY()

public:

	//------------------------------------------------- 待废弃

	// 多个碰撞数据
	UPROPERTY()
		TArray<FLocalCollisionInfo> multipleCollision;

	// 多个静态网格数据
	UPROPERTY()
		TArray < FLocalMeshInfo> multipleMesh;

};

// 选中网格信息
USTRUCT()
struct FSelectMeshComponentInfo
{
	GENERATED_BODY()

public:

	// 静态网格
	UPROPERTY()
		UStaticMeshComponent* component = nullptr;

	// 资源
	TSharedPtr<struct FStreamableHandle> handle;

	// 使用计数器(销毁静态网格)
	int count = 0;
};

// 主线程变换数据
struct TimelineOneFrameOneEntityData
{
	std::vector<FTransform> child_transform;	// 组件世界位置
	TWeakObjectPtr<AEditEntityActor> actor;	//
};