// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityManageTimelineComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Core/BaseGameInstance.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Tools/CommonlyObject.h"
#include "../Actor/EditEntityActor.h"
#include "../Actor/ControllerComponent/EditEntityTimelineComponent.h"

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include <Components/HierarchicalInstancedStaticMeshComponent.h>

////--------冲突的头文件--------//
//#include "Windows/AllowWindowsPlatformTypes.h"
//#include "Windows/PreWindowsApi.h"
//
//#include <windows.h>
//
//#include "Windows/PostWindowsApi.h"
//#include "Windows/HideWindowsPlatformTypes.h"
////--------冲突的头文件--------//

/*
时间轴控制器
*/

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

// UEditEntityManageTimelineComponent
UEditEntityManageTimelineComponent::UEditEntityManageTimelineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// BeginPlay
void UEditEntityManageTimelineComponent::Start(AEditEntityManage* view_)
{
	view = view_;

	tempRingInfo.stringRingBuffer = std::make_shared<StringRingBuffer>(100);

	gameInstance = Cast<UBaseGameInstance>(view_->GetGameInstance());


	// 启动线程
	ThreadController(true);
}

// EndPlay
void UEditEntityManageTimelineComponent::End()
{
	tempRingInfo.stringRingBuffer = nullptr;
	// 关闭线程
	ThreadController(false);
}

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

// [测试放弃] [多线程+环形缓冲区+protobuf] 启动线程
void UEditEntityManageTimelineComponent::ThreadController(bool isStart)
{
	if (programme == 3) {}
	else
	{
		return;
	}

	if (isStart)
	{
		if (transformThread == nullptr)
		{
			transformThread = std::make_shared<std::thread>(&UEditEntityManageTimelineComponent::WorkThread, this);
		}
	}
	else
	{
		if (transformThread != nullptr)
		{
			threadFlag = threadFlag_ready_end;
			while (true)
			{
				if (threadFlag == threadFlag_real_end)
				{
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			transformThread->detach();
			transformThread = nullptr;
		}
	}
}

// [测试放弃] [多线程+环形缓冲区+protobuf] 工作线程
void UEditEntityManageTimelineComponent::WorkThread()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例变换线程] [WorkThread] 线程启动"), this);

	// 等待时间
	int waitTempTime = 50;
	float nowTime = 0;
	float nextTime = 0;
	auto world = GetWorld();

	threadFlag = ThreadFlag::threadFlag_run;
	while (threadFlag == threadFlag_run)
	{
		// 本次等待时间
		nowTime = world->TimeSeconds;
		nextTime = (int)nowTime + 3.0f;
		// 工作帧
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [实例变换线程] [WorkThread]  [nowTime:%f] [nowTime:%d] "), this, nowTime, (int)nowTime);
			if (programme == 1)
			{
				GetEntityData1((int)nowTime + 1, 30);
				GetEntityData1((int)nowTime + 2, 30);
				GetEntityData1((int)nowTime + 3, 30);
			}
			else if (programme == 3)
			{
				GetEntityData3((int)nowTime + 1, 30);
				GetEntityData3((int)nowTime + 2, 30);
				GetEntityData3((int)nowTime + 3, 30);
			}
		}
		// 等待时间
		{
			while (world->TimeSeconds <= nextTime)
			{
				if (threadFlag == threadFlag_run)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(waitTempTime));
				}
				else
				{
					break;
				}
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例变换线程] [WorkThread] 线程结束"), this);
	threadFlag = threadFlag_real_end;
}

