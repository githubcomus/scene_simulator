// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityTimelineComponent.h"
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
#include "../../../SceneStaticMesh/SceneStaticMeshView.h"
#include <Curves/CurveVector.h>
#include "../../Timeline/EditEntityManageTimelineComponent.h"
#include "EditEntityChildComponent.h"
#include "Yxjs/Tools/CalculationObject.h"
#include "../../../Selected/SelectedViewActor.h"
#include "EditEntityEditComponent.h"
#include "../../../Selected/SelectedControllerComponent.h"

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

// 获取指定时间的actor位移数据
FTransform UEditEntityTimelineComponent::GetActorTimelineTransform(float time)
{
	FTransform transform;
	transform.SetScale3D(FVector::OneVector);
	transform.SetLocation(locationCurveVector->GetVectorValue(time));
	auto rotator = rotationCurveVector->GetVectorValue(time);
	transform.SetRotation(FRotator(rotator.X, rotator.Y, rotator.Z).Quaternion());
	//UE_LOG(LogTemp, Log, TEXT("[%x] [旋转] [GetActorTimelineTransform] 获取指定时间的 [rotator:%s]"), this, *rotator.ToString());
	return transform;
}

// 获取指定时间的颜色
FVector UEditEntityTimelineComponent::GetActorTimelineColor(float time)
{
	if (colorCurveVector == nullptr)return FVector::ZeroVector;
	return colorCurveVector->GetVectorValue(time);;
}

// 获取指定时间的actor位移数据
void UEditEntityTimelineComponent::GetActorTimelineTransform2(float time, FVector& location, FVector& rotator, FVector& color)
{
	if (locationCurveVector == nullptr || rotationCurveVector == nullptr || colorCurveVector == nullptr)return;
	location = locationCurveVector->GetVectorValue(time);
	rotator = rotationCurveVector->GetVectorValue(time);
	color = colorCurveVector->GetVectorValue(time);
}

// 获取指定时间的位移数据
void UEditEntityTimelineComponent::GetChildTimelineTransform(FTransform relativelyTransform, TimelineOneFrameOneEntityData& oneFrameOneEntityData)
{
	auto newWorldtransform = UCalculationObject::RelativeToWorldTransform(
		owner_->GetObserverTransform(), relativelyTransform);

	for (auto node : owner_->childComponentList)
	{
		auto worldTransform = UCalculationObject::RelativeToWorldTransform(
			newWorldtransform, node->meshInfo.rTransform);
		oneFrameOneEntityData.child_transform.push_back(worldTransform);
	}
}

// 获取指定时间的位移数据
void UEditEntityTimelineComponent::GetChildTimelineTransform(float time, TimelineOneFrameOneEntityData& oneFrameOneEntityData)
{
	float tempVar = time / time_lenght;
	float loopTime = (tempVar - (int)tempVar) * time_lenght;
	auto relativelyTransform = GetActorTimelineTransform(loopTime);
	GetChildTimelineTransform(relativelyTransform, oneFrameOneEntityData);
}

