// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceControllerMoveTimelineObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
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
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Tools/CalculationObject.h"
#include "UI/BaseHud.h"

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
#include "../Actor/ControllerComponent/EditEntityEditComponent.h"
#include "../Actor/EditEntityActor.h"

//#include<vector>

/*
灯光/激光束的控制器组件
*/

/*------------------------------------------------------------------*/

// UPlaceControllerMoveTimelineObject
UPlaceControllerMoveTimelineObject::UPlaceControllerMoveTimelineObject()
{
}

/*------------------------------------------------------------------*/

// 获取 sync数据更变local数据
void UPlaceControllerMoveTimelineObject::MackFLocalEntityInfo(Gamedata::EntityInfo* syncInfo,
	FLocalEntityInfo* localInfo)
{
	//auto data = controller->data;

	//auto xlsInfo = data->GetXlsInfoWithXlsId(syncInfo->xlsid());
	//if (xlsInfo == nullptr)return;
	//ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(controller->GetWorld()));

	//{
	//	// 灯光/激光束
	//	if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_laser)
	//		// if (syncInfo->type() == Gamedata::EntityType::EntityType_Light)
	//	{
	//		auto t2 = syncInfo->transform();

	//		auto spawnWTransform = UCommonlyObject::PBToUe4Transform(&t2);
	//		FString name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());
	//		localInfo->laser.meshName = FString::Printf(TEXT("ShowLaserMesh_%d_%s"), syncInfo->materialid(), *name);
	//		localInfo->multipleMesh[0].selectMeshName = FString::Printf(
	//			TEXT("SelectLaserMesh_%d_%s_%d"), syncInfo->materialid(), *name, syncInfo->entityid());


	//		// 计算灯光束真实的位置
	//		GetLaserRealTransform(syncInfo, localInfo);
	//	}
	//}
}

// 获取 激光束真实的位置
void UPlaceControllerMoveTimelineObject::GetLaserRealTransform(Gamedata::EntityInfo* syncInfo,
	FLocalEntityInfo* localInfo)
{
	//FCollisionQueryParams traceParams(FName(TEXT("test")), false, nullptr);
	//auto rotator = localInfo->laser.meshWTransform.GetRotation().Rotator().Vector();
	//auto start = localInfo->laser.meshWTransform.GetLocation();
	//auto end = start + rotator * 60000;
	//FVector realHitPoint = end;

	//FHitResult hitResult;
	//bool bBlocked = controller->GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Pawn,
	//	traceParams);
	//if (bBlocked)
	//{
	//	realHitPoint = hitResult.ImpactPoint;
	//}
	//auto xLen = (start - realHitPoint).Size() / 2;
	//auto laserLocation = start + rotator * xLen;
	//localInfo->laser.laserWTransform = localInfo->laser.meshWTransform;
	//localInfo->laser.laserWTransform.SetLocation(laserLocation);
	//FVector scale(xLen / 100, 0.1, 0.1);

	//// UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), laserLocation, 20, 20, FRotator::ZeroRotator, FLinearColor::Red, 1, 10);
	//// UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, realHitPoint, FLinearColor::Red, 0.2, 10);
	//localInfo->laser.laserWTransform.SetScale3D(scale);
}

// 获取 hud
ABaseHUD* UPlaceControllerMoveTimelineObject::GetHUD()
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController)
	{
		auto hud = Cast<ABaseHUD>(playerController->GetHUD());
		return hud;
	}
	return nullptr;
}

/*------------------------------------------------------------------*/

// BeginPlay
void UPlaceControllerMoveTimelineObject::Start(TWeakObjectPtr<UEditEntityManageController> controller_)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::Start]   "), this);
	controller = controller_;
}

// EndPlay
void UPlaceControllerMoveTimelineObject::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::End]   "), this);
}

/*------------------------------------------------------------------*/

//
void UPlaceControllerMoveTimelineObject::TickComponent(float DeltaTime)
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

	// 控制预览
	// 控制最大帧数(1/0.05=20)
	// {
	// 	auto nowTime = controller->GetWorld()->TimeSeconds;
	// 	if (nowTime >= (tickInfo.lastTime + 0.1))
	// 	{
	// 		tickInfo.lastTime = nowTime;
	// 		TickMove();
	// 	}
	// }
}

