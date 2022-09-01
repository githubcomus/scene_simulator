// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "EditEntityManageController.generated.h"

class UEditEntityManageData;
class AEditEntityManage;
class UArrowComponent;
class UPlaceControllerMoveTimelineObject;
class UPlaceControllerScenesTimelineObject;
class AEditEntityActor;

class UImage;

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UEditEntityManageController : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 管理类
	TWeakObjectPtr < UEditEntityManageData> data = nullptr;

	// 视图类
	TWeakObjectPtr < AEditEntityManage> view = nullptr;

	/*------------------------------------------------------------------*/

	// 等待移除

	// 位移时间轴
	UPROPERTY()
		UPlaceControllerMoveTimelineObject* moveTimelineController = nullptr;

	// 场景时间轴
	UPROPERTY()
		UPlaceControllerScenesTimelineObject* scenesTimelineController = nullptr;

	/*------------------------------------------------------------------*/

	bool IsStaticColorChange(Gamedata::EntityInfo* oldSyncInfo, Gamedata::EntityInfo* newSyncInfo);
	bool IsMaterialTypeChange(Gamedata::EntityInfo* oldSyncInfo, Gamedata::EntityInfo* newSyncInfo);
	bool IsMaterialIdChange(Gamedata::EntityInfo* oldSyncInfo, Gamedata::EntityInfo* newSyncInfo);
	//
	UEditEntityManageController();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*------------------------------------------------------------------*/

	//
	void Start(TWeakObjectPtr < UEditEntityManageData> data_, TWeakObjectPtr < AEditEntityManage> view_);

	//
	void End();

	// 新增数据
	void NetCallback_EntityAdd(Gamedata::EntityInfo* syncInfo_);

	// 移除数据
	void NetCallback_EntityRemove(int32 entityId);

	// 更新网络数据
	void NetCallback_EntityUpdate(Gamedata::EntityInfo* syncInfo_);
	void RemoveChooseAxis(int32 entityId);
	void OnlineResources_Callback_SetImage(int32 entityId, FVector2D imageSize, UImage* image, TWeakObjectPtr<UTexture2D> texture2D);

	void NetCall_NewData(Gamedata::EntityInfo syncInfo);

	// 网络请求 新增数据
	void NetCall_NewData(TArray<int32> entityIdList);

	// 网络请求 更新数据
	void NetCall_UpdateData(TArray<int32> entityIdList);

	void NetCall_RemoveData(TArray<int32> entityIdList);
	void NetCall_MergeEntity(std::vector<Gamedata::EntityInfo*>nodeEntityIdList);
	void NetCall_BreakDownEntity(std::vector<Gamedata::EntityInfo*>nodeEntityIdList);
};
