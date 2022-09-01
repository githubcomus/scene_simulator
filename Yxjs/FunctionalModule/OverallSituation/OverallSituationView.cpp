// Fill out your copyright notice in the Description page of Project Settings.

#include "OverallSituationView.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Yxjs/Tools/CommonlyObject.h"

#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "UI/BaseHud.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
//#include <WeakObjectPtrTemplates.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include "Yxjs/Pawn/Character/BaseCharacter.h"
#include "Yxjs/Pawn/Character/CharacterComponent/PlayerEntityMergeComponent.h"

// 功能模块
// view
/*
 *
 */

 /*----------------------------------------------------*/
 /*----------------------------------------------------*/

// 检查必须的数据
bool AOverallSituationView::CheckTargetData()
{
	bool checkTwice = false;

	if (skyActor.IsValid() == false || weatherActor.IsValid() == false || skyControllerActor.IsValid() == false)
	{
		TArray<AActor*>actorList;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actorList);
		for (auto node : actorList)
		{
			if (node->GetName().StartsWith(TEXT("Ultra_Dynamic_Sky")))
			{
				skyActor = node;
			}
			else if (node->GetName().StartsWith(TEXT("Ultra_Dynamic_Weather")))
			{
				weatherActor = node;
			}
		}

		auto gameMode_ = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gameMode_)
		{
			skyControllerActor = gameMode_->skyControllerActor;
		}

		checkTwice = true;
	}

	bool flag = true;
	if (checkTwice)
	{
		if (skyActor.IsValid() == false || weatherActor.IsValid() == false || skyControllerActor.IsValid() == false)
		{
			flag = false;
		}
	}

	return flag;
}

// 检查必须的数据
bool AOverallSituationView::CheckDefaultData()
{
	bool checkTwice = false;
	if (gameInstance.IsValid() == false || gameMode.IsValid() == false
		|| playerController.IsValid() == false || hud.IsValid() == false)
	{
		gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController.IsValid())hud = Cast<ABaseHUD>(playerController->GetHUD());

		checkTwice = true;
	}

	bool flag = true;
	if (checkTwice)
	{
		if (gameInstance.IsValid() == false || gameMode.IsValid() == false
			|| playerController.IsValid() == false || hud.IsValid() == false)
		{
			flag = false;
		}
	}

	return flag;
}

// 获取 当前放置系统视图
TWeakObjectPtr< AEditEntityManage> AOverallSituationView::GetEditEntityManage()
{
	if (gameMode.IsValid())
	{
		return gameMode->editEntityManage;
	}
	return nullptr;
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

//
AOverallSituationView::AOverallSituationView()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;
}

//
void AOverallSituationView::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void AOverallSituationView::BeginPlay()
{
	Super::BeginPlay();
	Start();
}

//
void AOverallSituationView::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	End();
	Super::EndPlay(EndPlayReason);
}

//
void AOverallSituationView::Start()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//baseGameMode->OverallSituationView = this;

	// 找到天气actor
	CheckTargetData();

	CheckDefaultData();

	//if (skyActor.IsValid() == false || weatherActor.IsValid() == false) {
	//	UE_LOG(LogTemp, Log, TEXT("[%x] [全局数据] [AOverallSituationView] err [skyActor:%x] [weatherActor:%x]"), this,skyActor.Get(), weatherActor.Get());
	//}

	//NetCall_ChangeSkySet();
}

//
void AOverallSituationView::End()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//baseGameMode->OverallSituationView = nullptr;
}

// tick
void AOverallSituationView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (createCount == 0 && destroyCount == 0)
	//{
	//	return;
	//}

	//int count = 0;
	//for (auto it = componentMap.CreateConstIterator(); it; ++it)
	//{
	//	FString key = it.Key();
	//	FSceneComponentInfo& node = componentMap[key];
	//	if (node.isLoadRes != ESceneResLoadType::ResLoadType_None)
	//	{
	//		if (createCount > 0 && node.createInstanceList.size() > 0)
	//		{
	//			Tick_CreateInstance(count, node);
	//		}
	//		if (destroyCount > 0 && node.destroyInstanceList.size() > 0)
	//		{
	//			Tick_DestroyInstance(count, node);
	//		}
	//	}
	//}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 网络请求 天气更变