// tick
void UPlaceControllerMoveTimelineObject::TickMove()
{
	auto nowTime = controller->GetWorld()->TimeSeconds;
	auto view = controller->view;

	//view->actorMap

	//for (auto actor : view->actorMap)
	//{
	//	auto actor_ = actor.Value;
	//	auto syncInfo = &actor_->syncInfo;
	//	auto localInfo = &actor_->localInfo;
	//	auto xlsInfo = actor_->xlsInfo;

	//	//auto syncInfo = node.second;
	//	//auto xlsInfo = view->data->GetXlsInfo(syncInfo.entityid());
	//	if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_laser)
	//	{
	//		//auto localInfo = view->data->GetLocalInfo(node.first);
	//		//if (localInfo)
	//		{
	//			LocalPreviewMode(syncInfo, localInfo);
	//		}
	//	}
	//}
}

/*------------------------------------------------------------------*/

// 区域时间轴 时间轴控制器
void UPlaceControllerMoveTimelineObject::TimelineController()
{
}

// 区域时间轴 本地预览模式
void UPlaceControllerMoveTimelineObject::LocalPreviewMode(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	// if (localInfo->light.isOpenPreview == false)return;
	// if (syncInfo->laser_size() < localInfo->light.playIndex)return;
	//
	// // 时间轴基本信息
	// auto operating = &syncInfo->laser()[localInfo->light.playIndex];
	//
	// // 时间轴数组
	// auto entitytimeinfomap = &operating->entitytimeinfomap();
	//
	// auto nowTime = controller->GetWorld()->TimeSeconds;
	//
	// // 保留1位小数点
	// int playTime = (nowTime-localInfo->light.previewTime)*10;
	//
	// // 超出总时间后执行操作
	//
	// // 在时间节点里搜索该节点设置
	// if(playTime!=localInfo->light.lastTimeRun)
	// {
	// 	localInfo->light.lastTimeRun = playTime;
	//
	// 	// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::LocalPreviewMode] [playTime:%f] [playTime:%d]   "), this, (nowTime - localInfo->light.previewTime),playTime);
	//
	// 	// 播放当前时间节点数据
	// 	auto it = entitytimeinfomap->find(playTime);
	// 	if(it!= entitytimeinfomap->end())
	// 	{
	// 		auto entityTimeInfo = &it->second;
	// 		localInfo->light.actor->TickMove(playTime,entityTimeInfo);
	// 	}
	// }
}

/*------------------------------------------------------------------*/

// 预览 所有拥有时间轴的物体开始播放到指定位置
bool UPlaceControllerMoveTimelineObject::SetAllEntityTimelineFrame(float time)
{
	// UE_LOG(LogTemp, Log, TEXT("[%x] [全局时间轴] [SetAllEntityTimelineFrame]  [time:%f] "), this, time);

	//auto& syncEntityInfoMap = controller->data->syncEntityInfoMap;

	//for (auto it : controller->view->actorMap)
	//	//for (auto it : syncEntityInfoMap)
	//{
	//	auto syncInfo = &it.Value->syncInfo;
	//	//auto syncInfo = &syncEntityInfoMap[it.first];
	//	//if(syncInfo->movetimeline_size()>0)
	//	{
	//		auto localInfo = controller->data->GetLocalInfo(syncInfo->entityid());
	//		SetCurrentPlayLocationAndRotator(0, time * 1000, syncInfo, localInfo);
	//	}
	//}

	return true;
}

