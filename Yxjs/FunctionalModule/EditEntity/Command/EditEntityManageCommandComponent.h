// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include <string>
#include <memory>
#include <list>
#include <map>

#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "Tools/StringRingBuffer.h"
#include "EditEntityManageCommandComponent.generated.h"

/**
 *
 */
class ABaseGameMode;
class AEditEntityManage;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UEditEntityManageData;
class AEditEntityActor;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UBoxComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UImage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UEditEntityManageCommandComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 视图类
	AEditEntityManage* view;

	/*------------------------------------------------------------------*/

	/**主线程*/

	// 线程数据
	struct TransformThreadInfo
	{
		// tick执行时间
		float lastTimeTick = 0;

		// 每帧等待时间
		float waitTime = 1.0 / 60;

		// 动态帧数
		int dynamicFrameCount = 0;

	};
	TransformThreadInfo threadInfo;

	// tick
	bool isTick = false;

	/*------------------------------------------------------------------*/

	// 任务 实例-任务 字典 key:entityId value:info
	std::map<int32, TWeakObjectPtr<AEditEntityActor>> entityJobMap;

	// 任务 当前帧待删除数据
	std::list<int> deleteEntityList;

	/*------------------------------------------------------------------*/

	// 命令文本 命令-实例 字典
	std::map<std::string, std::list<int>> entityCommandMap;

	// 命令文本 实例-命令 字典
	std::map<int, std::list<std::string>> commandEntityMap;

	// 命令文本 实例列表
	std::list<int> entityList;

	/*------------------------------------------------------------------*/

	//
	UEditEntityManageCommandComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	std::list<int>* GetCommandEntityList(std::string commandStr);
	void RegisterCommand(int entityId, std::string commandStr);
	void RemoveCommand(int entityId, std::string commandStr);
	void EntityRegisterCommand(int entityId, std::list<std::string>& commandList);
	void EntityRemoveCommand(int entityId);

	void RegisterJob(int entityId, std::string commandStr);
	void RemoveJob(int entityId);
	//
	void Start(AEditEntityManage* view_);

	//
	void End();

	void SetOpenClosePlay(bool isPlay);
};
