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

////--------??????????????????--------//
//#include "Windows/AllowWindowsPlatformTypes.h"
//#include "Windows/PreWindowsApi.h"
//
//#include <windows.h>
//
//#include "Windows/PostWindowsApi.h"
//#include "Windows/HideWindowsPlatformTypes.h"
////--------??????????????????--------//

/*
??????????????????
*/

/*/////////////////////////////////////////////////////////////*/
// ??????
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


	// ????????????
	ThreadController(true);
}

// EndPlay
void UEditEntityManageTimelineComponent::End()
{
	tempRingInfo.stringRingBuffer = nullptr;
	// ????????????
	ThreadController(false);
}

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// ?????????
/*/////////////////////////////////////////////////////////////*/

// [????????????] [?????????+???????????????+protobuf] ????????????
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

// [????????????] [?????????+???????????????+protobuf] ????????????
void UEditEntityManageTimelineComponent::WorkThread()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [??????????????????] [WorkThread] ????????????"), this);

	// ????????????
	int waitTempTime = 50;
	float nowTime = 0;
	float nextTime = 0;
	auto world = GetWorld();

	threadFlag = ThreadFlag::threadFlag_run;
	while (threadFlag == threadFlag_run)
	{
		// ??????????????????
		nowTime = world->TimeSeconds;
		nextTime = (int)nowTime + 3.0f;
		// ?????????
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [??????????????????] [WorkThread]  [nowTime:%f] [nowTime:%d] "), this, nowTime, (int)nowTime);
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
		// ????????????
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
	UE_LOG(LogTemp, Log, TEXT("[%x] [??????????????????] [WorkThread] ????????????"), this);
	threadFlag = threadFlag_real_end;
}

// [????????????] [?????????+???????????????+protobuf] tick 1
void UEditEntityManageTimelineComponent::Tick1()
{
	float nowTime = GetWorld()->TimeSeconds;

	// 100????????????????????????
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
				UE_LOG(LogTemp, Log, TEXT("[%x] [UEditEntityManageTimelineComponent] ????????????????????? [nowTime:%f] [nowTimeSecond:%d] [dynamicFrameCount:%d] [readTime:%d] "),
					this, nowTime, threadInfo.nowTimeSecond, threadInfo.dynamicFrameCountReal, readTime);
			}
		}
	}

	// ??????????????????
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
					//str = str + FString::Printf(TEXT(",??????:%d"), it->time());
					tempRingInfo.ringInfoList.erase(it);
				}
				else if (it->time() == threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT(",????????????:%d"), it->time());
					break;
				}
				else if (it->time() > threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT("??????:%d"), it->time());
					break;
				}
				++it;
			}
			//UE_LOG(LogTemp, Log, TEXT("[%x] [UEditEntityManageTimelineComponent] ????????????????????? [nowTimeSecond:%d] [%s] "), this, threadInfo.nowTimeSecond, *str);
		}
	}

	// ??????????????????
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

			// ??????30???
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

// [????????????] [?????????+???????????????+protobuf] ???????????????????????????
void UEditEntityManageTimelineComponent::GetEntityData1(int time, int frameCount)
{
	Gamedata::TimelineOneSecondAllFrameData oneSecondAllFrameData;

	// ??????????????????????????????timelineTickActorMap
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

	// ??????????????????
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

	// ????????????????????????
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

// [????????????] [?????????+list+?????????] tick 3
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
					//str = str + FString::Printf(TEXT(",??????:%d"), it->time());
					timelineList.erase(it);
				}
				else if (it->time == threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT(",????????????:%d"), it->time());
					break;
				}
				else if (it->time > threadInfo.nowTimeSecond)
				{
					//str = str + FString::Printf(TEXT("??????:%d"), it->time());
					break;
				}
				++it;
			}
			//UE_LOG(LogTemp, Log, TEXT("[%x] [UEditEntityManageTimelineComponent] ????????????????????? [nowTimeSecond:%d] [%s] "), this, threadInfo.nowTimeSecond, *str);
		}
	}
	// ??????????????????
	{
		auto it = timelineList.begin();
		if ((it == timelineList.end()))return;
		//const ::Gamedata::TimelineOnewFrameAllEntityData& frame_info = it->frame_list[threadInfo.dynamicFrameCount];
		auto& frame_info = it->frame_list[threadInfo.dynamicFrameCount];

		// ??????30???
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

// [????????????] [?????????+list+?????????] ???????????????????????????
void UEditEntityManageTimelineComponent::GetEntityData3(int time, int frameCount)
{
	//Gamedata::TimelineOneSecondAllFrameData oneSecondAllFrameData;
	TimelineOneSecondAllFrameData oneSecondAllFrameData;

	// ??????????????????????????????timelineTickActorMap
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

	// ??????????????????
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

	//// ????????????????????????
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

// [??????tick] tick 2
void UEditEntityManageTimelineComponent::Tick2()
{
	float nowTime = GetWorld()->TimeSeconds;


	// ??????
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

// ?????? ??????????????????
void UEditEntityManageTimelineComponent::SetOpenClosePlay(bool isPlay)
{
	isTick = isPlay;

	// ?????????????????????????????????0
	if (isTick == false)
	{
		auto count = 0;
		for (auto node : timelineTickActorMap)
		{
			auto& actor = node.Value;
			if (actor.IsValid())
			{
				if (actor->isSelect)
				{// ???????????????
					actor->myTimelineComponent->MoveTimelineUI(true, 0);
				}
				else
				{
					actor->myTimelineComponent->TimelineAutoPlay(true, 0);// ???????????????
				}

				if (count > 2000)break;
				count++;
			}
		}
	}
}

// ?????????????????????