// 预览 设置当前播放帧播放的位置和旋转
bool UPlaceControllerMoveTimelineObject::SetCurrentPlayLocationAndRotator(
	int timelineIndex, int nowPlayTime, Gamedata::EntityInfo* syncInfo,
	FLocalEntityInfo* localInfo)
{
	// nowPlayTime 放大了1000倍

	bool lastFrame = false;
	//if (syncInfo->movetimeline_size() == 0 && timelineIndex >= syncInfo->movetimeline_size())
	//{
	//	return lastFrame;
	//}
	auto nowTime = controller->GetWorld()->TimeSeconds;
	// auto& laser = syncInfo->laser(0);
	// auto& alone = laser.alone(timelineIndex);
	//auto& movetimeline = syncInfo->movetimeline();

	//auto& nodemap = movetimeline.pointmap();

	// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::SetCurrentPlayLocationAndRotator] [playTime:%d] [nowTime:%f] "), this,
	//        nowPlayTime, nowTime);

	//// 只有一帧
	//int size = localInfo->timelimeKeyList.size();
	//if (size <= 1)
	//{
	//	return lastFrame;
	//}

	//// 超过最后一帧
	//auto& frameLastKey = localInfo->timelimeKeyList[size - 1];
	//if (nowPlayTime > frameLastKey && (nowPlayTime - frameLastKey) > 20)
	//{
	//	return lastFrame;
	//}

	//// 愚蠢的办法找到当前帧所在的位置
	//bool flag = false;
	//for (int i = 0; i < size; i++)
	//{
	//	// 前面帧
	//	if (i < size - 1)
	//	{
	//		auto& frameNowKey = localInfo->timelimeKeyList[i];
	//		auto& frameNextKey = localInfo->timelimeKeyList[i + 1];

	//		// 两帧之间
	//		if (frameNowKey <= nowPlayTime && nowPlayTime < frameNextKey)
	//		{
	//			auto frameNowInfo = nodemap.find(frameNowKey)->second;
	//			auto frameNextInfo = nodemap.find(frameNextKey)->second;

	//			// 获取插帧值
	//			// frameNowInfo.ishide();

	//			auto timeLenght = float(frameNextKey - frameNowKey) / 1000.0f;
	//			auto timeRun = float(nowPlayTime - frameNowKey) / 1000.0f;

	//			// 关键帧 当前关键帧播放的第一帧(离帧头非常近并且处于切换帧状态)
	//			if (nowPlayTime == frameNowKey
	//				|| (timeRun < 0.1 && frameNowKey != localInfo->timelineKeyIndex))
	//			{
	//				localInfo->timelineKeyIndex = frameNowKey;

	//				auto r1 = UCommonlyObject::PBToUe4Vector(&frameNowInfo.location());
	//				SetEntityLocalTempLocationOrRotator(syncInfo->entityid(), movetimeline.ismove(), r1);

	//				UE_LOG(LogTemp, Log,
	//					TEXT(
	//						"[%x] [UPlaceControllerMoveTimelineObject::SetCurrentPlayLocationAndRotator] 关键帧 [nowTime:%f]  [playTime:%d] [frameNowKey:%d] [index:%d/%d]  "
	//					), this, nowTime, nowPlayTime, frameNowKey, i, size - 1);
	//			}
	//			// 插帧
	//			else
	//			{
	//				auto r1 = UCommonlyObject::PBToUe4Vector(&frameNowInfo.location());
	//				auto r2 = UCommonlyObject::PBToUe4Vector(&frameNextInfo.location());
	//				// if (movetimeline.ismove())
	//				{
	//					auto r3 = UKismetMathLibrary::VInterpTo(r1, r2, timeRun, 1.0 / timeLenght);
	//					SetEntityLocalTempLocationOrRotator(syncInfo->entityid(), true, r3);
	//				}
	//				// else
	//				// {
	//				// 	auto r3 = UKismetMathLibrary::
	//				// 		RInterpTo(r1.Rotation(), r2.Rotation(), timeRun, 1.0 / timeLenght);
	//				// 	SetEntityLocalTempLocationOrRotator(syncInfo->entityid(), false, r3.Vector());
	//				// }

	//				// UE_LOG(LogTemp, Log,
	//				//        TEXT(
	//				// 	       "[%x] [UPlaceControllerMoveTimelineObject::SetCurrentPlayLocationAndRotator] 插帧 [nowTime:%f]  [playTime:%d] [frameNowKey:%d] [index:%d/%d] [time:%f/%f] "
	//				//        ),
	//				//        this, nowTime, nowPlayTime, frameNowKey, i, size - 1, timeRun, timeLenght, *r1.ToString(), *r2.ToString(),
	//				//        *r3.ToString());
	//			}
	//			flag = true;
	//			break;
	//		}
	//	}
	//	// 最后一帧
	//	else if (i == size - 1 && nowPlayTime >= frameLastKey)
	//	{
	//		auto frame = nodemap.find(frameLastKey)->second;

	//		// frame.ishide();

	//		auto r1 = UCommonlyObject::PBToUe4Vector(&frame.location());
	//		SetEntityLocalTempLocationOrRotator(syncInfo->entityid(), movetimeline.ismove(), r1);

	//		UE_LOG(LogTemp, Log,
	//			TEXT(
	//				"[%x] [UPlaceControllerMoveTimelineObject::SetCurrentPlayLocationAndRotator] 最后一帧 [nowTime:%f]  [playTime:%d]  [frameNowKey:%d] [index:%d/%d]  "
	//			), this, nowTime, nowPlayTime, frameLastKey, i, size - 1);
	//		flag = true;
	//		lastFrame = true;
	//		break;
	//	}
	//}

	return lastFrame;
}

