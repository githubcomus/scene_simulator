// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityManageCommandComponent.h"
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


/*
命令控制器总控

1.玩家操纵播放词条
2.词条实例广播状态更变
3.实例命令组件注册动画播放
4.实例总控中心播放动画

*/

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

// UEditEntityManageCommandComponent
UEditEntityManageCommandComponent::UEditEntityManageCommandComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// BeginPlay
void UEditEntityManageCommandComponent::Start(AEditEntityManage* view_)
{
	view = view_;

}

// EndPlay
void UEditEntityManageCommandComponent::End()
{
}

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

// 返回指定命令的支持实例列表
std::list<int>* UEditEntityManageCommandComponent::GetCommandEntityList(std::string commandStr)
{

	auto it = entityCommandMap.find(commandStr);
	if (it != entityCommandMap.end())
	{
		return &it->second;
	}
	return nullptr;
}

// 注册单行命令
void UEditEntityManageCommandComponent::RegisterCommand(int entityId, std::string commandStr)
{

	if (commandStr == "")return;
	auto it = entityCommandMap.find(commandStr);
	if (it == entityCommandMap.end())
	{
		std::list<int> value;
		entityCommandMap.emplace(commandStr, value);
		it = entityCommandMap.find(commandStr);
	}
	if (it != entityCommandMap.end())
	{
		auto it2 = std::find(it->second.begin(), it->second.end(), entityId);
		if (it2 == it->second.end())
		{
			it->second.push_back(entityId);
			FString str = UTF8_TO_TCHAR(commandStr.c_str());
			UE_LOG(LogTemp, Log, TEXT("[%x] [命令] [RegisterCommand] 注册单行命令 [entityId:%d] [commandStr:%s] "), this, entityId, *str);
		}
	}
}

// 移除单行命令
void UEditEntityManageCommandComponent::RemoveCommand(int entityId, std::string commandStr)
{

	if (commandStr == "")return;
	auto it = entityCommandMap.find(commandStr);
	if (it != entityCommandMap.end())
	{
		auto it2 = std::find(it->second.begin(), it->second.end(), entityId);
		if (it2 != it->second.end())
		{
			it->second.erase(it2);
			FString str = UTF8_TO_TCHAR(commandStr.c_str());
			UE_LOG(LogTemp, Log, TEXT("[%x] [命令] [RemoveCommand] 移除单行命令 [entityId:%d] [commandStr:%s] "), this, entityId, *str);
		}
		if (it->second.size() == 0)
		{
			entityCommandMap.erase(it);
		}
	}
}

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

// Called every frame
void UEditEntityManageCommandComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isTick == false)return;

	if (entityJobMap.size() == 0)return;

	float nowTime = GetWorld()->TimeSeconds;

	auto count = 0;
	if (threadInfo.lastTimeTick + threadInfo.waitTime < nowTime)
	{
		threadInfo.lastTimeTick = nowTime;

		// tick
		for (auto node : entityJobMap)
		{
			TWeakObjectPtr<AEditEntityActor>& entity = node.second;
			bool flag = entity->commandComponent->EntityRunFrame(nowTime);
			if (flag)
			{
				deleteEntityList.push_back(node.first);
			}
			if (count > 1000)break;
			count++;
		}


		// tick
		if (deleteEntityList.size() > 0)
		{
			for (auto node : deleteEntityList)
			{
				RemoveJob(node);
			}
			deleteEntityList.clear();
		}

		threadInfo.dynamicFrameCount++;
	}

}

// 实例注册命令
void UEditEntityManageCommandComponent::EntityRegisterCommand(int entityId, std::list<std::string>& commandList)
{

	for (auto it = commandList.begin(); it != commandList.end(); ++it)
	{
		RegisterCommand(entityId, *it);
	}
	commandEntityMap.emplace(entityId, commandList);
	entityList.push_back(entityId);
}

// 实例移除命令
void UEditEntityManageCommandComponent::EntityRemoveCommand(int entityId)
{

	auto it = commandEntityMap.find(entityId);
	if (it != commandEntityMap.end())
	{
		std::list<std::string>& commandList = it->second;

		for (auto it2 = commandList.begin(); it2 != commandList.end(); ++it2)
		{
			RemoveCommand(entityId, *it2);
		}
		entityList.remove(entityId);

		commandEntityMap.erase(it);
	}
}

// 实例注册任务
void UEditEntityManageCommandComponent::RegisterJob(int entityId, std::string commandStr)
{
	auto entity = view->data->GetEntityActor(entityId);
	if (entity == nullptr)return;
	RemoveJob(entityId);
	entity->commandComponent->RegisterJob(commandStr);
	entityJobMap.emplace(entityId, entity);
}

// 实例移除任务
void UEditEntityManageCommandComponent::RemoveJob(int entityId)
{

	auto it = entityJobMap.find(entityId);
	if (it != entityJobMap.end())
	{
		entityJobMap.erase(it);
	}
}

// 事件 开启关闭播放
void UEditEntityManageCommandComponent::SetOpenClosePlay(bool isPlay)
{
	isTick = isPlay;

	// 将已经运动的位置全部归0
	if (isTick == false)
	{
		auto count = 0;
		for (auto node : entityList)
		{
			auto actor = view->data->GetEntityActor(node);
			//auto& actor = node.second;
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

		// 移除所有的任务>
		entityJobMap.clear();
	}
}

// 指定播放到某帧