void AOverallSituationView::NetCall_ChangeSkySet(int time, int weather, int cloud_coverageoverall, int cloud_speed, int moon_color)
{
	if (CheckDefaultData() == false || CheckTargetData() == false)return;

	{
		Gamedata::OverallSituationDataRequest overallSituationDataRequest;
		overallSituationDataRequest.set_set_type(Gamedata::OverallSituationDataType_req_sky);
		overallSituationDataRequest.mutable_sky_info()->set_time(time);
		overallSituationDataRequest.mutable_sky_info()->set_weather(weather);
		overallSituationDataRequest.mutable_sky_info()->set_cloud_coverageoverall(cloud_coverageoverall);
		overallSituationDataRequest.mutable_sky_info()->set_cloud_speed(cloud_speed);
		overallSituationDataRequest.mutable_sky_info()->set_moon_color(moon_color);

		gameMode->controller->NetCall_GameData(EMsgType::EMsgType_OverallSituationDataRequest, &overallSituationDataRequest);

		//overallSituationDataRequest.ByteSize();
		//UE_LOG(LogTemp, Log, TEXT("[%x] [全局数据] [NetCall_ChangeSkySet]  [size:%d] "), this, overallSituationDataRequest.ByteSize());
	}

	{
		//Gamedata::OverallSituationDataRequest overallSituationDataRequest;

		//auto skyInfo2 = overallSituationDataRequest.mutable_sky_info2();
		//skyInfo2->set_time(1200);
		//skyInfo2->set_cloud_coverageoverall(3);
		//skyInfo2->set_cloud_speed(15);
		//skyInfo2->set_time(1200);

		//auto skyInfo3 = overallSituationDataRequest.mutable_sky_info3();
		//skyInfo3->set_time(1200);
		//skyInfo3->set_cloud_coverageoverall(3);
		//skyInfo3->set_cloud_speed(15);
		//skyInfo3->set_time(1200);

		//UE_LOG(LogTemp, Log, TEXT("[%x] [全局数据] [NetCall_ChangeSkySet]  [size:%d] "), this, overallSituationDataRequest.ByteSize());
	}
}

// 网络响应 获取所有角色
void AOverallSituationView::NetCall_Place_Merge_GetAllActor()
{
}

// 网络响应 新增角色
void AOverallSituationView::NetCall_Place_Merge_AddActor(std::vector<Gamedata::EntityInfo*>nodeEntityIdList)
{
	//auto placeView = GetEditEntityManage();
	//if (placeView == nullptr)return;
	//auto placeData = placeView->data;
	//if (placeData == nullptr)return;
	auto localPlayerId = gameMode->GetLocalPlayerId();

	if (nodeEntityIdList.size() == 0)return;

	Gamedata::OverallSituationDataRequest overallSituationDataRequest;
	overallSituationDataRequest.set_set_type(Gamedata::OverallSituationDataType_req_add_actor);
	auto actorInfo = overallSituationDataRequest.mutable_actor_add();

	auto masterSyncInfo = nodeEntityIdList[0];
	actorInfo->set_player_id(localPlayerId);
	actorInfo->set_xls_id(masterSyncInfo->entityid());

	overallSituationDataRequest.mutable_actor_add_transform()->mutable_location()->CopyFrom(*masterSyncInfo->mutable_transform()->mutable_location());
	overallSituationDataRequest.mutable_actor_add_transform()->mutable_rotator()->set_pitch(0);
	overallSituationDataRequest.mutable_actor_add_transform()->mutable_rotator()->set_yaw(0);
	overallSituationDataRequest.mutable_actor_add_transform()->mutable_rotator()->set_roll(0);
	overallSituationDataRequest.mutable_actor_add_transform()->mutable_scale()->set_x(1);
	overallSituationDataRequest.mutable_actor_add_transform()->mutable_scale()->set_y(1);
	overallSituationDataRequest.mutable_actor_add_transform()->mutable_scale()->set_z(1);

	FTransform masterTransform = UCommonlyObject::PBToUe4Transform(overallSituationDataRequest.mutable_actor_add_transform());

	// node
	{
		for (auto i = 0; i < nodeEntityIdList.size(); i++)
		{
			auto tempSyncInfo = nodeEntityIdList[i];

			auto childInfo = actorInfo->add_child();
			childInfo->set_xlsid(tempSyncInfo->child_list(0).mesh().xls_id());
			childInfo->set_materialid(tempSyncInfo->child_list(0).mesh().material_id());
			auto childWTransform = UCommonlyObject::PBToUe4Transform(tempSyncInfo->mutable_transform());
			auto childRTransform = childWTransform.GetRelativeTransform(masterTransform);

			UCommonlyObject::Ue4ToPbTransform(childRTransform, childInfo->mutable_transform());
			//UE_LOG(LogTemp, Log, TEXT("[%x] [合并实例] [NetCall_Place_Merge_AddActor]  [index_:%d]  [xlsId:%d] [location:%s] [scale:%s]"),
			//	this, i, tempSyncInfo->xlsid(), *childRTransform.GetLocation().ToString(), *childRTransform.GetScale3D().ToString());
		}
	}

	//return;
	gameMode->controller->NetCall_GameData(EMsgType::EMsgType_OverallSituationDataRequest, &overallSituationDataRequest);
}

