// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "EditEntityManage.generated.h"

class UEditEntityManageData;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UPlaceViewImageComponent;
class UPlaceViewMeshComponent;
class UPlaceViewCollisionComponent;
class UPlaceControllerLightComponent;
class UPlaceViewLaserBeamComponent;
class APlaceNearActorView;
class AMyPythonActor;
class UEditEntityManageTimelineComponent;
class UEditEntityManageCommandComponent;
class UScanMesh;
class UEditEntityResourceIcon;

class UBoxComponent;
class UMaterial;
class USceneComponent;
class UArrowComponent;
class USceneComponent;
class UStaticMeshComponent;
class UMaterialInstanceConstant;
class UImage;

UCLASS()
class AEditEntityManage : public AActor
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	// 数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UEditEntityManageData* data;

	// 控制器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UEditEntityManageController* controller;

	// 时间轴控制器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UEditEntityManageTimelineComponent* timeline;

	// 命令控制器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UEditEntityManageCommandComponent* command;

	// 近处摆放物体
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		APlaceNearActorView* placeNearActorView;

	// 生成的实例
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<int, AEditEntityActor*> actorMap;

	// python控制器
	UPROPERTY()
		AMyPythonActor* myPythonActor = nullptr;

	// 网格数据扫描
	UPROPERTY()
		UScanMesh* scanMesh = nullptr;

	// 资源缩略图
	UPROPERTY()
		UEditEntityResourceIcon* resourceIcon = nullptr;

	// 临时统计数据
	int tempCount = 0;

	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	AEditEntityManage();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// EndPlay
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// PostInitializeComponents
	virtual void PostInitializeComponents() override;

	/*------------------------------------------------------------------*/

	// START
	void Start();

	// END
	void End();

	/*------------------------------------------------------------------*/

	// 新增数据
	void EntityAdd(Gamedata::EntityInfo* syncInfo);

	// 移除数据
	void EntityRemove(int32 entityId);

};
