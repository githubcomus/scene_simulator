// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "Yxjs/FunctionalModule/Selected/SelectedInfo.h"
#include <Components/TimelineComponent.h>
#include "EditEntityActor.generated.h"

class UEditEntityManageData;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UPlaceViewImageComponent;
class UPlaceViewMeshComponent;
class UPlaceViewCollisionComponent;
class UPlaceControllerLightComponent;
class UPlaceViewLaserBeamComponent;
class APlaceNearActorView;
class UEditEntityChildComponent;
class AEditEntityManage;
class ABaseGameMode;
class ABaseHUD;
class UEditEntityTimelineComponent;
class UEditEntityEditComponent;
class UEditEntityCommandComponent;

class UBoxComponent;
class UMaterial;
class USceneComponent;
class UArrowComponent;
class USceneComponent;
class UStaticMeshComponent;
class UMaterialInstanceConstant;
class UImage;

UCLASS()
class AEditEntityActor : public AActor
{
	GENERATED_BODY()

public:

	//
	TWeakObjectPtr< AEditEntityManage> editEntityManage;

	/*----------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyData")
		USceneComponent* sceneRoot;

	// 时间轴
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyData")
		UEditEntityTimelineComponent* myTimelineComponent;

	// 子组件列表
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyData")
		TArray<UEditEntityChildComponent*> childComponentList;

	// 编辑组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyData")
		UEditEntityEditComponent* editComponent;

	// 命令控制组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyData")
		UEditEntityCommandComponent* commandComponent;

	/*----------------------------------------------------*/

	// 是否正确初始化
	bool isInit = false;

	// 网络(本地模拟操作)同步数据
	Gamedata::EntityInfo syncInfo;

	// 表格数据
	xlsdata::PlaceTableDefine* xlsInfo = nullptr;

	//
	int entityId = 0;

	// 是否选中
	bool isSelect = false;

	/*----------------------------------------------------*/

	// 网络同步数据备份
	Gamedata::EntityInfo observerSyncInfo;

	/*----------------------------------------------------*/

	// 常用指针
	TWeakObjectPtr<UBaseGameInstance> gameInstance;

	// 常用指针
	TWeakObjectPtr<APlayerController> playerController;

	// 常用指针
	TWeakObjectPtr<ABaseGameMode> gameMode;

	// 常用指针
	TWeakObjectPtr<ABaseHUD> hud;

	/*------------------------------------------------------------------*/

	xlsdata::PlaceTableDefine* GetChildMeshXlsInfo(int index);
	xlsdata::MaterialTableDefine* GetChildMaterialXlsInfo(int index);
	bool CheckDefaultData();
	xlsdata::PlaceTableDefine* GetXlsInfo();
	AEditEntityActor();

	void Start(Gamedata::EntityInfo* syncInfo_);

	void End();

	void EntitySetNewSyncInfo(Gamedata::EntityInfo* newSyncInfo);
	void ReplaceMesh();
	bool IsUseTimeline();

	FTransform GetNowTransform();
	FTransform GetObserverTransform();
	bool IsTimelineAuto();
	bool IsTimelineCommandText();
	bool IsDynamicColorChange();
	FString GetMyName();
	void CreateSelectMesh();
	void DestroySelectMesh();
	void ShowCollisionMeshComponent(bool show, int32 selectIndex);
	void ColorChange(FVector color);
	void ChangeHighlight();
	void CreateChildMesh(int index);
	void DestroyChildMesh(int index);
	void UpdateChildMesh(int index);
	void UpdateChildMesh();
	void ReplaceChildMesh(int index);
};
