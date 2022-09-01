// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityCommandComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Core/BaseGameInstance.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshView.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshComponent.h"
#include "../../Timeline/EditEntityManageTimelineComponent.h"
#include "EditEntityChildComponent.h"
#include "Yxjs/Tools/CalculationObject.h"
#include "../../../Selected/SelectedViewActor.h"
#include "../../../SceneStaticMesh/SceneStaticMeshView.h"

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
#include <Curves/CurveVector.h>
#include "../../Command/EditEntityManageCommandComponent.h"
#include "EditEntityTimelineComponent.h"

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/


/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
UEditEntityCommandComponent::UEditEntityCommandComponent()
{

}

// BeginPlay
void UEditEntityCommandComponent::Start(AEditEntityActor* view)
{
	owner_ = view;
	if (owner_->gameMode->sceneStaticMeshView == nullptr)return;
	editEntityManage = owner_->editEntityManage;

	RegisterToMaster();
}

// EndPlay
void UEditEntityCommandComponent::End()
{
	// 没有start却end将引发这个奔溃
	if (owner_.IsValid() == false || owner_->gameMode->sceneStaticMeshView == nullptr)return;

	RemoveFromMaster();
}

// 数据更新,将自身所有命令注册到命令总控中心,方便调取
void UEditEntityCommandComponent::RegisterToMaster()
{

	if (owner_->entityId == PreviewEntityID)return;

	auto& syncInfo = owner_->syncInfo;
	if (owner_->IsTimelineCommandText())
	{
		auto& command_list = syncInfo.command().command_list();
		std::list<std::string> commandList;
		for (auto it = command_list.begin(); it != command_list.end(); ++it)
		{
			commandList.push_back(it->command_name());
		}
		owner_->editEntityManage->command->EntityRegisterCommand(owner_->entityId, commandList);
	}
}

// 数据移除注册
void UEditEntityCommandComponent::RemoveFromMaster()
{

	if (owner_->entityId == PreviewEntityID)return;

	owner_->editEntityManage->command->EntityRemoveCommand(owner_->entityId);
}

// 数据更新注册
void UEditEntityCommandComponent::UpdateToMaster()
{

	if (owner_->entityId == PreviewEntityID)return;

	owner_->editEntityManage->command->RemoveJob(owner_->entityId);	// 移除在运行的任务
	RemoveFromMaster();
	RegisterToMaster();
}

// 更新,播放状态动画,像总控中心注册动画播放
void UEditEntityCommandComponent::PlayAnim(std::string command)
{
	// 向命令中心注册

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(owner_->GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;


	UE_LOG(LogTemp, Log, TEXT("[%x] [命令] [PlayAnim] 像总控中心注册动画播放 [playerId:%d] [entityId:%d] "), this, character->myData->playerId, owner_->entityId);


	owner_->editEntityManage->command->RegisterJob(owner_->entityId, command);
}

//-----------------------------------------------------------//

// 执行帧 单行实例执行帧
bool UEditEntityCommandComponent::EntityRunFrame(float nowTime)
{
	bool isEnd = true;

	auto timeline_start_time = commandJobInfo.entityCommandCommand.start_time();
	auto timeline_end_time = commandJobInfo.entityCommandCommand.end_time();

	if (timeline_start_time == timeline_end_time)return isEnd;

	float timeLenght = abs(timeline_end_time - timeline_start_time);

	float runTime = nowTime - commandJobInfo.startTime;

	float timelineRunTime = 0;

	// 正向播放
	if (timeline_start_time < timeline_end_time)
	{
		timelineRunTime = timeline_start_time + runTime;
	}
	// 反向播放
	else
	{
		timelineRunTime = timeline_start_time - runTime;
	}

	owner_->myTimelineComponent->PlayToTime(timelineRunTime, nowTime);

	if (runTime < timeLenght)
	{
		isEnd = false;
	}

	//UE_LOG(LogTemp, Log, TEXT("[%x] [命令控制] [EntityRunFrame] 执行帧 [entityId:%d] [timeline_start_time %f-%f] [runTime:%f/%f] [timelineRunTime:%f]"),
	//	this, commandJobInfo.entityId, timeline_start_time, timeline_end_time, runTime, timeLenght, timelineRunTime);

	return isEnd;
}

// 执行帧 实例注册任务
bool UEditEntityCommandComponent::RegisterJob(std::string commandStr)
{

	// 没有实例 退出
	auto& entity = owner_;

	// 没有数据 退出
	if (entity->syncInfo.has_command() == false || entity->syncInfo.command().type() != Gamedata::EntityCommandType::EntityCommandType_command)return false;

	// 没有命令数据 退出
	Gamedata::EntityCommandCommand entityCommandCommand;
	bool flag = false;
	auto& command_list = entity->syncInfo.command().command_list();
	for (auto it = command_list.begin(); it != command_list.end(); ++it)
	{
		if (it->command_name() == commandStr)
		{
			entityCommandCommand.CopyFrom(*it);
			flag = true;
			break;
		}
	}
	if (flag == false)return false;

	// 命令数据错误 退出
	if (entityCommandCommand.start_time() == entityCommandCommand.end_time())return false;

	auto nowTime = owner_->GetWorld()->TimeSeconds;
	commandJobInfo.startTime = nowTime;
	float lenght = entityCommandCommand.end_time() - entityCommandCommand.start_time();
	commandJobInfo.endTime = nowTime + lenght;
	commandJobInfo.entityId = owner_->entityId;
	commandJobInfo.entityCommandCommand.CopyFrom(entityCommandCommand);

	return true;
}
