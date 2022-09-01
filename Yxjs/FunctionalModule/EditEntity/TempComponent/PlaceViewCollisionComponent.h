// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include <string>
#include "PlaceViewCollisionComponent.generated.h"

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
class  UPlaceViewCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

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

	// 资源信息
	TMap<UObject*, struct CvResourcesInfo> resMap;

	/*------------------------------------------------------------------*/

	// 视图类
	AEditEntityManage* view;

	/*------------------------------------------------------------------*/

	//
	UPlaceViewCollisionComponent();

	/*------------------------------------------------------------------*/

	//
	void Start(AEditEntityManage* view_);

	//
	void End();

	/*------------------------------------------------------------------*/

	//// 添加盒体碰撞
	//void CreateBox(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	//// 移除盒体碰撞
	//void DestroyBox(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	//// 添加盒体碰撞
	//void UpdateBox(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool changeScale);
	//void BoxSetImageSize(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	//bool CreateCollisionMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	//void DestroyCollisonMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	//void ShowCollisionMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	//void HiddenCollisionMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	// 加载资源
	void LoadRes(bool satrt, FString path, enum ELoadResourcesType type, UObject* obj);
};