// [测试放弃] [多线程+环形缓冲区+protobuf] tick 1
void UEditEntityManageTimelineComponent::Tick1()
{
	float nowTime = GetWorld()->TimeSeconds;

	// 100毫秒读取一次数据
	{
		if (tempRingInfo.lastTimeTick + 0.1 < nowTime)
		{
			tempRingInfo.lastTimeTick = nowTime;
			int readTime = 0;
			while (tempRingInfo.stringRingBuffer->Read(tempRingInfo.readRingIfo))
			{
				Gamedata::TimelineOneSecondAllFrameData oneSecondAllFrameData;
				bool flag = oneSecondAllFrameData.ParseFromString(tempRingInfo.readRingIfo.str);
				if (flag)
				{
					tempRingInfo.ringInfoList.push_back(oneSecondAllFrameData);
					readTime++;
				}
			}
			if (readTime > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("[%x] [UEditEntityManageTimelineComponent] 主线程抓取数据 [nowTime:%f] [nowTimeSecond:%d] [dynamicFrameCount:%d] [readTime:%d] "),
					this, nowTime, threadInfo.nowTimeSecond, threadInfo.dynamicFrameCountReal, readTime);
			}
		}
	}

	// 锁定使用数据
	{
		//
		if (threadInfo.nowTimeSecond != (int)nowTime)
		{
			threadInfo.nowTimeSecond = nowTime;
			threadInfo.dynamicFrameCountReal = threadInfo.dynamicFrameCount;
			threadInfo.dynamicFrameCount = 0;

			FString str = TEXT("");
			while (true)
			{
				auto it = tempRingInfo.ringInfoList.begin();
				if ((it == tempRingInfo.ringInfoList.end()))
				{
					break;
				}
				else if (it->time() < threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT(",移除:%d"), it->time());
					tempRingInfo.ringInfoList.erase(it);
				}
				else if (it->time() == threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT(",设置当前:%d"), it->time());
					break;
				}
				else if (it->time() > threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT("错误:%d"), it->time());
					break;
				}
				++it;
			}
			//UE_LOG(LogTemp, Log, TEXT("[%x] [UEditEntityManageTimelineComponent] 设置当前帧数据 [nowTimeSecond:%d] [%s] "), this, threadInfo.nowTimeSecond, *str);
		}
	}

	// 正式使用数据
	{
		bool isRun = true;
		auto it = tempRingInfo.ringInfoList.begin();
		if ((it == tempRingInfo.ringInfoList.end()))
		{
			isRun = false;
		}

		if (it->frame_list_size() <= threadInfo.dynamicFrameCountReal)
		{
			isRun = false;
		}

		if (isRun)
		{
			const ::Gamedata::TimelineOnewFrameAllEntityData& frame_info = it->frame_list(threadInfo.dynamicFrameCount);

			// 锁住30帧
			if (it->time() == threadInfo.nowTimeSecond)
			{
				auto count = 0;
				if (threadInfo.lastTimeTick + threadInfo.waitTime < nowTime)
				{
					threadInfo.lastTimeTick = nowTime;

					if (isRun)
					{
						for (auto node : timelineTickActorMap)
						{
							auto& actor = node.Value;
							if (actor.IsValid())
							{
								//auto entity_transform = frame_info.entity_transform_map().find(actor->entityId);
								//if (entity_transform != frame_info.entity_transform_map().end()) {
								//	auto& oneFrameOneEntity = entity_transform->second;
								//	actor->myTimelineComponent->ComponenntTick(oneFrameOneEntity.child_transform());
								//}
								//if (count > 2000)break;
								count++;
							}
						}

						threadInfo.dynamicFrameCount++;
					}
				}
			}
		}
	}
}

