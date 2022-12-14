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
// ??????
/*/////////////////////////////////////////////////////////////*/


/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// ?????????
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
	// ??????start???end?????????????????????
	if (owner_.IsValid() == false || owner_->gameMode->sceneStaticMeshView == nullptr)return;

	RemoveFromMaster();
}

// ????????????,????????????????????????????????????????????????,????????????
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

// ??????????????????
void UEditEntityCommandComponent::RemoveFromMaster()
{

	if (owner_->entityId == PreviewEntityID)return;

	owner_->editEntityManage->command->EntityRemoveCommand(owner_->entityId);
}

// ??????????????????
void UEditEntityCommandComponent::UpdateToMaster()
{

	if (owner_->entityId == PreviewEntityID)return;

	owner_->editEntityManage->command->RemoveJob(owner_->entityId);	// ????????????????????????
	RemoveFromMaster();
	RegisterToMaster();
}

// ??????,??????????????????,?????????????????????????????????
void UEditEntityCommandComponent::PlayAnim(std::string command)
{
	// ?????????????????????

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(owner_->GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;


	UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [PlayAnim] ????????????????????????????????? [playerId:%d] [entityId:%d] "), this, character->myData->playerId, owner_->entityId);


	owner_->editEntityManage->command->RegisterJob(owner_->entityId, command);
}

//-----------------------------------------------------------//

// ????????? ?????????????????????
bool UEditEntityCommandComponent::EntityRunFrame(float nowTime)
{
	bool isEnd = true;

	auto timeline_start_time = commandJobInfo.entityCommandCommand.start_time();
	auto timeline_end_time = commandJobInfo.entityCommandCommand.end_time();

	if (timeline_start_time == timeline_end_time)return isEnd;

	float timeLenght = abs(timeline_end_time - timeline_start_time);

	float runTime = nowTime - commandJobInfo.startTime;

	float timelineRunTime = 0;

	// ????????????
	if (timeline_start_time < timeline_end_time)
	{
		timelineRunTime = timeline_start_time + runTime;
	}
	// ????????????
	else
	{
		timelineRunTime = timeline_start_time - runTime;
	}

	owner_->myTimelineComponent->PlayToTime(timelineRunTime, nowTime);

	if (runTime < timeLenght)
	{
		isEnd = false;
	}

	//UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [EntityRunFrame] ????????? [entityId:%d] [timeline_start_time %f-%f] [runTime:%f/%f] [timelineRunTime:%f]"),
	//	this, commandJobInfo.entityId, timeline_start_time, timeline_end_time, runTime, timeLenght, timelineRunTime);

	return isEnd;
}

// ????????? ??????????????????
bool UEditEntityCommandComponent::RegisterJob(std::string commandStr)
{

	// ???????????? ??????
	auto& entity = owner_;

	// ???????????? ??????
	if (entity->syncInfo.has_command() == false || entity->syncInfo.command().type() != Gamedata::EntityCommandType::EntityCommandType_command)return false;

	// ?????????????????? ??????
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

	// ?????????????????? ??????
	if (entityCommandCommand.start_time() == entityCommandCommand.end_time())return false;

	auto nowTime = owner_->GetWorld()->TimeSeconds;
	commandJobInfo.startTime = nowTime;
	float lenght = entityCommandCommand.end_time() - entityCommandCommand.start_time();
	commandJobInfo.endTime = nowTime + lenght;
	commandJobInfo.entityId = owner_->entityId;
	commandJobInfo.entityCommandCommand.CopyFrom(entityCommandCommand);

	return true;
}
