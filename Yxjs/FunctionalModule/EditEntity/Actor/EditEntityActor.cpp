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
// ??????
/*/////////////////////////////////////////////////////////////*/

// ?????????????????????
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

// ?????? xlsInfo
xlsdata::PlaceTableDefine* AEditEntityActor::GetXlsInfo()
{
	return xlsInfo;
}

// ?????????????????????
xlsdata::PlaceTableDefine* AEditEntityActor::GetChildMeshXlsInfo(int index)
{
	auto tempXlsInfo = editEntityManage->data->GetXlsInfoWithXlsId(syncInfo.child_list(index).mesh().xls_id());
	return tempXlsInfo;
}

// ?????????????????????
xlsdata::MaterialTableDefine* AEditEntityActor::GetChildMaterialXlsInfo(int index)
{
	auto tempXlsInfo = editEntityManage->data->GetMaterialInfoWithId(syncInfo.child_list(index).mesh().material_id());
	if (tempXlsInfo == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [GetChildMaterialXlsInfo] [material_id:%d]"), this,
			syncInfo.child_list(index).mesh().material_id());
		tempXlsInfo = editEntityManage->data->GetMaterialInfoWithId(0);
	}
	return tempXlsInfo;
}

// ????????????????????????
bool AEditEntityActor::IsUseTimeline()
{
	if (xlsInfo->UseTimeline_ == false)return false;
	// ????????????
	if (syncInfo.has_transform_timeline() && syncInfo.mutable_transform_timeline()->point_map_size() > 0)
	{
		return true;
	}
	return false;
}

// ??????????????????
FTransform AEditEntityActor::GetNowTransform()
{

	auto tTransform = UCommonlyObject::PBToUe4Transform(syncInfo.mutable_transform());
	return tTransform;
}

// ??????????????????
FTransform AEditEntityActor::GetObserverTransform()
{
	return  UCommonlyObject::PBToUe4Transform(observerSyncInfo.mutable_transform());
}

// ?????????????????????????????????
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

// ???????????????????????????????????????
bool AEditEntityActor::IsTimelineCommandText()
{
	if (syncInfo.has_command() && syncInfo.command().type() == Gamedata::EntityCommandType::EntityCommandType_command)
	{
		return true;
	}
	return false;
}

// ??????????????????????????????
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

// ????????????
FString AEditEntityActor::GetMyName()
{
	FString name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());
	FString name2 = FString::Printf(TEXT("%s_%d"), *name, entityId);
	return name2;
}

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// ?????????
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

	// ?????????
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

// ????????? ??????
void AEditEntityActor::CreateChildMesh(int index)
{
	FString name = FString::Printf(TEXT("child_%d"), index);
	auto component = NewObject<UEditEntityChildComponent>(this, *name);
	childComponentList.Add(component);

	component->Start(this, index);
}

// ????????? ???????????????
void AEditEntityActor::DestroyChildMesh(int index)
{
	if (childComponentList.Num() > index)
	{
		childComponentList[index]->End();
	}
}

// ????????? ??????
void AEditEntityActor::UpdateChildMesh(int index)
{
	if (childComponentList.Num() > index)
	{
		childComponentList[index]->EntityUpdate();
	}
}

// ????????? ????????????
void AEditEntityActor::ReplaceChildMesh(int index)
{
	if (childComponentList.Num() > index)
	{
		childComponentList[index]->ReplaceMesh();
	}
}

// ????????? ????????????
void AEditEntityActor::ReplaceMesh()
{

	UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [ReplaceMesh] [entityId:%d]"), this,
		syncInfo.child_list(0).mesh().material_id());

	for (auto i = 0; i < syncInfo.child_list_size(); i++)
	{
		UpdateChildMesh(i);
		ReplaceChildMesh(i);
	}
}

// ????????? ??????
void AEditEntityActor::UpdateChildMesh()
{

	for (auto i = 0; i < syncInfo.child_list_size(); i++)
	{
		UpdateChildMesh(i);
	}
}

// ?????? ??????
void AEditEntityActor::CreateSelectMesh()
{
	isSelect = true;

	if (childComponentList.Num() > 0)
	{
		childComponentList[0]->CreateSelectMesh();
	}
}

// ?????? ??????
void AEditEntityActor::DestroySelectMesh()
{
	isSelect = false;

	if (childComponentList.Num() > 0)
	{
		childComponentList[0]->DestroySelectMesh();
	}
}

// ?????? ????????????  ??????
void AEditEntityActor::ShowCollisionMeshComponent(bool show, int32 selectIndex)
{
	if (childComponentList.Num() > selectIndex)
	{
		childComponentList[selectIndex]->ShowCollisionMeshComponent(show);
	}
}

// ?????? ????????????
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

// ?????? ????????????/??????
void AEditEntityActor::ChangeHighlight()
{

	// ????????????

	// ????????????

	// ??????????????????

	for (auto node : childComponentList)
	{
		if (node->colorMeshComponent)
		{
			node->colorMeshComponent->ChangeHighlight();
		}
	}
}

// ???????????? ????????????
void AEditEntityActor::EntitySetNewSyncInfo(Gamedata::EntityInfo* newSyncInfo)
{
	syncInfo.CopyFrom(*newSyncInfo);
	observerSyncInfo.CopyFrom(*newSyncInfo);

	GetObserverTransform() = UCommonlyObject::PBToUe4Transform(observerSyncInfo.mutable_transform());
}
