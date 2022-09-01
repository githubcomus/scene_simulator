// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include <string>
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "PlaceViewImageComponent.generated.h"

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
class UImage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlaceViewImageComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 视图类
	AEditEntityManage* view;

	/*------------------------------------------------------------------*/

	//
	UPlaceViewImageComponent();

	/*------------------------------------------------------------------*/

	//
	void Start(AEditEntityManage* view_);

	//
	void End();

	/*------------------------------------------------------------------*/

	void CreateImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void UpdateImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void DestroyImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void CallUIOverSetImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, UImage* image, TWeakObjectPtr<UTexture2D> texture2D);
	void CallUIReadySetImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void MeshLoadMaterial(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);


};
