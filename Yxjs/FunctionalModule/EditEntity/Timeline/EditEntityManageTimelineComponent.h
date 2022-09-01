// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include <string>
#include <memory>

/**线程*/
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
/**线程*/

#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "Tools/StringRingBuffer.h"
#include "EditEntityManageTimelineComponent.generated.h"

/**
 *
 */
class ABaseGameMode;
class AEditEntityManage;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UEditEntityManageData;
class AEditEntityActor;
class UBaseGameInstance;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UBoxComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UImage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UEditEntityManageTimelineComponent : public UActorComponent
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

		// 动态帧数
		int dynamicFrameCountReal = 0;

		// 当前秒钟
		int nowTimeSecond = 0;
	};
	TransformThreadInfo threadInfo;

	/*------------------------------------------------------------------*/

	/**子线程*/

	// 计算变换线程
	enum ThreadFlag
	{
		threadFlag_none,
		threadFlag_run,
		threadFlag_ready_end,
		threadFlag_real_end
	};
	ThreadFlag threadFlag = threadFlag_none;
	std::shared_ptr<std::thread>  transformThread;

	/*------------------------------------------------------------------*/

	/**变换计算线程数据*/

	// 时间轴tick key:entityId,value:actor
	TMap<int32, TWeakObjectPtr<AEditEntityActor>> timelineTickActorMap;

	/*------------------------------------------------------------------*/

	/**tick1*/

	// 环形缓冲区数据
	struct TempRingInfo
	{
		// tick执行时间
		float lastTimeTick = 0;

		// 环形缓冲区
		std::shared_ptr<StringRingBuffer> stringRingBuffer;

		// 环形缓冲区解析出来的数据
		std::list<Gamedata::TimelineOneSecondAllFrameData> ringInfoList;

		// 临时变量
		StringRingBuffer::RingInfo readRingIfo;
	};
	TempRingInfo tempRingInfo;

	/*------------------------------------------------------------------*/

	/**tick3*/

	// 对象队列
	struct TimelinneActorInfo
	{
		bool type = true; // true:新增 false:移除
		TWeakObjectPtr<AEditEntityActor> actor;
		int entityId = 0;
	};
	std::list<TimelinneActorInfo>timelineTickActorList;

	/*------------------------------------------------------------------*/

	/**tick2*/

	// 时间轴 1帧所有entity数据
	struct TimelineOnewFrameAllEntityData
	{
		std::map<int32, TimelineOneFrameOneEntityData> entity_transform_map;	// 1帧所有实例  key:entityId,value:transform
	};

	// 时间轴 1秒30帧数据
	struct TimelineOneSecondAllFrameData
	{
		int32 time = 1;		// 时间
		std::vector < TimelineOnewFrameAllEntityData> frame_list; // 数据 1秒30帧
	};

	std::list<TimelineOneSecondAllFrameData>timelineList;

	/*------------------------------------------------------------------*/

	/**当前方案*/

	// 当前方案 1号方案:多线程+环形缓冲区+protobuf 2号方案:直接tick 3号方案:多线程+list+结构体
	int programme = 2;

	// 是否启用tick
	bool isTick = false;

	/*------------------------------------------------------------------*/

	TWeakObjectPtr<UBaseGameInstance> gameInstance;

	//
	UEditEntityManageTimelineComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Tick1();
	void GetEntityData1(int time, int frameCount);
	void Tick2();
	void Tick3();
	void ThreadController(bool isStart);
	void WorkThread();
	void GetEntityData3(int time, int frameCount);

	//
	void Start(AEditEntityManage* view_);

	//
	void End();

	void SetOpenClosePlay(bool isPlay);
};
