// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectedInfo.h"
#include <string>
#include "SelectedDataComponent.generated.h"

/**
 *
 */
class ASelectedViewActor;
class USelectedControllerComponent;
class UPlayerRangedWeaponComponent;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  USelectedDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 视图类
	ASelectedViewActor* view;

	// 控制类
	USelectedControllerComponent* controller;

	// 当前工作模式
	ESelectedWorkMode selectedWorkMode;

	// 当前高亮的坐标轴(0:none , 1:x , 2:y , 3:z)
	ESelectedMoveStrategy changeAxisArrowColorType;

	// 视图缩放比
	float scale;

	// 是否选中物品
	bool isSelected;

	/*------------------------------------------------------------------*/

	//
	USelectedDataComponent();

	/*------------------------------------------------------------------*/

	//
	void Start(USelectedControllerComponent* controller_, ASelectedViewActor* view_);

	//
	void End();

	/*------------------------------------------------------------------*/
};
