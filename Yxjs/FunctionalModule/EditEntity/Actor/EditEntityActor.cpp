// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityActor.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewMeshComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewImageComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewCollisionComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewLaserLightActor.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewLaserBeamComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/Actor/ViewComponent/ImageUserWidget.h"
#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/NearActor/PlaceNearActorView.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshView.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshComponent.h"

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Yxjs/Tools/CalculationObject.h"
#include "ControllerComponent/EditEntityTimelineComponent.h"
#include "ControllerComponent/EditEntityChildComponent.h"
#include "../../Selected/SelectedViewActor.h"
#include "ControllerComponent/EditEntityEditComponent.h"
#include "ControllerComponent/EditEntityCommandComponent.h"
#include "ViewComponent/EditEntityColorMeshComponent.h"

/*
view
*/

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

// 检查必须的数据
bool AEditEntityActor::CheckDefaultData()
{
	bool checkTwice = false;
	if (gameInstance.IsValid() == false || gameMode.IsValid() == false
		|| playerController.IsValid() == false || hud.IsValid() == false)
	{
		gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController.IsValid())hud = Cast<ABaseHUD>(playerController->GetHUD());

		checkTwice = true;
	}

	bool flag = true;
	if (checkTwice)
	{
		if (gameInstance.IsValid() == false || gameMode.IsValid() == false
			|| playerController.IsValid() == false || hud.IsValid() == false)
		{
			flag = false;
		}
	}

	return flag;
}

// 获取 xlsInfo
xlsdata::PlaceTableDefine* AEditEntityActor::GetXlsInfo()
{
	return xlsInfo;
}

// 获取子表格数据
xlsdata::PlaceTableDefine* AEditEntityActor::GetChildMeshXlsInfo(int index)
{
	auto tempXlsInfo = editEntityManage->data->GetXlsInfoWithXlsId(syncInfo.child_list(index).mesh().xls_id());
	return tempXlsInfo;
}

// 获取子表格数据
xlsdata::MaterialTableDefine* AEditEntityActor::GetChildMaterialXlsInfo(int index)
{
	auto tempXlsInfo = editEntityManage->data->GetMaterialInfoWithId(syncInfo.child_list(index).mesh().material_id());
	if (tempXlsInfo == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [错误材质] [GetChildMaterialXlsInfo] [material_id:%d]"), this,
			syncInfo.child_list(index).mesh().material_id());
		tempXlsInfo = editEntityManage->data->GetMaterialInfoWithId(0);
	}
	return tempXlsInfo;
}

// 是否启用了时间轴
bool AEditEntityActor::IsUseTimeline()
{
	if (xlsInfo->UseTimeline_ == false)return false;
	// 对比版本
	if (syncInfo.has_transform_timeline() && syncInfo.mutable_transform_timeline()->point_map_size() > 0)
	{
		return true;
	}
	return false;
}

// 返回当前变换
FTransform AEditEntityActor::GetNowTransform()
{

	auto tTransform = UCommonlyObject::PBToUe4Transform(syncInfo.mutable_transform());
	return tTransform;
}

// 返回备份变换
FTransform AEditEntityActor::GetObserverTransform()
{
	return  UCommonlyObject::PBToUe4Transform(observerSyncInfo.mutable_transform());
}

// 是否支持时间轴自动运行
bool AEditEntityActor::IsTimelineAuto()
{
	if (syncInfo.has_transform_timeline() && syncInfo.mutable_transform_timeline()->point_map_size() > 0)
	{
		if (syncInfo.has_command() == false || (syncInfo.has_command() && syncInfo.command().type() == Gamedata::EntityCommandType::EntityCommandType_auto))
		{
			return true;
		}
	}
	return false;
}

// 是否支持时间轴命令文本控制
bool AEditEntityActor::IsTimelineCommandText()
{
	if (syncInfo.has_command() && syncInfo.command().type() == Gamedata::EntityCommandType::EntityCommandType_command)
	{
		return true;
	}
	return false;
}

// 是否支持动态颜色变化
bool AEditEntityActor::IsDynamicColorChange()
{

	EMyMeshMaterialType newType = editEntityManage->data->GetChildMaterialType(&syncInfo, 0);
	if (
		newType == EMyMeshMaterialType::EMyMeshMaterialType_DynamicHighhight
		|| newType == EMyMeshMaterialType::EMyMeshMaterialType_DynamicNormalColor
		)
	{
		return true;
	}

	return false;
}

// 返回名字
FString AEditEntityActor::GetMyName()
{
	FString name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());
	FString name2 = FString::Printf(TEXT("%s_%d"), *name, entityId);
	return name2;
}

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
AEditEntityActor::AEditEntityActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	sceneRoot->PrimaryComponentTick.bCanEverTick = false;
	RootComponent = sceneRoot;
	sceneRoot->SetShouldUpdatePhysicsVolume(false);

	myTimelineComponent = NewObject<UEditEntityTimelineComponent>();
	editComponent = NewObject<UEditEntityEditComponent>();
	commandComponent = NewObject<UEditEntityCommandComponent>();
}

