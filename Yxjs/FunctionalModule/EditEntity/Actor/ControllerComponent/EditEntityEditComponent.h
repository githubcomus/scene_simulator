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
#include "EditEntityEditComponent.generated.h"

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
class  UEditEntityEditComponent : public UObject
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr< AEditEntityManage> editEntityManage;

	//
	TWeakObjectPtr< AEditEntityActor> owner_;

	/*------------------------------------------------------------------*/

	/*------------------------------------------------------------------*/

	UEditEntityEditComponent();
	void Start(AEditEntityActor* view);
	void End();
	//void EntitySyncInfoToLocalInfo(bool isInit_);
	//void SingleMeshAddSigleLine(bool isInit_, FLocalEntityInfo* localInfo_);
	//void MultipleMeshSigleLine(bool isInit_, int index, FLocalEntityInfo* localInfo_, Gamedata::EntityMeshChildInfo* entityMeshChildInfo, xlsdata::PlaceTableDefine* childXlsInfo);
	void EntityLocalMoveMain(ESelectedWorkMode selectedWorkMode, FVector newLocation, FRotator newRotator, FVector newScale);
	void EntityLocalMoveLocation(FVector newLocation);
	void EntityLocalMoveSelfRotation(FRotator newRotator);
	void EntityLocalMoveAllRotation(FVector newLocation, FRotator newRotator);
	void EntityLocalMoveScale(FVector newScale);
	void EntityUpdateView();
};
