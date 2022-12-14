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
?????????????????????

1.????????????????????????
2.??????????????????????????????
3.????????????????????????????????????
4.??????????????????????????????

*/

/*/////////////////////////////////////////////////////////////*/
// ??????
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
// ??????
/*/////////////////////////////////////////////////////////////*/

// ???????????????????????????????????????
std::list<int>* UEditEntityManageCommandComponent::GetCommandEntityList(std::string commandStr)
{

	auto it = entityCommandMap.find(commandStr);
	if (it != entityCommandMap.end())
	{
		return &it->second;
	}
	return nullptr;
}

// ??????????????????
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
			UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [RegisterCommand] ?????????????????? [entityId:%d] [commandStr:%s] "), this, entityId, *str);
		}
	}
}

// ??????????????????
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
			UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [RemoveCommand] ?????????????????? [entityId:%d] [commandStr:%s] "), this, entityId, *str);
		}
		if (it->second.size() == 0)
		{
			entityCommandMap.erase(it);
		}
	}
}

/*/////////////////////////////////////////////////////////////*/
// ?????????
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

// ??????????????????
void UEditEntityManageCommandComponent::EntityRegisterCommand(int entityId, std::list<std::string>& commandList)
{

	for (auto it = commandList.begin(); it != commandList.end(); ++it)
	{
		RegisterCommand(entityId, *it);
	}
	commandEntityMap.emplace(entityId, commandList);
	entityList.push_back(entityId);
}

// ??????????????????
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

// ??????????????????
void UEditEntityManageCommandComponent::RegisterJob(int entityId, std::string commandStr)
{
	auto entity = view->data->GetEntityActor(entityId);
	if (entity == nullptr)return;
	RemoveJob(entityId);
	entity->commandComponent->RegisterJob(commandStr);
	entityJobMap.emplace(entityId, entity);
}

// ??????????????????
void UEditEntityManageCommandComponent::RemoveJob(int entityId)
{

	auto it = entityJobMap.find(entityId);
	if (it != entityJobMap.end())
	{
		entityJobMap.erase(it);
	}
}

// ?????? ??????????????????
void UEditEntityManageCommandComponent::SetOpenClosePlay(bool isPlay)
{
	isTick = isPlay;

	// ?????????????????????????????????0
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

		// ?????????????????????>
		entityJobMap.clear();
	}
}

// ?????????????????????