// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include <string>
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "PlaceViewMeshComponent.generated.h"

/**
 *
 */
class ABaseGameMode;
class AEditEntityManage;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UEditEntityManageData;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UBoxComponent;
//class UPlaceStaticMeshComponent;
class UImage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlaceViewMeshComponent : public UActorComponent
{
    GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 加载图片类型
	enum ELoadResourcesType
	{
		LoadRes_Image,
		LoadRes_Mesh,
		LoadRes_Mesh2,
		LoadRes_Mesh3,
		LoadRes_Material,
		LoadRes_Material2,
	};

	// 资源信息
	struct CvResourcesInfo
	{
		//
		TSharedPtr<struct FStreamableHandle> handle;

		// 路径
		FString path;
	};

	// 摆放数据
	struct CvEditEntityManageInfo
	{
	};

	// 选中网格数据
	struct CvSelectMeshInfo
	{
		// 网格组件对应实例索引字典 key是网格指针,value是同步数据索引
		TMap<UStaticMeshComponent*, int32> isntanceMap;
	};

    /*------------------------------------------------------------------*/

    // 视图类
    AEditEntityManage* view;

    /*------------------------------------------------------------------*/

	// 选中网格数据
	CvSelectMeshInfo cvSelectMeshInfo;

	// 选中材质
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UMaterialInstanceConstant* selectMaterial;

	// 资源信息
	TMap<UObject*, struct CvResourcesInfo> resMap;

	// 网格实例的缓存,等待模型加载完后回调
	//TMap<USceneComponent*, TArray<int32>> meshInstanceCachingMap;

	/*------------------------------------------------------------------*/

    //
    UPlaceViewMeshComponent();

    /*------------------------------------------------------------------*/

    //
    void Start(AEditEntityManage * view_);

    //
    void End();

    /*------------------------------------------------------------------*/
	// 加载资源
	void LoadRes(bool satrt, FString path, enum ELoadResourcesType type, UObject* obj);

		// 创建网格实例
	void CreateMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool mast = false);

	// 销毁网格组件
	void DestroyMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool mast = false);

	// 移动盒体碰撞
	void UpdateMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	//// 创建网格组件
	//bool CreateMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	 
	//// 销毁网格组件
	//void DestroyMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	// 选中网格 创建
	void CreateSelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	// 选中网格 移动
	void UpdateSelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
    void SetSelectMeshHide(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool isShow);

    // 选中网格 销毁
	void DestroySelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	// 选中网格 发光材质加载
	void LoadSelectMaterial();

	// 选中网格  发光材质替换
	void ReplaceSelectMaterial(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);


	void MeshLoadMaterial(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);


};