// 预览 插帧计算
float UPlaceControllerMoveTimelineObject::GetInsertFrameSmoothValue(float start, float end, float time)
{
	return 0;
}

// 预览 本地设置某一个灯为预览模式,并且该预览从什么时候开始播放,以什么速度播放
void UPlaceControllerMoveTimelineObject::SetPreviewData(int32 entityId, float startTime, float speed, int playIndex)
{
	// auto view = controller->view;
	// auto nowTime = controller->GetWorld()->TimeSeconds;
	//
	// if (view->data->IsExistData(entityId) == false)return;
	//
	// auto localInfo = view->data->GetLocalInfo(entityId);
	// auto syncInfo = view->data->GetSyncInfo(entityId);
}

/*------------------------------------------------------------------*/

// 录制 本地每帧
void UPlaceControllerMoveTimelineObject::SetEntityLocalTempLocationOrRotator(int32 entityId, bool isMove, FVector move)
{
	// isMove true:移动 flase:旋转
	auto editEntityManage = controller->view;
	if (editEntityManage->data->IsExistData(entityId) == false)return;

	if (isMove)
	{
		//view->controller->EntityLocalMoveMain(entityId, ESelectedWorkMode::WorldLocation, move, FRotator::ZeroRotator,
		//	FVector::ZeroVector);

		if (auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(entityId)))
		{
			entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::WorldLocation, move, FRotator::ZeroRotator, FVector::ZeroVector);
		}
	}
	else
	{
		//view->controller->EntityLocalMoveMain(entityId, ESelectedWorkMode::WorldRotator, FVector::ZeroVector,
		//	move.Rotation(), FVector::ZeroVector);
		if (auto entityActor = Cast<AEditEntityActor>(editEntityManage->data->GetEntityActor(entityId)))
		{
			entityActor->editComponent->EntityLocalMoveMain(ESelectedWorkMode::WorldRotator, FVector::ZeroVector, move.Rotation(), FVector::ZeroVector);
		}
	}
}

// 录制 进入编辑模式时初始化
void UPlaceControllerMoveTimelineObject::InitTimeline(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//if (syncInfo->movetimeline_size() == 0)

	//int timelineIndex = 0;
	//auto& movetimeline = syncInfo->movetimeline();
	//if (movetimeline.IsInitialized() == false)
	//{
	//	WriteTimelineHeadData(syncInfo, localInfo);
	//}

	//auto nodemap = movetimeline.pointmap();
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::InitTimeline]  "), this);
}

// 录制 加入一个时间轴
int UPlaceControllerMoveTimelineObject::WriteTimelineHeadData(Gamedata::EntityInfo* syncInfo,
	FLocalEntityInfo* localInfo)
{
	//auto xlsInfo = controller->data->GetXlsInfo(syncInfo->entityid());

	//auto movetimeline = syncInfo->mutable_movetimeline();
	////int timelineIndex = syncInfo->movetimeline_size() - 1;

	//FString timelineName = "the0";
	//std::string str(TCHAR_TO_UTF8(*timelineName));
	//movetimeline->set_name(str);
	//movetimeline->set_entitymovetimelineendofplay(Gamedata::EntityMoveTimelineEndOfPlay_ShutDown);
	//movetimeline->set_isreflection(false);
	//movetimeline->set_size(1);
	//movetimeline->set_lenght(40000);
	//movetimeline->set_playlenght(10);

	//if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_laser)
	//{
	//	movetimeline->set_ismove(false);
	//}
	//else
	//{
	//	movetimeline->set_ismove(true);
	//}
	//movetimeline->set_moveyaw(true);

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::WriteTimelineHeadData] "), this);

	return 0;
}