// START
void AEditEntityActor::Start(Gamedata::EntityInfo* syncInfo_)
{
	CheckDefaultData();
	editEntityManage = Cast<AEditEntityManage>(GetOwner());

	syncInfo.CopyFrom(*syncInfo_);
	entityId = syncInfo.entityid();
	xlsInfo = editEntityManage->data->GetXlsInfoWithXlsId(syncInfo.child_list(0).mesh().xls_id());
	observerSyncInfo.CopyFrom(syncInfo);
	GetObserverTransform() = UCommonlyObject::PBToUe4Transform(observerSyncInfo.mutable_transform());

	if (xlsInfo == nullptr)return;

	editComponent->Start(this); //

	// 子网格
	for (auto i = 0; i < syncInfo.child_list_size(); i++)
	{
		CreateChildMesh(i);
	}
	myTimelineComponent->Start(this);
	commandComponent->Start(this);
	isInit = true;
}

// END
void AEditEntityActor::End()
{
	editComponent->End();
	myTimelineComponent->End();
	commandComponent->End();

	for (auto i = 0; i < syncInfo.child_list_size(); i++)
	{
		DestroyChildMesh(i);
	}
	childComponentList.Reset();
}

// 子组件 创建
void AEditEntityActor::CreateChildMesh(int index)
{
	FString name = FString::Printf(TEXT("child_%d"), index);
	auto component = NewObject<UEditEntityChildComponent>(this, *name);
	childComponentList.Add(component);

	component->Start(this, index);
}

// 子组件 销毁子组件
void AEditEntityActor::DestroyChildMesh(int index)
{
	if (childComponentList.Num() > index)
	{
		childComponentList[index]->End();
	}
}

// 子组件 更新
void AEditEntityActor::UpdateChildMesh(int index)
{
	if (childComponentList.Num() > index)
	{
		childComponentList[index]->EntityUpdate();
	}
}

// 子组件 替换网格
void AEditEntityActor::ReplaceChildMesh(int index)
{
	if (childComponentList.Num() > index)
	{
		childComponentList[index]->ReplaceMesh();
	}
}

// 子组件 替换网格
void AEditEntityActor::ReplaceMesh()
{

	UE_LOG(LogTemp, Log, TEXT("[%x] [更变材质] [ReplaceMesh] [entityId:%d]"), this,
		syncInfo.child_list(0).mesh().material_id());

	for (auto i = 0; i < syncInfo.child_list_size(); i++)
	{
		UpdateChildMesh(i);
		ReplaceChildMesh(i);
	}
}

// 子组件 更新
void AEditEntityActor::UpdateChildMesh()
{

	for (auto i = 0; i < syncInfo.child_list_size(); i++)
	{
		UpdateChildMesh(i);
	}
}

// 选中 创建
void AEditEntityActor::CreateSelectMesh()
{
	isSelect = true;

	if (childComponentList.Num() > 0)
	{
		childComponentList[0]->CreateSelectMesh();
	}
}

// 选中 移除
void AEditEntityActor::DestroySelectMesh()
{
	isSelect = false;

	if (childComponentList.Num() > 0)
	{
		childComponentList[0]->DestroySelectMesh();
	}
}

// 选中 严谨检测  点击
void AEditEntityActor::ShowCollisionMeshComponent(bool show, int32 selectIndex)
{
	if (childComponentList.Num() > selectIndex)
	{
		childComponentList[selectIndex]->ShowCollisionMeshComponent(show);
	}
}

// 颜色 更变颜色
void AEditEntityActor::ColorChange(FVector color)
{

	if (IsDynamicColorChange() == false)return;
	for (auto node : childComponentList)
	{
		if (node->colorMeshComponent)
		{
			node->colorMeshComponent->ChangeMaterialsInstanceValue(color);
		}
		if (node->selectMeshComponent)
		{
			node->selectMeshComponent->ChangeMaterialsInstanceValue(color);
		}
	}
}

// 颜色 切换颜色/高光
void AEditEntityActor::ChangeHighlight()
{

	// 切近高光

	// 切出高光

	// 高光参数更变

	for (auto node : childComponentList)
	{
		if (node->colorMeshComponent)
		{
			node->colorMeshComponent->ChangeHighlight();
		}
	}
}

// 模块数据 更新数据
void AEditEntityActor::EntitySetNewSyncInfo(Gamedata::EntityInfo* newSyncInfo)
{
	syncInfo.CopyFrom(*newSyncInfo);
	observerSyncInfo.CopyFrom(*newSyncInfo);

	GetObserverTransform() = UCommonlyObject::PBToUe4Transform(observerSyncInfo.mutable_transform());
}
