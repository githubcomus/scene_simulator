// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include <string>
#include "Json.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "EditEntityManageData.generated.h"

// 我的材质类型(根据功能部分区分)
enum EMyMeshMaterialType
{
	EMyMeshMaterialType_WhiteMold,			// 白膜
	EMyMeshMaterialType_OrdinaryMaterial,	// 普通材质 
	EMyMeshMaterialType_StaticNormalColor,	// 固定普通颜色
	EMyMeshMaterialType_StaticHighhight,	// 固定高光颜色
	EMyMeshMaterialType_DynamicNormalColor,	// 渐变普通颜色
	EMyMeshMaterialType_DynamicHighhight,	// 渐变高光颜色

};

/**
 *
 */
class ABaseGameMode;
class AEditEntityManage;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class USceneStaticMeshComponent;
class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class AEditEntityManage;
class ABaseHUD;
class AEditEntityActor;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UBoxComponent;
//class UPlaceStaticMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEditEntityManageData : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	// 常用指针
	TWeakObjectPtr<UBaseGameInstance> gameInstance;

	// 常用指针
	TWeakObjectPtr<APlayerController> playerController;

	// 常用指针
	TWeakObjectPtr<ABaseGameMode> gameMode;

	// 常用指针
	TWeakObjectPtr<ABaseHUD> hud;

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	// 视图类
	TWeakObjectPtr<AEditEntityManage> view;

	// 控制类
	TWeakObjectPtr<UEditEntityManageController> controller;

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	// box搜索entityId
	struct PlaceBoxInfo
	{
		//
		int32 entityId = 0;

		//
		int32 index = 0;
	};
	TMap<UBoxComponent*, PlaceBoxInfo> collisionBoxMap;

	// 选中材质
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UMaterialInstanceConstant* selectMaterial;

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	//
	UEditEntityManageData();

	//
	void Start(TWeakObjectPtr<UEditEntityManageController> controller_, TWeakObjectPtr<AEditEntityManage> view_);

	//
	void End();

	bool CheckDefaultData();
	bool IsExistData(int32 entityId);

	// 获取联网数据
	Gamedata::EntityInfo* GetSyncInfo(int32 entityId);

	// 获取本地数据
	//FLocalEntityInfo* GetLocalInfo(int32 entityId);

	bool IsExistXlsInfo(int32 xlsId);
	// 获取表格数据
	xlsdata::PlaceTableDefine* GetXlsInfo(int32 entityId);

	// 获取表格数据
	xlsdata::PlaceTableDefine* GetXlsInfoWithXlsId(int32 xlsId);

	// 通过碰撞体获取实例索引
	bool GetEntityIdWithBox(UBoxComponent* box, int& entityId, int32& index);
	xlsdata::MaterialTableDefine* GetMaterialInfoWithId(int32 xlsId);
	int GetLocalPlayerId();

	TWeakObjectPtr<USceneStaticMeshComponent> GetEntityMeshComponent(int actorMapId);
	void LoadSelectMaterial();
	TWeakObjectPtr<AEditEntityActor> GetEntityActor(int32 entityId);
	Gamedata::EntityMeshMasterInfo* GetMergeInfoWithXlsId(int xlsId);
	EMyMeshMaterialType GetChildMaterialType(Gamedata::EntityInfo* syncInfo, int childIndex);
	FString GetHightlightMaterialPath(bool highlight);
	/*------------------------------------------------------------------*/
};
