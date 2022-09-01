// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include <string>
#include <Components/TimelineComponent.h>
#include "EditEntityCommandComponent.generated.h"

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
class  UEditEntityCommandComponent : public UObject
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr< AEditEntityManage> editEntityManage;

	//
	TWeakObjectPtr< AEditEntityActor> owner_;

	/*------------------------------------------------------------------*/

	// 执行任务的entity数据
	struct CommandJobInfo
	{

		//
		int entityId = 0;

		// 游戏内时间
		float startTime = 0;

		// 游戏内时间
		float endTime = 0;

		// 该行命令数据
		Gamedata::EntityCommandCommand entityCommandCommand;

		//
		//TWeakObjectPtr<AEditEntityActor> actor;
	};

	CommandJobInfo commandJobInfo;

	/*------------------------------------------------------------------*/

	UEditEntityCommandComponent();
	void Start(AEditEntityActor* view);
	void End();
	void RegisterToMaster();
	void RemoveFromMaster();
	void UpdateToMaster();
	void PlayAnim(std::string command);
	bool EntityRunFrame(float nowTime);
	bool RegisterJob(std::string commandStr);
	//void RemoveJob(int entityId);
};
