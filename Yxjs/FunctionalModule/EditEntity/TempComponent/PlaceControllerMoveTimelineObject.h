// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include <string>
#include "UI/BaseHud.h"
#include "PlaceControllerMoveTimelineObject.generated.h"

/**
 *
 */
class ABaseGameMode;
class AEditEntityManage;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UEditEntityManageData;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UBoxComponent;
//class UPlaceStaticMeshComponent;
class UImage;

UCLASS()
class UPlaceControllerMoveTimelineObject : public UObject
{
	GENERATED_BODY()

public:

	// tick info
	struct CvTickInfo
	{
		// 最后执行时间
		float lastTime = 0;

		// 统计时间
		float countTime = 0;

		// 统计次数
		int functionCount = 0;

		//
		float lastTimeTestFunc = 0;
	};

	/*------------------------------------------------------------------*/

	// 视图类
	// AEditEntityManage* view;
	TWeakObjectPtr<UEditEntityManageController> controller;

	// tick
	CvTickInfo tickInfo;

	/*------------------------------------------------------------------*/

	UPlaceControllerMoveTimelineObject();

	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TickComponent(float DeltaTime);

	/*------------------------------------------------------------------*/

	//
	void Start(TWeakObjectPtr<UEditEntityManageController> controller_);

	//
	void End();
	void MackFLocalEntityInfo(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void GetLaserRealTransform(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	ABaseHUD* GetHUD();
	void TickMove();
	void TimelineController();
	void LocalPreviewMode(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	bool SetAllEntityTimelineFrame(float time);
	void SetPreviewData(int32 entityId, float startTime, float speed, int playIndex);
	void SetEntityLocalTempLocationOrRotator(int32 entityId, bool isMove, FVector move);
	void InitTimeline(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	int WriteTimelineHeadData(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void WriteTimelineLightData(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, FVector move,
		float enterTime, int timelineIndex);
	void WriteTimelineKeyList(int timelineIndex, Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void TestFunc();
	float GetInsertFrameSmoothValue(float start, float end, float time);
	bool SetCurrentPlayLocationAndRotator(int timelineIndex, int nowPlayTime, Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	/*------------------------------------------------------------------*/
};
