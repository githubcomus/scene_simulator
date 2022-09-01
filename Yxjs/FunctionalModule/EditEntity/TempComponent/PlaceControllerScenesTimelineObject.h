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
#include "PlaceControllerScenesTimelineObject.generated.h"

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
class UPlaceControllerScenesTimelineObject : public UObject
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

	//
	TWeakObjectPtr<UEditEntityManageController> controller;

	// tick
	CvTickInfo tickInfo;

	/*------------------------------------------------------------------*/

	UPlaceControllerScenesTimelineObject();

	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TickComponent(float DeltaTime);

	/*------------------------------------------------------------------*/

	//
	void Start(TWeakObjectPtr<UEditEntityManageController> controller_);

	//
	void End();
	void MackFLocalEntityInfo(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void TickMove();
	void TimelineController();
	void TestFunc();

	void NetCall_EntityOperation(Gamedata::EntityOperationTotal& entityOperation);
	void NetCallback_EntityOperation(Gamedata::EntityOperationTotal& entityOperation);
	void Function_SetPlay(Gamedata::EntityOperationAlone* node);
	/*------------------------------------------------------------------*/
};