// 网络更新是否需要更新时间轴数据
bool UEditEntityTimelineComponent::IsTimelineUpdate(Gamedata::EntityInfo* newSyncInfo)
{
	if (owner_->xlsInfo->UseTimeline_ == false)return false;

	// type发生更变
	//if (owner_->IsTimelineCommandText())return true;

	// 对比版本
	if (newSyncInfo->has_transform_timeline() && owner_->syncInfo.has_transform_timeline())
	{
		Gamedata::EntityTransformTimeline* new_transform_timeline = newSyncInfo->mutable_transform_timeline();
		int new_version = new_transform_timeline->version();
		Gamedata::EntityTransformTimeline* old_transform_timeline = owner_->syncInfo.mutable_transform_timeline();
		int old_version = old_transform_timeline->version();
		if (new_version != old_version)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// 新增
	else if (newSyncInfo->has_transform_timeline() && owner_->syncInfo.has_transform_timeline() == false)
	{
		return true;
	}
	// 移除
	else if (newSyncInfo->has_transform_timeline() == false && owner_->syncInfo.has_transform_timeline())
	{
		return true;
	}
	return false;
}

// 获取当前时间显示的变换
void UEditEntityTimelineComponent::GetNowTransform(bool isRelativaly, float nowTime, FVector& location, FRotator& rotator, FVector& color)
{
	// isRelativaly true:相对变换

	if (time_lenght == 0)
	{
		location = FVector::ZeroVector;
		rotator = FRotator::ZeroRotator;
		color = FVector::ZeroVector;
		return;
	}
	FVector rotatorTemp;
	GetActorTimelineTransform2(nowTime, location, rotatorTemp, color);
	rotator.Pitch = rotatorTemp.X;
	rotator.Yaw = rotatorTemp.Y;
	rotator.Roll = rotatorTemp.Z;


	//FTransform newWorldtransform(FRotator::ZeroRotator, FVector::ZeroVector);

	//if (time_lenght == 0) {
	//	return newWorldtransform;
	//}
	//auto relativelyTransform = GetActorTimelineTransform(nowTime);

	//if (isRelativaly) {
	//	newWorldtransform = relativelyTransform;
	//}
	//else {
	//	newWorldtransform = UCalculationObject::RelativeToWorldTransform(
	//		owner_->GetObserverTransform(), relativelyTransform);
	//}
	//return newWorldtransform;
}

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
UEditEntityTimelineComponent::UEditEntityTimelineComponent()
{
}

// BeginPlay
void UEditEntityTimelineComponent::Start(AEditEntityActor* view)
{
	owner_ = view;
	if (owner_->gameMode->sceneStaticMeshView == nullptr)return;
	editEntityManage = owner_->editEntityManage;

	if (owner_->xlsInfo->UseTimeline_ == false)return;

	TestFunc();

	UpdateTimeline();

	if (owner_->IsTimelineAuto())
	{
		RegisterTick(true);
	}
}

// EndPlay
void UEditEntityTimelineComponent::End()
{
	if (owner_.IsValid() == false || owner_->gameMode->sceneStaticMeshView == nullptr)return;

	// box

	// 从tick移除
	//RegisterTick(false);
	if (owner_->IsTimelineAuto())
	{
		RegisterTick(false);
	}
}

// 加入一个关键帧
void UEditEntityTimelineComponent::AddKeyFrame(int time, FVector location, FRotator rotator, FVector color)
{
	if (locationCurveVector == nullptr)
	{
		return;
	}
	if (rotationCurveVector == nullptr)
	{
		return;
	}
	FRichCurve& location_x = locationCurveVector->FloatCurves[0];
	FRichCurve& location_y = locationCurveVector->FloatCurves[1];
	FRichCurve& location_z = locationCurveVector->FloatCurves[2];
	FRichCurve& rotation_pitch = rotationCurveVector->FloatCurves[0];
	FRichCurve& rotation_yaw = rotationCurveVector->FloatCurves[1];
	FRichCurve& rotation_roll = rotationCurveVector->FloatCurves[2];
	FRichCurve& color_r = colorCurveVector->FloatCurves[0];
	FRichCurve& color_g = colorCurveVector->FloatCurves[1];
	FRichCurve& color_b = colorCurveVector->FloatCurves[2];

	float realTime = time / 1000.0f;

	FKeyHandleInfo keyHandleInfo;
	keyHandleInfo.time = time;
	keyHandleInfo.jobId = jobIdNumber;

	{
		keyHandleInfo.handle_x = location_x.AddKey(realTime, location.X);
		keyHandleInfo.handle_y = location_y.AddKey(realTime, location.Y);
		keyHandleInfo.handle_z = location_z.AddKey(realTime, location.Z);

		location_x.SetKeyInterpMode(keyHandleInfo.handle_x, ERichCurveInterpMode::RCIM_Linear);
		location_y.SetKeyInterpMode(keyHandleInfo.handle_y, ERichCurveInterpMode::RCIM_Linear);
		location_z.SetKeyInterpMode(keyHandleInfo.handle_z, ERichCurveInterpMode::RCIM_Linear);
	}
	{
		keyHandleInfo.handle_roll = rotation_pitch.AddKey(realTime, rotator.Pitch, true);
		keyHandleInfo.handle_pitch = rotation_yaw.AddKey(realTime, rotator.Yaw, true);
		keyHandleInfo.handle_yaw = rotation_roll.AddKey(realTime, rotator.Roll, true);

		rotation_pitch.SetKeyInterpMode(keyHandleInfo.handle_roll, ERichCurveInterpMode::RCIM_Linear);
		rotation_yaw.SetKeyInterpMode(keyHandleInfo.handle_pitch, ERichCurveInterpMode::RCIM_Linear);
		rotation_roll.SetKeyInterpMode(keyHandleInfo.handle_yaw, ERichCurveInterpMode::RCIM_Linear);
	}
	{
		keyHandleInfo.handle_r = color_r.AddKey(realTime, color.X);
		keyHandleInfo.handle_g = color_g.AddKey(realTime, color.Y);
		keyHandleInfo.handle_b = color_b.AddKey(realTime, color.Z);

		color_r.SetKeyInterpMode(keyHandleInfo.handle_r, ERichCurveInterpMode::RCIM_Linear);
		color_g.SetKeyInterpMode(keyHandleInfo.handle_g, ERichCurveInterpMode::RCIM_Linear);
		color_b.SetKeyInterpMode(keyHandleInfo.handle_b, ERichCurveInterpMode::RCIM_Linear);
	}
	keyInfoMap.Add(jobIdNumber, keyHandleInfo);
	jobIdNumber++;
}

// 移除一个关键帧
void UEditEntityTimelineComponent::RemoveKeyFrame(int jobId)
{
	if (locationCurveVector == nullptr)
	{
		return;
	}
	if (rotationCurveVector == nullptr)
	{
		return;
	}
	if (colorCurveVector == nullptr)
	{
		return;
	}
	if (keyInfoMap.Find(jobId) == nullptr)
	{
		return;
	}

	FRichCurve& location_x = locationCurveVector->FloatCurves[0];
	FRichCurve& location_y = locationCurveVector->FloatCurves[1];
	FRichCurve& location_z = locationCurveVector->FloatCurves[2];
	FRichCurve& rotation_pitch = rotationCurveVector->FloatCurves[0];
	FRichCurve& rotation_yaw = rotationCurveVector->FloatCurves[1];
	FRichCurve& rotation_roll = rotationCurveVector->FloatCurves[2];
	FRichCurve& color_r = colorCurveVector->FloatCurves[0];
	FRichCurve& color_g = colorCurveVector->FloatCurves[1];
	FRichCurve& color_b = colorCurveVector->FloatCurves[2];

	FKeyHandleInfo& keyHandleInfo = keyInfoMap[jobId];

	location_x.DeleteKey(keyHandleInfo.handle_x);
	location_y.DeleteKey(keyHandleInfo.handle_y);
	location_z.DeleteKey(keyHandleInfo.handle_z);
	rotation_pitch.DeleteKey(keyHandleInfo.handle_pitch);
	rotation_yaw.DeleteKey(keyHandleInfo.handle_yaw);
	rotation_roll.DeleteKey(keyHandleInfo.handle_roll);
	color_r.DeleteKey(keyHandleInfo.handle_r);
	color_g.DeleteKey(keyHandleInfo.handle_g);
	color_b.DeleteKey(keyHandleInfo.handle_b);

	keyInfoMap.Remove(jobId);
}

// 更新关键帧数据
void UEditEntityTimelineComponent::UpdateKeyFrame(int jobId, int time, FVector location, FRotator rotator, FVector color)
{
	if (locationCurveVector == nullptr)
	{
		return;
	}
	if (rotationCurveVector == nullptr)
	{
		return;
	}
	if (keyInfoMap.Find(jobId) == nullptr)
	{
		return;
	}
	FRichCurve& location_x = locationCurveVector->FloatCurves[0];
	FRichCurve& location_y = locationCurveVector->FloatCurves[1];
	FRichCurve& location_z = locationCurveVector->FloatCurves[2];
	FRichCurve& rotation_pitch = rotationCurveVector->FloatCurves[0];
	FRichCurve& rotation_yaw = rotationCurveVector->FloatCurves[1];
	FRichCurve& rotation_roll = rotationCurveVector->FloatCurves[2];
	FRichCurve& color_r = colorCurveVector->FloatCurves[0];
	FRichCurve& color_g = colorCurveVector->FloatCurves[1];
	FRichCurve& color_b = colorCurveVector->FloatCurves[2];

	FKeyHandleInfo& keyHandleInfo = keyInfoMap[jobId];

	float realTime = time / 1000.0f;
	location_x.SetKeyTime(keyHandleInfo.handle_x, realTime);
	location_y.SetKeyTime(keyHandleInfo.handle_y, realTime);
	location_z.SetKeyTime(keyHandleInfo.handle_z, realTime);
	rotation_pitch.SetKeyTime(keyHandleInfo.handle_pitch, realTime);
	rotation_yaw.SetKeyTime(keyHandleInfo.handle_yaw, realTime);
	rotation_roll.SetKeyTime(keyHandleInfo.handle_roll, realTime);
	color_r.SetKeyTime(keyHandleInfo.handle_r, realTime);
	color_g.SetKeyTime(keyHandleInfo.handle_g, realTime);
	color_b.SetKeyTime(keyHandleInfo.handle_b, realTime);

	location_x.SetKeyValue(keyHandleInfo.handle_x, location.X);
	location_y.SetKeyValue(keyHandleInfo.handle_y, location.Y);
	location_z.SetKeyValue(keyHandleInfo.handle_z, location.Z);
	rotation_pitch.SetKeyValue(keyHandleInfo.handle_pitch, rotator.Roll);
	rotation_yaw.SetKeyValue(keyHandleInfo.handle_yaw, rotator.Pitch);
	rotation_roll.SetKeyValue(keyHandleInfo.handle_roll, rotator.Yaw);
	color_r.SetKeyValue(keyHandleInfo.handle_r, color.X);
	color_g.SetKeyValue(keyHandleInfo.handle_g, color.Y);
	color_b.SetKeyValue(keyHandleInfo.handle_b, color.Z);
}

/*------------------------------------------------------------------*/

// 保存 实例平移 Call_EntityChangeTransform()->实例更变变换
void UEditEntityTimelineComponent::UpdateEntityDataTransform(FTimelineEnterPointInfo* timelinePointInfo, int entityId, FTimelinePointIndex& pointIndex, CvTimelineUIInfo& info)
{
	if (timelinePointInfo == nullptr)return;

	Gamedata::EntityInfo* syncInfo = &owner_->syncInfo;
	if (syncInfo == nullptr)
	{
		return;
	}

	Gamedata::EntityTransformTimeline* transform_timeline = syncInfo->mutable_transform_timeline();
	if (transform_timeline->point_map_size() > 0)
	{
		google::protobuf::Map<::google::protobuf::int32, ::Gamedata::EntityTransformTimelinePoint>* point_map = transform_timeline->mutable_point_map();

		auto it = point_map->find(timelinePointInfo->keyTime);
		if (it != point_map->end())
		{
			Gamedata::FVector location = UCommonlyObject::Ue4ToPbVector(info.location);
			Gamedata::FRotator rotation = UCommonlyObject::Ue4ToPbRotator(info.rotator);
			Gamedata::FVector color_ = UCommonlyObject::Ue4ToPbVector(info.color);

			it->second.mutable_location()->CopyFrom(location);
			it->second.mutable_rotator()->CopyFrom(rotation);
			it->second.mutable_color()->CopyFrom(color_);
		}
	}
	transform_timeline->set_color_teleport(info.colorTeleport);
	transform_timeline->set_timeline_highlight((Gamedata::TimelineHighlightType)info.highlight);
	transform_timeline->set_location_teleport(info.locationTeleport);
	UpdateTimeline();
}

// 保存 点 新增点 调用外部
void UEditEntityTimelineComponent::EntityAddPointData(int entityId, float time, ETimelinePointType timelinePointType,
	FVector location, FRotator rotation, FVector color)
{
	Gamedata::EntityInfo* syncInfo = &owner_->syncInfo;
	if (syncInfo == nullptr)
	{
		return;
	}

	auto transform_timeline = syncInfo->mutable_transform_timeline();
	auto point_map = transform_timeline->mutable_point_map();

	if (point_map->size() == 0)
	{
		transform_timeline->set_version(10);
	}
	else
	{
		transform_timeline->set_version(transform_timeline->version() + 1);
	}

	// 找到最大的索引,+1
	int uniqueMarkNum = 1;
	if (point_map->size() == 0)
	{	// 第一行数据就是1
	}
	// 最大数值+1
	else
	{
		for (auto node : *point_map)
		{
			auto tempuniquemark = node.second.uniquemark();
			if (tempuniquemark > uniqueMarkNum)
			{
				uniqueMarkNum = tempuniquemark;
			}
		}
		uniqueMarkNum++;
	}

	Gamedata::EntityTransformTimelinePoint pointInfo;
	pointInfo.mutable_location()->set_x(location.X);
	pointInfo.mutable_location()->set_y(location.Y);
	pointInfo.mutable_location()->set_z(location.Z);
	pointInfo.mutable_rotator()->set_pitch(rotation.Pitch);
	pointInfo.mutable_rotator()->set_roll(rotation.Roll);
	pointInfo.mutable_rotator()->set_yaw(rotation.Yaw);
	pointInfo.mutable_color()->set_x(color.X);
	pointInfo.mutable_color()->set_y(color.Y);
	pointInfo.mutable_color()->set_z(color.Z);
	pointInfo.set_uniquemark(uniqueMarkNum);

	int keyTime2 = time * 1000;
	point_map->insert({ keyTime2, pointInfo });

	//auto entityAcotr = GetEntityAcotr(entityId);
	UpdateTimeline();
}

// 保存 点 拖动点 调用外部
void UEditEntityTimelineComponent::MoveEntityData(FTimelineEnterPointInfo* timelinePointInfo, int newTime, FVector2D move)
{
	if (timelinePointInfo == nullptr)return;

	Gamedata::EntityInfo* syncInfo = &owner_->syncInfo;
	if (syncInfo == nullptr)
	{
		return;
	}


	// 当前key
	// 预计移动到的key

	int oldTime = timelinePointInfo->keyTime;

	::Gamedata::EntityTransformTimelinePoint oldPointInfo;

	// 存入背景时间轴点数据
	Gamedata::EntityTransformTimeline* transform_timeline = syncInfo->mutable_transform_timeline();
	if (transform_timeline->point_map_size() > 0)
	{
		// 背景时间轴排序
		google::protobuf::Map<::google::protobuf::int32, ::Gamedata::EntityTransformTimelinePoint>* nodemap = transform_timeline->mutable_point_map();

		auto it = nodemap->find(oldTime);
		if (it != nodemap->end())
		{
			oldPointInfo.CopyFrom(it->second);
			nodemap->erase(oldTime);
		}

		nodemap->insert({ newTime, oldPointInfo });
	}

	transform_timeline->set_version(transform_timeline->version() + 1);

	UpdateTimeline();
}

// 保存 点 删除点 调用外部
void UEditEntityTimelineComponent::PointDeleteData(FTimelineEnterPointInfo* timelinePointInfo, FTimelinePointIndex& selectInfo)
{
	//FTimelineEnterPointInfo* timelinePointInfo = GetTimelinePointInfo(selectInfo.entityId,
	//	selectInfo.uniqueMark);
	if (timelinePointInfo == nullptr)return;

	Gamedata::EntityInfo* syncInfo = &owner_->syncInfo;
	if (syncInfo == nullptr)
	{
		return;
	}

	//if (entityLineInfoMap.Find(selectInfo.entityId) == nullptr)
	//{
	//	return;
	//}

	// 当前key
	// 预计移动到的key

	int oldTime = timelinePointInfo->keyTime;

	// 存入背景时间轴点数据
	Gamedata::EntityTransformTimeline* transform_timeline = syncInfo->mutable_transform_timeline();
	if (transform_timeline->point_map_size() > 0)
	{
		// 背景时间轴排序
		google::protobuf::Map<::google::protobuf::int32, ::Gamedata::EntityTransformTimelinePoint>* nodemap = transform_timeline->mutable_point_map();

		auto it = nodemap->find(oldTime);
		if (it != nodemap->end())
		{
			nodemap->erase(oldTime);
		}
	}
	transform_timeline->set_version(transform_timeline->version() + 1);

	UpdateTimeline();
}

/*------------------------------------------------------------------*/

// 刷新时间轴 syncInfo -> FRichCurve
void UEditEntityTimelineComponent::UpdateTimeline()
{


	if (owner_->entityId == PreviewEntityID)return;

	if (owner_->syncInfo.has_transform_timeline() && owner_->syncInfo.mutable_transform_timeline()->point_map_size() > 0)
	{
		if (locationCurveVector == nullptr)
		{
			locationCurveVector = NewObject<UCurveVector>(GetTransientPackage(), UCurveVector::StaticClass(), NAME_None, RF_Transactional | RF_Public);
		}
		if (rotationCurveVector == nullptr)
		{
			rotationCurveVector = NewObject<UCurveVector>(GetTransientPackage(), UCurveVector::StaticClass(), NAME_None, RF_Transactional | RF_Public);
		}
		if (colorCurveVector == nullptr)
		{
			colorCurveVector = NewObject<UCurveVector>(GetTransientPackage(), UCurveVector::StaticClass(), NAME_None, RF_Transactional | RF_Public);
		}

		FRichCurve& location_x = locationCurveVector->FloatCurves[0];
		FRichCurve& location_y = locationCurveVector->FloatCurves[1];
		FRichCurve& location_z = locationCurveVector->FloatCurves[2];
		location_x.Reset();
		location_y.Reset();
		location_z.Reset();
		FRichCurve& rotation_x = rotationCurveVector->FloatCurves[0];
		FRichCurve& rotation_y = rotationCurveVector->FloatCurves[1];
		FRichCurve& rotation_z = rotationCurveVector->FloatCurves[2];
		rotation_x.Reset();
		rotation_y.Reset();
		rotation_z.Reset();
		FRichCurve& color_r = colorCurveVector->FloatCurves[0];
		FRichCurve& color_g = colorCurveVector->FloatCurves[1];
		FRichCurve& color_b = colorCurveVector->FloatCurves[2];
		color_r.Reset();
		color_g.Reset();
		color_b.Reset();

		auto& point_map = *owner_->syncInfo.mutable_transform_timeline()->mutable_point_map();

		int minTime = 0;
		int maxTime = 0;
		for (auto it = point_map.begin(); it != point_map.end(); ++it)
		{
			FVector location = UCommonlyObject::PBToUe4Vector(&it->second.location());
			FRotator rotator = UCommonlyObject::PBToUe4Rotator(&it->second.rotator());
			FVector color = UCommonlyObject::PBToUe4Vector(&it->second.color());
			AddKeyFrame(it->first, location, rotator, color);
			if (it->first < minTime)
			{
				minTime = it->first;
			}
			if (it->first > maxTime)
			{
				maxTime = it->first;
			}
		}
		time_lenght = (maxTime - minTime) / 1000.0f;
		//time_lenght = owner_->syncInfo.mutable_transform_timeline()->time_lenght();

	}
	else
	{
		if (locationCurveVector)
		{
			FRichCurve& location_x = locationCurveVector->FloatCurves[0];
			FRichCurve& location_y = locationCurveVector->FloatCurves[1];
			FRichCurve& location_z = locationCurveVector->FloatCurves[2];
			location_x.Reset();
			location_y.Reset();
			location_z.Reset();
			locationCurveVector = nullptr;
		}
		if (rotationCurveVector)
		{
			FRichCurve& rotation_x = rotationCurveVector->FloatCurves[0];
			FRichCurve& rotation_y = rotationCurveVector->FloatCurves[1];
			FRichCurve& rotation_z = rotationCurveVector->FloatCurves[2];
			rotation_x.Reset();
			rotation_y.Reset();
			rotation_z.Reset();
			rotationCurveVector = nullptr;
		}
		if (colorCurveVector)
		{
			FRichCurve& color_r = colorCurveVector->FloatCurves[0];
			FRichCurve& color_g = colorCurveVector->FloatCurves[1];
			FRichCurve& color_b = colorCurveVector->FloatCurves[2];
			color_r.Reset();
			color_g.Reset();
			color_b.Reset();
			colorCurveVector = nullptr;
		}
		time_lenght = 0;
	}

	//bool flag = time_lenght > 0 ? true : false;
	//RegisterTick(flag);


}

// 注册移除tick组
void UEditEntityTimelineComponent::RegisterTick(bool isRegister)
{

	if (owner_->entityId == PreviewEntityID)return;

	// 无运转长度
	if (isRegister && time_lenght == 0)
	{
		return;
	}

	// 无命令数据
	//if (isRegister && owner_->syncInfo.has_command() == false) {

	//}
	// 有命令数据但是自动播放
	//else if (isRegister && owner_->syncInfo.has_command() && owner_->syncInfo.command().type() == Gamedata::EntityCommandType::EntityCommandType_auto) {

	//}

	// 有命令数据但不是自动播放的情况
	if (isRegister && owner_->syncInfo.has_command() && owner_->syncInfo.command().type() != Gamedata::EntityCommandType::EntityCommandType_auto)
	{
		return;
	}

	auto& programme = owner_->editEntityManage->timeline->programme;

	if (programme == 2)
	{
		auto& timelineTickActorMap = owner_->editEntityManage->timeline->timelineTickActorMap;
		if (isRegister)
		{
			if (timelineTickActorMap.Find(owner_->syncInfo.entityid()))
			{
				timelineTickActorMap.Remove(owner_->syncInfo.entityid());
			}
			timelineTickActorMap.Add(owner_->syncInfo.entityid(), owner_);

		}
		else
		{
			if (timelineTickActorMap.Find(owner_->syncInfo.entityid()))
			{
				timelineTickActorMap.Remove(owner_->syncInfo.entityid());
			}
		}
	}
	else if (programme == 3)
	{
		auto& timelineTickActorList = owner_->editEntityManage->timeline->timelineTickActorList;
		UEditEntityManageTimelineComponent::TimelinneActorInfo timelinneActorInfo;
		timelinneActorInfo.actor = owner_;
		timelinneActorInfo.type = isRegister;
		timelinneActorInfo.entityId = owner_->syncInfo.entityid();
		timelineTickActorList.push_back(timelinneActorInfo);
	}
}

// 时间轴 手动拖拽时间轴UI使得整个发生变化->等同于编辑实例操作s
void UEditEntityTimelineComponent::MoveTimelineUI(bool isEdit, float nowTime)
{
	// isEdit true:编辑模式,不带有时间轴变换


	ASelectedViewActor* selectedViewActor = owner_->gameMode->selectedViewActor;
	if (selectedViewActor == nullptr)return;

	FTransform newWorldtransform;
	FTransform newRelativelyTransform;

	if (isEdit == false)
	{
		if (time_lenght == 0)
		{
			return;
		}
		newRelativelyTransform = GetActorTimelineTransform(nowTime);
		newWorldtransform = UCalculationObject::RelativeToWorldTransform(
			owner_->GetObserverTransform(), newRelativelyTransform);
	}
	else
	{
		newWorldtransform = owner_->GetObserverTransform();
	}
	owner_->editComponent->EntityLocalMoveMain(ESelectedWorkMode::WorldTransform, newWorldtransform.GetLocation(), newWorldtransform.GetRotation().Rotator(), newWorldtransform.GetScale3D());// 实例更新
	selectedViewActor->SetActorLocation(newWorldtransform.GetLocation());// 坐标轴
	selectedViewActor->controller->SetRelativelyRotation(newWorldtransform);// 坐标轴相对旋转

	//auto tempLocation = childComponentList[0]->meshWorldTransform.GetLocation();
	//UKismetSystemLibrary::DrawDebugBox(GetWorld(), newWorldtransform.GetLocation(), FVector(20, 20, 100), FLinearColor::Blue, FRotator::ZeroRotator, 10, 3);
	//UKismetSystemLibrary::DrawDebugBox(GetWorld(), tempLocation, FVector(20, 20, 100), FLinearColor::Red, FRotator::ZeroRotator, 10, 3);

	//FString str1 = owner_->GetActorLocation().ToString();

	//auto str1 = owner_->GetObserverTransform().GetRotation().Rotator();
	//auto str2 = newRelativelyTransform.GetRotation().Rotator();
	//auto str3 = newWorldtransform.GetRotation().Rotator();
	//auto str4 = owner_->childComponentList[0]->meshRelativelyTransform.GetRotation().Rotator();
	//auto str5 = owner_->childComponentList[0]->meshWorldTransform.GetRotation().Rotator();
	//UE_LOG(LogTemp, Log, TEXT("[%x] [网格] [MoveTimelineUI] [eintityId:%d] [time:%f] [实例世界原点旋转:%f--%f--%f]  [时间轴相对旋转:%f--%f--%f] [时间轴世界旋转:%f--%f--%f]  [实例相对旋转:%f--%f--%f]  [实例世界旋转:%f--%f--%f]"),
	//	this, owner_->entityId, nowTime,
	//	str1.Pitch, str1.Yaw, str1.Roll,
	//	str2.Pitch, str2.Yaw, str2.Roll,
	//	str3.Pitch, str3.Yaw, str3.Roll,
	//	str4.Pitch, str4.Yaw, str4.Roll,
	//	str5.Pitch, str5.Yaw, str5.Roll
	//);


	if (owner_->IsDynamicColorChange())
	{
		FVector color = GetActorTimelineColor(nowTime);
		owner_->ColorChange(color);
	}
}

// 时间轴总控 自动播放时间轴
void UEditEntityTimelineComponent::TimelineAutoPlay(bool isEdit, float nowTime)
{
	// isEdit true:编辑模式,不带有时间轴变换

	if (time_lenght == 0)return;

	if (isEdit)
	{
		MoveTimelineUI(true, 0);
	}
	else
	{
		ComponenntTick2(nowTime);
	}
}

// 时间轴总控 TimelineAutoPlay()->
void UEditEntityTimelineComponent::ComponenntTick2(float nowTime)
{
	float tempVar = nowTime / time_lenght;
	float loopTime = (tempVar - (int)tempVar) * time_lenght;

	PlayToTime(loopTime, nowTime);
}

// 播放指定时间轴位置 ComponenntTick2->
void UEditEntityTimelineComponent::PlayToTime(float loopTime, float nowTime)
{

	// 当前是否为关键帧或者接近关键帧
	int tempTime = loopTime * 1000;


	// 变换
	if (locationCurveVector != nullptr && rotationCurveVector != nullptr)
	{

		auto relativelyTransform = GetActorTimelineTransform(loopTime);
		FTransform newWorldtransform;
		bool moveFlag = false;
		if (relativelyTransform.GetLocation().Equals(FVector::ZeroVector) == false || relativelyTransform.GetRotation().Rotator().Equals(FRotator::ZeroRotator) == false)
		{
			newWorldtransform = UCalculationObject::RelativeToWorldTransform(
				owner_->GetObserverTransform(), relativelyTransform);
			newWorldtransform.SetScale3D(owner_->GetObserverTransform().GetScale3D());	// 带上缩放的世界原点,再进行第二次变换.
			moveFlag = true;
		}
		if (moveFlag)
		{
			for (auto i = 0; i < owner_->childComponentList.Num(); i++)
			{
				auto child = owner_->childComponentList[i];
				auto newMeshWorldTransform = UCalculationObject::RelativeToWorldTransform(
					newWorldtransform, child->meshInfo.rTransform);
				child->SetLocationAndRotator(newMeshWorldTransform);
				//UKismetSystemLibrary::DrawDebugBox(GetWorld(), transform.GetLocation(), FVector(30, 30, 30), FLinearColor::Blue, FRotator::ZeroRotator, 10, 3);
				//if (nowTime == 0) {
				//	FString str1 = relativelyTransform.GetLocation().ToString();
				//	FString str2 = newWorldtransform.GetLocation().ToString();
				//	FString str3 = child->meshRelativelyTransform.GetLocation().ToString();
				//	FString str4 = newMeshWorldTransform.GetLocation().ToString();
				//	UE_LOG(LogTemp, Log, TEXT("[%x] [网格] [MoveTimelineUI] [eintityId:%d] [时间轴相对位置%s] [actor世界位置%s] [网格相对位置%s] [网格世界位置%s]"),
				//		this, owner_->entityId, *str1, *str2, *str3, *str4);
				//}
			}
		}
	}

	// 颜色
	if (colorCurveVector != nullptr)
	{
		//if (lastTimeColorTick + 0.1 < nowTime) 
		{
			lastTimeColorTick = nowTime;
			if (owner_->IsDynamicColorChange())
			{
				FVector color = GetActorTimelineColor(loopTime);
				owner_->ColorChange(color);
			}
		}
	}

}

/*------------------------------------------------------------------*/

// 测试
void UEditEntityTimelineComponent::TestFunc()
{
	// 如果时xx就往sync里写入几行数据,然后让它一直自动播放好了.

	return;

	owner_->editEntityManage->tempCount++;
	if (owner_->editEntityManage->tempCount > 1800)
	{
		return;
	}

	//if (owner_->syncInfo.entityid() == 10065)
	{
		auto& point_map = *owner_->syncInfo.mutable_transform_timeline()->mutable_point_map();
		point_map.clear();

		for (int i = 0; i < 21; i++)
		{
			int time = (i) * 1000;
			FRotator rotator(0, 0, 0);
			FVector location(0, 0, 0);
			if (i < 10)
			{
				location = FVector(0, 0, i * 50);
			}
			else
			{
				auto temp_i = 20 - i;
				location = FVector(0, 0, temp_i * 50);
			}
			Gamedata::EntityTransformTimelinePoint entityTransformTimelinePoint;
			entityTransformTimelinePoint.set_uniquemark(i + 10);
			//UE_LOG(LogTemp, Log, TEXT("[唯一标记] [TestFunc] [entityId:%d] [time:%d] [uniquemark:%d]"), owner_->syncInfo.entityid(), time, i + 10);
			entityTransformTimelinePoint.mutable_location()->set_x(location.X);
			entityTransformTimelinePoint.mutable_location()->set_y(location.Y);
			entityTransformTimelinePoint.mutable_location()->set_z(location.Z);
			entityTransformTimelinePoint.mutable_rotator()->set_pitch(rotator.Pitch);
			entityTransformTimelinePoint.mutable_rotator()->set_roll(rotator.Roll);
			entityTransformTimelinePoint.mutable_rotator()->set_yaw(rotator.Yaw);
			point_map.insert({ time ,entityTransformTimelinePoint });
		}
		//UE_LOG(LogTemp, Log, TEXT("[唯一标记] [TestFunc] [entityId:%d] [size:%d]"), owner_->syncInfo.entityid(), point_map.size());
	}
}

// [测试放弃] tick 
void UEditEntityTimelineComponent::FunctionTick(float time)
{
	float tempVar = time / time_lenght;
	float loopTime = (tempVar - (int)tempVar) * time_lenght;
	auto transform = GetActorTimelineTransform(loopTime);
	//UE_LOG(LogTemp, Log, TEXT("[%x] [我的时间轴] [Start] [nowTime:%f] [tempVar:%f] [loopTime:%f] [location:%f %f %f]"), this, time, tempVar, loopTime, transform.GetLocation().X, transform.GetLocation().Y, transform.GetLocation().Z);
	owner_->editComponent->EntityLocalMoveMain(ESelectedWorkMode::RelativelyTransform, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D());
}

// [测试放弃] tick 
void UEditEntityTimelineComponent::ComponenntTick(const ::google::protobuf::RepeatedPtrField< ::Gamedata::FTransform_s >& child_transform)
{
	if (child_transform.size() != owner_->childComponentList.Num())
	{
		return;
	}

	for (auto i = 0; i < owner_->childComponentList.Num(); i++)
	{
		auto child = owner_->childComponentList[i];
		auto& transform = child_transform[i];
		//child->SetLocationAndRotator(&transform);
	}
}

// [测试放弃] tick3 
void UEditEntityTimelineComponent::ComponenntTick3(std::vector< FTransform >& child_transform)
{
	if (child_transform.size() != owner_->childComponentList.Num())
	{
		return;
	}

	for (auto i = 0; i < owner_->childComponentList.Num(); i++)
	{
		auto child = owner_->childComponentList[i];
		auto& transform = child_transform[i];
		child->SetLocationAndRotator(transform);
	}
	// 设置actor位置
}