// 录制 写入时间轴预览数据
void UPlaceControllerMoveTimelineObject::WriteTimelineLightData(Gamedata::EntityInfo* syncInfo,
	FLocalEntityInfo* localInfo,
	FVector move, float enterTime, int timelineIndex)
{
	//// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::SetEntityLocalTempLocationOrRotator]  [entityId:%d] "), this, entityId);

	//// auto view = controller->view;
	//// if (view->data->IsExistData(entityId) == false)return;
	//auto nowTime = controller->GetWorld()->TimeSeconds;

	//// 当前组时间轴(默认第0组)
	////if (syncInfo->movetimeline_size() == 0)
	////{
	////	return;
	////}
	//auto movetimeline = syncInfo->mutable_movetimeline();

	//// 四舍五入保留两位小数
	//auto timeKey = ((float)((int)((enterTime + 0.005) * 100))) / 100;

	//int timeKey2 = timeKey * 1000;

	//// 计算当前时间节点

	//// Gamedata::EntityMoveFrameKey entityTimeInfo;
	//// entityTimeInfo.set_ishide(isHide);
	//// entityTimeInfo.set_colortype(colorType);
	////
	//// entityTimeInfo.mutable_move()->set_x(move.X);
	//// entityTimeInfo.mutable_move()->set_y(move.Y);
	//// entityTimeInfo.mutable_move()->set_z(move.Z);

	//Gamedata::EntityMoveTimelinePoint pointInfo;
	//Gamedata::FVector location;
	//location.set_x(move.X);
	//location.set_y(move.Y);
	//location.set_z(move.Z);

	//pointInfo.mutable_location()->CopyFrom(location);
	//pointInfo.set_uniquemark(0);

	//auto pointmap = movetimeline->mutable_pointmap();
	//pointmap->insert({ timeKey2, pointInfo });

	//UE_LOG(LogTemp, Log,
	//	TEXT(
	//		"[%x] [UPlaceControllerMoveTimelineObject::WriteTimelineLightData] [nowTime:%f] [entityId:%d] [playTime:%f -- %f] [key:%d]  [count:%d]"
	//	), this, nowTime, syncInfo->entityid(), enterTime, timeKey, timeKey2, pointmap->size());
}

// 录制 本地key排序数组(鼠标左键本地录制结束的时候调用)
void UPlaceControllerMoveTimelineObject::WriteTimelineKeyList(int timelineIndex, Gamedata::EntityInfo* syncInfo,
	FLocalEntityInfo* localInfo)
{
	//auto& movetimeline = syncInfo->movetimeline();
	// auto& alone = laser.alone(timelineIndex);
	//localInfo->timelimeKeyList.clear();
	//for (auto node : movetimeline.pointmap())
	//{
	//	auto key = node.first;

	//	localInfo->timelimeKeyList.push_back(key);

	//	// UE_LOG(LogTemp, Log,TEXT("[%x] [UPlaceControllerMoveTimelineObject::WriteTimelineKeyList] A [node:%d]"), this, key);
	//}
	//std::sort(localInfo->timelimeKeyList.begin(), localInfo->timelimeKeyList.end());

	//for (auto node : localInfo->timelimeKeyList)
	//{
	//	// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::WriteTimelineKeyList] B [node:%d]"), this, node);
	//}
}

/*------------------------------------------------------------------*/

// 测试
void UPlaceControllerMoveTimelineObject::TestFunc()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceControllerMoveTimelineObject::TestFunc] "), this);

	// SetPreviewData(319,0,1,0);
}

/*------------------------------------------------------------------*/