// [测试放弃] [多线程+环形缓冲区+protobuf] 遍历所有循环的实例
void UEditEntityManageTimelineComponent::GetEntityData1(int time, int frameCount)
{
	Gamedata::TimelineOneSecondAllFrameData oneSecondAllFrameData;

	// 从队列中拉取数据操作timelineTickActorMap
	{
		if (timelineTickActorList.size() > 0)
		{
			while (true)
			{
				auto it = timelineTickActorList.begin();
				auto& node = *it;
				if (node.type)
				{
					if (timelineTickActorMap.Find(node.entityId))
					{
						timelineTickActorMap.Remove(node.entityId);
					}
					timelineTickActorMap.Add(node.entityId, node.actor);
				}
				else
				{
					if (timelineTickActorMap.Find(node.entityId))
					{
						timelineTickActorMap.Remove(node.entityId);
					}
				}
				timelineTickActorList.erase(it);
				if (timelineTickActorList.size() == 0)
				{
					break;
				}
			}
		}
	}

	// 遍历该帧数据
	{
		for (auto i = 0; i < frameCount; i++)
		{
			oneSecondAllFrameData.add_frame_list();
		}
		oneSecondAllFrameData.set_time(time);

		int counnt = 0;
		float thisTime = 0;
		for (auto node : timelineTickActorMap)
		{
			int entityId = node.Key;
			auto& actor = node.Value;
			if (actor.IsValid())
			{
				for (auto i = 0; i < frameCount; i++)
				{
					thisTime = time + i * threadInfo.waitTime;
					Gamedata::TimelineOneFrameOneEntityData oneFrameOneEntityData;
					//actor->myTimelineComponent->GetChildTimelineTransform(thisTime, oneFrameOneEntityData);

					Gamedata::TimelineOnewFrameAllEntityData* onewFrameAllEntityData = oneSecondAllFrameData.mutable_frame_list(i);
					onewFrameAllEntityData->mutable_entity_transform_map()->insert({ entityId ,oneFrameOneEntityData });
				}
			}
			counnt++;
			if (counnt > 2000)break;
		}
	}

	// 发送给环形缓冲区
	{
		StringRingBuffer::RingInfo ringIfo;
		ringIfo.str = oneSecondAllFrameData.SerializePartialAsString();
		if (tempRingInfo.stringRingBuffer)
		{
			tempRingInfo.stringRingBuffer->Write(ringIfo);
		}
	}
}

/*----------------------------------------------------*/

// [测试放弃] [多线程+list+结构体] tick 3
void UEditEntityManageTimelineComponent::Tick3()
{
	float nowTime = GetWorld()->TimeSeconds;

	{
		//
		if (threadInfo.nowTimeSecond != (int)nowTime)
		{
			threadInfo.nowTimeSecond = nowTime;
			threadInfo.dynamicFrameCountReal = threadInfo.dynamicFrameCount;
			threadInfo.dynamicFrameCount = 0;

			FString str = TEXT("");
			while (true)
			{
				auto it = timelineList.begin();
				if ((it == timelineList.end()))
				{
					break;
				}
				else if (it->time < threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT(",移除:%d"), it->time());
					timelineList.erase(it);
				}
				else if (it->time == threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT(",设置当前:%d"), it->time());
					break;
				}
				else if (it->time > threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT("错误:%d"), it->time());
					break;
				}
				++it;
			}
			//UE_LOG(LogTemp, Log, TEXT("[%x] [UEditEntityManageTimelineComponent] 设置当前帧数据 [nowTimeSecond:%d] [%s] "), this, threadInfo.nowTimeSecond, *str);
		}
	}
	// 正式使用数据
	{
		auto it = timelineList.begin();
		if ((it == timelineList.end()))return;
		//const ::Gamedata::TimelineOnewFrameAllEntityData& frame_info = it->frame_list[threadInfo.dynamicFrameCount];
		auto& frame_info = it->frame_list[threadInfo.dynamicFrameCount];

		// 锁住30帧
		if (it->time == threadInfo.nowTimeSecond)
		{
			auto count = 0;
			if (threadInfo.lastTimeTick + threadInfo.waitTime < nowTime)
			{
				threadInfo.lastTimeTick = nowTime;

				for (auto node : frame_info.entity_transform_map)
				{
					node.second.actor->myTimelineComponent->ComponenntTick3(node.second.child_transform);
					//if (count > 2000)break;
					count++;
				}

				threadInfo.dynamicFrameCount++;
			}
		}
	}
}

