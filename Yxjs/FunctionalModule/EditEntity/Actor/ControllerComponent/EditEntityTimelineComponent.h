// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "UI/Game/EditEntity/Timeline/TimelineInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include <string>
#include <Components/TimelineComponent.h>
#include "EditEntityTimelineComponent.generated.h"

/**
 *
 */
class ABaseGameMode;
class AEditEntityManage;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UEditEntityManageData;
class AEditEntityActor;
class UEditEntityManageTimelineComponent;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UBoxComponent;
class UImage;
class UStaticMeshComponent;
class UTimelineComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UEditEntityTimelineComponent : public UObject
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr< AEditEntityManage> editEntityManage;

	//
	TWeakObjectPtr< AEditEntityActor> owner_;

	/*------------------------------------------------------------------*/

	UPROPERTY()
		UCurveFloat* curveFloat;

	UPROPERTY()
		UCurveVector* locationCurveVector;

	UPROPERTY()
		UCurveVector* rotationCurveVector;

	UPROPERTY()
		UCurveVector* colorCurveVector;

	struct FKeyHandleInfo
	{
		int jobId = 0;
		int time = 0;
		FKeyHandle handle_x;
		FKeyHandle handle_y;
		FKeyHandle handle_z;
		FKeyHandle handle_roll;
		FKeyHandle handle_pitch;
		FKeyHandle handle_yaw;
		FKeyHandle handle_r;
		FKeyHandle handle_g;
		FKeyHandle handle_b;
	};
	// key:jobid value:
	TMap<int32, FKeyHandleInfo> keyInfoMap;

	//
	int jobIdNumber = 1000;

	// 时间轴长度
	float time_lenght = 0;

	// 上一次tick时间
	float lastTimeColorTick = 0;

	/*------------------------------------------------------------------*/

	UEditEntityTimelineComponent();

	void Start(AEditEntityActor* view);

	void End();
	void AddKeyFrame(int time, FVector location, FRotator rotator, FVector color);
	void RemoveKeyFrame(int jobId);
	void UpdateKeyFrame(int jobId, int time, FVector location, FRotator rotator, FVector color);
	FTransform GetActorTimelineTransform(float time);
	FVector GetActorTimelineColor(float time);
	void GetActorTimelineTransform2(float time, FVector& location, FVector& rotator, FVector& color);
	void GetChildTimelineTransform(FTransform relativelyTransform, TimelineOneFrameOneEntityData& oneFrameOneEntityData);
	void GetChildTimelineTransform(float time, TimelineOneFrameOneEntityData& oneFrameOneEntityData);
	bool IsTimelineUpdate(Gamedata::EntityInfo* newSyncInfo);
	void GetNowTransform(bool isRelativaly, float nowTime, FVector& location, FRotator& rotator, FVector& color);
	void FunctionTick(float time);
	void ComponenntTick(const ::google::protobuf::RepeatedPtrField< ::Gamedata::FTransform_s >& child_transform);
	void ComponenntTick2(float nowTime);
	void PlayToTime(float loopTime, float nowTime);
	//void ComponenntTick3(const ::google::protobuf::RepeatedPtrField< ::Gamedata::FTransform_s >& child_transform);
	void ComponenntTick3(std::vector< FTransform >& child_transform);
	//void SetEditStartTransform();
	void RegisterTick(bool isRegister);
	void UpdateTimeline();
	void TestFunc();
	void MoveTimelineUI(bool isEdit, float nowTime);
	void TimelineAutoPlay(bool isEdit, float nowTime);

	void PointDeleteData(FTimelineEnterPointInfo* timelinePointInfo, FTimelinePointIndex& selectInfo);
	void MoveEntityData(FTimelineEnterPointInfo* timelinePointInfo, int newTime, FVector2D move);
	void UpdateEntityDataTransform(FTimelineEnterPointInfo* timelinePointInfo, int entityId, FTimelinePointIndex& pointIndex, CvTimelineUIInfo& info);
	void EntityAddPointData(int entityId, float time, ETimelinePointType timelinePointType, FVector location,
		FRotator rotation, FVector color);
};