// 网络响应 更新角色
void AOverallSituationView::NetCall_Place_Merge_UpdateActor(int entityId)
{
}

// 网络响应 移除角色
void AOverallSituationView::NetCall_Place_Merge_RemoveActor(int entityId)
{
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 网络回调 全局数据更变
void AOverallSituationView::NetCallback_BP_OverallSituationData_Change(Gamedata::OverallSituationDataResponse& info)
{
	if (CheckDefaultData() == false || CheckTargetData() == false)return;

	if (info.set_type() == Gamedata::OverallSituationDataType_push_all)
	{
		NetCallback_ChangeSkySet(info.mutable_data()->mutable_sky_info());
		NetCallback_get_all_actor(info.mutable_data()->mutable_actor_list());
	}
	if (info.set_type() == Gamedata::OverallSituationDataType_req_sky)
	{
		syncInfo.mutable_sky_info()->CopyFrom(*info.mutable_data()->mutable_sky_info());
		NetCallback_ChangeSkySet(info.mutable_sky_info());
	}
	else if (info.set_type() == Gamedata::OverallSituationDataType_req_get_all_actor)
	{
		NetCallback_get_all_actor(info.mutable_actor_list());
	}
	else if (info.set_type() == Gamedata::OverallSituationDataType_req_add_actor)
	{
		NetCallback_add_actor(info.mutable_actor_add());
	}
	else if (info.set_type() == Gamedata::OverallSituationDataType_req_update_actor)
	{
		NetCallback_actor_update(info.mutable_actor_update());
	}
	else if (info.set_type() == Gamedata::OverallSituationDataType_req_remove_actor)
	{
		NetCallback_remove_actor(info.actor_delete());
	}
}

// 网络响应 天气更变
void AOverallSituationView::NetCallback_ChangeSkySet(Gamedata::OverallSituationSkyInfo* info)
{
	syncInfo.mutable_sky_info()->CopyFrom(*info);
	Sky_Set(info);
}

// 天气系统 设置天气
void AOverallSituationView::Sky_Set(Gamedata::OverallSituationSkyInfo* info)
{
	FString cmd = FString::Printf(TEXT("Event_Sky_Set %d %d %d %d %d"),
		info->time(), info->weather(), info->cloud_coverageoverall(), info->cloud_speed(), info->moon_color());
	FOutputDeviceDebug device;
	skyControllerActor->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
}

// 网络响应 获取所有角色
void AOverallSituationView::NetCallback_get_all_actor(::google::protobuf::Map< ::google::protobuf::int32, ::Gamedata::EntityMeshMasterInfo >* info)
{
	syncInfo.mutable_data()->Clear();
	for (auto it = info->begin(); it != info->end(); ++it)
	{
		NetCallback_add_actor(&it->second);
	}
}

// 网络响应 新增合并角色
void AOverallSituationView::NetCallback_add_actor(Gamedata::EntityMeshMasterInfo* info)
{
	auto placeView = GetEditEntityManage();
	if (placeView == nullptr)return;

	syncInfo.mutable_data()->mutable_actor_list()->insert({ info->xls_id(),*info });

	// 刷新UI
	auto character = Cast<ABaseCharacter>(playerController->GetPawn());
	if (character)
	{
		character->entityMergeComponent->UI_MergeActor_AddLine(info);
	}
}

// 网络响应 更新合并角色
void AOverallSituationView::NetCallback_actor_update(Gamedata::EntityMeshMasterInfo* info)
{
}

// 网络响应 移除合并角色
void AOverallSituationView::NetCallback_remove_actor(int entityId)
{
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/
