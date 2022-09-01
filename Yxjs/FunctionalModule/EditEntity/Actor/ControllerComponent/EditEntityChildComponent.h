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
#include "Tools/StringRingBuffer.h"
#include "EditEntityChildComponent.generated.h"

/**
 *
 */
class ABaseGameMode;
class AEditEntityManage;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UEditEntityManageData;
class AEditEntityActor;
class UEditEntityColorMeshComponent;
class UEditEntitySelectMeshComponent;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UBoxComponent;
class UImage;
class UStaticMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UEditEntityChildComponent : public UObject
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/
	// 管理器

	// manager
	TWeakObjectPtr< AEditEntityManage> editEntityManage;

	// acotr
	TWeakObjectPtr< AEditEntityActor> owner_;

	/*------------------------------------------------------------------*/
	// 组件

	// 碰撞合体
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* boxComponent;

	// 选中网格 编辑模式选中网格,边缘发光
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UEditEntitySelectMeshComponent* selectMeshComponent;

	// 颜色网格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UEditEntityColorMeshComponent* colorMeshComponent;

	// 点击检测网格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* collisionMeshComponent = nullptr;

	/*------------------------------------------------------------------*/

	// 所在组件
	int index = 0;

	// 子表格数据
	xlsdata::PlaceTableDefine* childMeshXlsInfo = nullptr;

	// 子表格数据
	xlsdata::MaterialTableDefine* childMaterialXlsInfo = nullptr;

	// 子组件同步数据
	Gamedata::EntityChildInfo* childSyncInfo = nullptr;

	// 用于选中精确碰撞的静态网格

	// mesh实例Id
	int meshIstanceId = 0;

	/*------------------------------------------------------------------*/
	// 本地数据,每次更新会更变

	// sync->local->view 中间数据
	FLocalCollisionInfo collisionInfo;

	// sync->local->view 中间数据
	FLocalMeshInfo meshInfo;

	/*------------------------------------------------------------------*/

	FString GetMeshName();
	ECollisionEnabled::Type GetCollisionType();
	UStaticMesh* GetMesh();

	void CalculateLocalInfo();
	//
	UEditEntityChildComponent();

	//
	void Start(AEditEntityActor* view, int index_);

	//
	void End();

	void EntityUpdate();
	void ReplaceMesh();
	void SetLocationAndRotator(FTransform& newTransform);
	void SetEditStartTransform();
	void ChildCreate(bool hide);
	void ChildUpdate(FTransform& newTransform);
	void ChildDestroy(bool thisFrame);
	void CreateMeshInstance(bool hide);
	void DestroyMeshInstance(bool thisFrame);
	void CreateDynamicColorMesh(bool hide);
	void DestroyDynamicColorMesh();
	void CreateSelectMesh();
	void DestroySelectMesh();
	void BoxCreate();
	void BoxCallback(bool isStart, FTransform rTransform);
	void BoxUpdate();
	void BoxDestroy();
	void ShowCollisionMeshComponent(bool show);
	bool CreateCollisionMeshComponent();
	void DestroyCollisonMeshComponent();
};