// [测试放弃] [多线程+list+结构体] 遍历所有循环的实例
void UEditEntityManageTimelineComponent::GetEntityData3(int time, int frameCount)
{
	//Gamedata::TimelineOneSecondAllFrameData oneSecondAllFrameData;
	TimelineOneSecondAllFrameData oneSecondAllFrameData;

	// 从队列中拉取数据操作timelineTickActorMap
	{
		if (timelineTickActorList.size() > 0)
		{
			while (true)
			{
				auto it = timelineTickActorList.begin();
				auto& node = *it;
				if (node.type)
				{
					if (timelineTickActorMap.Find(node.entityId))
					{
						timelineTickActorMap.Remove(node.entityId);
					}
					timelineTickActorMap.Add(node.entityId, node.actor);
				}
				else
				{
					if (timelineTickActorMap.Find(node.entityId))
					{
						timelineTickActorMap.Remove(node.entityId);
					}
				}
				timelineTickActorList.erase(it);
				if (timelineTickActorList.size() == 0)
				{
					break;
				}
			}
		}
	}

	// 遍历该帧数据
	{
		for (auto i = 0; i < frameCount; i++)
		{
			//oneSecondAllFrameData.add_frame_list();
			TimelineOnewFrameAllEntityData timelineOnewFrameAllEntityData;
			oneSecondAllFrameData.frame_list.push_back(timelineOnewFrameAllEntityData);
		}
		//oneSecondAllFrameData.set_time(time);
		oneSecondAllFrameData.time = time;

		int counnt = 0;
		float thisTime = 0;
		for (auto node : timelineTickActorMap)
		{
			int entityId = node.Key;
			auto& actor = node.Value;
			if (actor.IsValid())
			{
				for (auto i = 0; i < frameCount; i++)
				{
					thisTime = time + i * threadInfo.waitTime;
					//Gamedata::TimelineOneFrameOneEntityData oneFrameOneEntityData;
					TimelineOneFrameOneEntityData oneFrameOneEntityData;
					actor->myTimelineComponent->GetChildTimelineTransform(thisTime, oneFrameOneEntityData);

					//Gamedata::TimelineOnewFrameAllEntityData* onewFrameAllEntityData = oneSecondAllFrameData.mutable_frame_list(i);
					//onewFrameAllEntityData->mutable_entity_transform_map()->insert({ entityId ,oneFrameOneEntityData });

					oneFrameOneEntityData.actor = actor;
					oneSecondAllFrameData.frame_list[i].entity_transform_map.emplace(entityId, oneFrameOneEntityData);
				}
			}
			counnt++;
			if (counnt > 2000)break;
		}
		timelineList.push_back(oneSecondAllFrameData);
	}

	//// 发送给环形缓冲区
	//{
	//	StringRingBuffer::RingInfo ringIfo;
	//	ringIfo.str = oneSecondAllFrameData.SerializePartialAsString();
	//	if (tempRingInfo.stringRingBuffer) {
	//		tempRingInfo.stringRingBuffer->Write(ringIfo);
	//	}
	//}
}

/*----------------------------------------------------*/

// Called every frame
void UEditEntityManageTimelineComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isTick == false)return;

	if (programme == 1)
	{
	}
	else if (programme == 2)
	{
		Tick2();
	}
	else if (programme == 3)
	{
		Tick3();
	}
}

// [直接tick] tick 2
void UEditEntityManageTimelineComponent::Tick2()
{
	float nowTime = GetWorld()->TimeSeconds;


	// 等待
	if (gameInstance.IsValid() && gameInstance->lockFrame == CvMySettingType::SetFrames_999)
	{
		threadInfo.waitTime = 1.0f / 60.0f;

		if (nowTime > threadInfo.lastTimeTick + threadInfo.waitTime)
		{
			threadInfo.lastTimeTick = nowTime;
		}
		else
		{
			return;
		}
	}

	auto count = 0;
	for (auto node : timelineTickActorMap)
	{
		auto& actor = node.Value;
		if (actor.IsValid())
		{
			actor->myTimelineComponent->TimelineAutoPlay(false, nowTime);
			if (count > 2000)break;
			count++;
		}
	}
	threadInfo.dynamicFrameCount++;

}

// 事件 开启关闭播放
void UEditEntityManageTimelineComponent::SetOpenClosePlay(bool isPlay)
{
	isTick = isPlay;

	// 将已经运动的位置全部归0
	if (isTick == false)
	{
		auto count = 0;
		for (auto node : timelineTickActorMap)
		{
			auto& actor = node.Value;
			if (actor.IsValid())
			{
				if (actor->isSelect)
				{// 带有坐标轴
					actor->myTimelineComponent->MoveTimelineUI(true, 0);
				}
				else
				{
					actor->myTimelineComponent->TimelineAutoPlay(true, 0);// 没带坐标轴
				}

				if (count > 2000)break;
				count++;
			}
		}
	}
}

// 指定播放到某帧