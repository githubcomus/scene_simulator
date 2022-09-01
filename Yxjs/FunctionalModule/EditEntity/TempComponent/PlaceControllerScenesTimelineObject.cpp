// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceControllerScenesTimelineObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerScenesTimelineObject.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewLaserLightActor.h"
#include "Core/BaseGameInstance.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Tools/CommonlyObject.h"
#include "PlaceViewLaserBeamComponent.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Tools/CalculationObject.h"
#include "Net/NetMainComponent.h"

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

//#include<vector>

/*
时间轴的控制器组件
*/


/*------------------------------------------------------------------*/

// UPlaceControllerScenesTimelineObject
UPlaceControllerScenesTimelineObject::UPlaceControllerScenesTimelineObject()
{
}

//
void UPlaceControllerScenesTimelineObject::TickComponent(float DeltaTime)
{
	// Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// auto nowTime = controller->GetWorld()->TimeSeconds;
	// if (nowTime >= (tickInfo.lastTime + 0.1))
	// {
	// 	tickInfo.lastTime = nowTime;
	// }

	// 测试
	{
		auto nowTime = controller->GetWorld()->TimeSeconds;
		if (tickInfo.lastTimeTestFunc == 0)
		{
			tickInfo.lastTimeTestFunc = nowTime;
		}
		if (nowTime >= (tickInfo.lastTimeTestFunc + 5) && tickInfo.lastTimeTestFunc != -1)
		{
			tickInfo.lastTimeTestFunc = -1;
			TestFunc();
		}
	}

	// 控制最大帧数(1/0.05=20)
	{
		auto nowTime = controller->GetWorld()->TimeSeconds;
		if (nowTime >= (tickInfo.lastTime + 0.1))
		{
			tickInfo.lastTime = nowTime;
			TickMove();
		}
	}
}

// BeginPlay
void UPlaceControllerScenesTimelineObject::Start(TWeakObjectPtr<UEditEntityManageController> controller_)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerScenesTimelineObject::Start]   "), this);
	controller = controller_;
}

// EndPlay
void UPlaceControllerScenesTimelineObject::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerScenesTimelineObject::End]   "), this);
}

/*------------------------------------------------------------------*/

// 获取 sync数据更变local数据
void UPlaceControllerScenesTimelineObject::MackFLocalEntityInfo(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	auto data = controller->data;

	//auto xlsInfo = data->GetXlsInfoWithXlsId(syncInfo->xlsid());
	//if (xlsInfo == nullptr)return;
	//ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(controller->GetWorld()));

	//if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_laser)
	//{
	//}
}

/*------------------------------------------------------------------*/

// tick
void UPlaceControllerScenesTimelineObject::TickMove()
{
	auto nowTime = controller->GetWorld()->TimeSeconds;
	auto view = controller->view;

	// 统计执行次数
	// {
	// 	if (nowTime >= (tickInfo.countTime + 1))
	// 	{
	// 		tickInfo.countTime = nowTime;
	// 		UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerScenesTimelineObject::TickMove] [time:%f] [count:%d]"), this, tickInfo.countTime, tickInfo.functionCount);
	// 		tickInfo.functionCount=0;
	// 	}
	// 	tickInfo.functionCount++;
	// }


	// 遍历所有灯光
	// for (TPair<int32, Gamedata::EntityInfo>& node : view->data->syncEntityInfoMap)
	// {
	// 	auto syncInfo = &node.Value;
	// 	auto xlsInfo = view->data->GetXlsInfo(syncInfo->entityid());
	// 	if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_laser)
	// 	// if (syncInfo->type() == Gamedata::EntityType::EntityType_Light)
	// 	{
	// 		auto localInfo = view->data->GetLocalInfo(node.Key);
	// 		if (localInfo)
	// 		{
	// 			LocalPreviewMode(syncInfo, localInfo);
	// 		}
	// 	}
	// }
	// 找到时间控制器

	// 找到启动时间,当前时间
}

// 时间轴控制器
void UPlaceControllerScenesTimelineObject::TimelineController()
{
}

/*------------------------------------------------------------------*/

// 网络请求 同步操作
void UPlaceControllerScenesTimelineObject::NetCall_EntityOperation(Gamedata::EntityOperationTotal& entityOperation)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(controller->GetWorld()->GetAuthGameMode());
	auto gameInstance = baseGameMode->gameInstance;

	//char* sendData = (char*)malloc(1024 * 10);
	//int sendSize = 0;
	//int type;
	//controller->data->MakePB_EntityOperation(entityOperation, sendData, 1024 * 10, sendSize, type);
	//gameInstance->netMainComponent->NetCall_GameData(type, sendData, sendSize);
	//free(sendData);

	Gamedata::EntityRemoveRequest request;

	request.set_playerid(controller->data->GetLocalPlayerId());

	std::string sendStr = request.SerializePartialAsString();
	char* sendData = (char*)sendStr.c_str();
	int sendSize = sendStr.length();
	int type = (int)Gamedata::MsgType::PB_EntityOperation;
	gameInstance->netMainComponent->NetCall_GameData(type, sendData, sendSize);
}

// 网络回调 同步操作
void UPlaceControllerScenesTimelineObject::NetCallback_EntityOperation(Gamedata::EntityOperationTotal& total)
{
	for (int i = 0; i < total.node_size(); i++)
	{
		auto node = total.node(i);
		auto entityId = node.entityid();
		if (controller->data->IsExistData(entityId) == false)
		{
			continue;
		}

		Function_SetPlay(&node);
	}
}

/*------------------------------------------------------------------*/

// 业务 操作播放
void UPlaceControllerScenesTimelineObject::Function_SetPlay(Gamedata::EntityOperationAlone* node)
{
	//ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(controller->GetWorld()->GetAuthGameMode());
	//if (baseGameMode == nullptr)return;

	//auto entityId = node->entityid();
	//if (controller->data->IsExistData(entityId) == false)
	//{
	//	return;
	//}
	//if (node->thetype() != Gamedata::EntityOperationType_IsPlay)
	//{
	//	return;
	//}
	//if (node->value1_size() == 0)
	//{
	//	return;
	//}
	//auto value1 = node->value1(0);
	//bool isPlay = value1.isplay();

	//auto syncInfo = controller->data->GetSyncInfo(entityId);

	////if (syncInfo->scenestimeline_size() == 0)
	////{
	////	syncInfo->add_scenestimeline();
	////}
	//auto scenestimeline = syncInfo->scenestimeline();
	//// if (timeline.alone_size() == 0)
	//// {
	//// 	timeline.add_alone();
	//// }
	//// auto alone = timeline.alone(0);

	//scenestimeline.set_isplay(isPlay);
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerScenesTimelineObject::Function_SetPlay] [loacalPlayerId:%d] [time:%f] [entityId:%d] [isPlay:%d]"),
	//	this, baseGameMode->GetLocalPlayerId(), controller->GetWorld()->TimeSeconds, entityId, isPlay);
}

/*------------------------------------------------------------------*/

// 测试
void UPlaceControllerScenesTimelineObject::TestFunc()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerScenesTimelineObject::TestFunc] "), this);

	// SetPreviewData(319,0,1,0);
}

/*------------------------------------------------------------------*/
