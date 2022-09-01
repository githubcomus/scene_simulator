// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityEditComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Core/BaseGameInstance.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshView.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshComponent.h"
#include "../../Timeline/EditEntityManageTimelineComponent.h"
#include "EditEntityChildComponent.h"
#include "Yxjs/Tools/CalculationObject.h"
#include "../../../Selected/SelectedViewActor.h"
#include "../../../SceneStaticMesh/SceneStaticMeshView.h"

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
#include <Curves/CurveVector.h>

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
UEditEntityEditComponent::UEditEntityEditComponent()
{

}

// BeginPlay
void UEditEntityEditComponent::Start(AEditEntityActor* view)
{
	owner_ = view;
	if (owner_->gameMode->sceneStaticMeshView == nullptr)return;
	editEntityManage = owner_->editEntityManage;
}

// EndPlay
void UEditEntityEditComponent::End()
{
	// 没有start却end将引发这个奔溃
	if (owner_.IsValid() == false || owner_->gameMode->sceneStaticMeshView == nullptr)return;
}

// 实例编辑 操作
void UEditEntityEditComponent::EntityLocalMoveMain(ESelectedWorkMode selectedWorkMode, FVector newLocation, FRotator newRotator, FVector newScale)
{
	if (newLocation.ContainsNaN()
		|| newRotator.ContainsNaN()
		|| newScale.ContainsNaN()
		)
	{
		return;
	}



	if (selectedWorkMode == ESelectedWorkMode::WorldLocation
		|| selectedWorkMode == ESelectedWorkMode::RelativelyLocation)
	{
		EntityLocalMoveLocation(newLocation);
	}
	else if (selectedWorkMode == ESelectedWorkMode::WorldRotator
		|| selectedWorkMode == ESelectedWorkMode::RelativelyRotator)
	{
		EntityLocalMoveSelfRotation(newRotator);
	}
	else if (selectedWorkMode == ESelectedWorkMode::Scale)
	{
		EntityLocalMoveScale(newScale);
	}
	else if (selectedWorkMode == ESelectedWorkMode::CoreRotator)
	{
		EntityLocalMoveAllRotation(newLocation, newRotator);
	}
	else if (selectedWorkMode == ESelectedWorkMode::WorldTransform)
	{// 世界变换
		EntityLocalMoveLocation(newLocation);
		EntityLocalMoveSelfRotation(newRotator);
	}
	else if (selectedWorkMode == ESelectedWorkMode::RelativelyTransform)
	{
		FTransform transform2(newRotator, newLocation);
		auto transform3 = UCalculationObject::RelativeToWorldTransform(
			owner_->GetObserverTransform(), transform2);
		UCommonlyObject::Ue4ToPbTransform(transform3, owner_->syncInfo.mutable_transform());
	}

	//EntitySyncInfoToLocalInfo(false);// 更新数据

	EntityUpdateView();
}

// 实例编辑 平移
void UEditEntityEditComponent::EntityLocalMoveLocation(FVector newLocation)
{
	owner_->syncInfo.mutable_transform()->mutable_location()->set_x(newLocation.X);
	owner_->syncInfo.mutable_transform()->mutable_location()->set_y(newLocation.Y);
	owner_->syncInfo.mutable_transform()->mutable_location()->set_z(newLocation.Z);
}

// 实例编辑 旋转  自身旋转
void UEditEntityEditComponent::EntityLocalMoveSelfRotation(FRotator newRotator)
{
	owner_->syncInfo.mutable_transform()->mutable_rotator()->set_pitch(newRotator.Pitch);
	owner_->syncInfo.mutable_transform()->mutable_rotator()->set_yaw(newRotator.Yaw);
	owner_->syncInfo.mutable_transform()->mutable_rotator()->set_roll(newRotator.Roll);
}

// 实例编辑 旋转  环绕中心旋转
void UEditEntityEditComponent::EntityLocalMoveAllRotation(FVector newLocation, FRotator newRotator)
{
	owner_->syncInfo.mutable_transform()->mutable_location()->set_x(newLocation.X);
	owner_->syncInfo.mutable_transform()->mutable_location()->set_y(newLocation.Y);
	owner_->syncInfo.mutable_transform()->mutable_location()->set_z(newLocation.Z);
	owner_->syncInfo.mutable_transform()->mutable_rotator()->set_pitch(newRotator.Pitch);
	owner_->syncInfo.mutable_transform()->mutable_rotator()->set_yaw(newRotator.Yaw);
	owner_->syncInfo.mutable_transform()->mutable_rotator()->set_roll(newRotator.Roll);
}

// 实例编辑 缩放
void UEditEntityEditComponent::EntityLocalMoveScale(FVector newScale)
{
	owner_->syncInfo.mutable_transform()->mutable_scale()->set_x(newScale.X);
	owner_->syncInfo.mutable_transform()->mutable_scale()->set_y(newScale.Y);
	owner_->syncInfo.mutable_transform()->mutable_scale()->set_z(newScale.Z);

	//owner_->syncInfo.mutable_child_list(0)->mutable_transform()->mutable_scale()->set_x(newScale.X);
	//owner_->syncInfo.mutable_child_list(0)->mutable_transform()->mutable_scale()->set_y(newScale.Y);
	//owner_->syncInfo.mutable_child_list(0)->mutable_transform()->mutable_scale()->set_z(newScale.Z);
}

// 实例编辑 更新数据
void UEditEntityEditComponent::EntityUpdateView()
{
	auto actorTransform = UCommonlyObject::PBToUe4Transform(owner_->syncInfo.mutable_transform());

	owner_->SetActorLocation(actorTransform.GetLocation());
	for (int i = 0; i < owner_->childComponentList.Num(); i++)
	{
		owner_->UpdateChildMesh(i);
	}
}

/*----------------------------------------------------*/
