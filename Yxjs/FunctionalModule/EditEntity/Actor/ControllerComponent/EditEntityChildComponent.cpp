// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityChildComponent.h"
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
#include "../../../SceneStaticMesh/SceneStaticMeshView.h"
#include "Yxjs/Tools/CalculationObject.h"
#include "EditEntityTimelineComponent.h"
#include "../ViewComponent/EditEntityColorMeshComponent.h"
#include "../ViewComponent/EditEntitySelectMeshComponent.h"
#include "FunctionalModule/EditEntity/ScanMesh/ScanMesh.h"

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

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

// ????????????
FString UEditEntityChildComponent::GetMeshName()
{

	FString name = UTF8_TO_TCHAR(childMeshXlsInfo->Name_.c_str());
	FString meshName = FString::Printf(TEXT("ShowStaticMesh_%d_%s"), childSyncInfo->mesh().material_id(), *name);
	return meshName;
}

// ????????????
ECollisionEnabled::Type UEditEntityChildComponent::GetCollisionType()
{

	ECollisionEnabled::Type openCollision = ECollisionEnabled::Type::QueryAndPhysics;
	if (owner_->syncInfo.collision_type() == Gamedata::EntityCollisionType::EntityCollisionType_None)
	{
		openCollision = ECollisionEnabled::Type::NoCollision;
	}
	else if (owner_->syncInfo.collision_type() == Gamedata::EntityCollisionType::EntityCollisionType_Query)
	{
		openCollision = ECollisionEnabled::Type::QueryOnly;
	}
	else if (owner_->syncInfo.collision_type() == Gamedata::EntityCollisionType::EntityCollisionType_QueryAndPhysics)
	{
		openCollision = ECollisionEnabled::Type::QueryAndPhysics;
	}
	return openCollision;

}

// ???????????????
UStaticMesh* UEditEntityChildComponent::GetMesh()
{
	if (owner_->IsDynamicColorChange())
	{
		if (colorMeshComponent)
		{
			return colorMeshComponent->GetStaticMesh();
		}
	}
	else
	{
		auto component = owner_->gameMode->sceneStaticMeshView->GetEntityMeshComponent(meshIstanceId);
		if (component.IsValid())
		{
			return component->GetStaticMesh();
		}
	}
	return nullptr;
}

/*/////////////////////////////////////////////////////////////*/
// ?????????
/*/////////////////////////////////////////////////////////////*/

/*----------------------------------------------------*/

// ??????local info
void UEditEntityChildComponent::CalculateLocalInfo()
{
	auto spawnWTransform = UCommonlyObject::PBToUe4Transform(owner_->syncInfo.mutable_transform());
	meshInfo.rTransform = UCommonlyObject::PBToUe4Transform(childSyncInfo->mutable_transform());
	meshInfo.wTransform = UCalculationObject::RelativeToWorldTransform(spawnWTransform, meshInfo.rTransform);
}

/*----------------------------------------------------*/

//
UEditEntityChildComponent::UEditEntityChildComponent()
{
}

// 
void UEditEntityChildComponent::Start(AEditEntityActor* view, int index_)
{
	if (view->gameMode->sceneStaticMeshView == nullptr)return;

	owner_ = view;
	editEntityManage = owner_->editEntityManage;
	index = index_;

	childMeshXlsInfo = owner_->GetChildMeshXlsInfo(index_);
	childMaterialXlsInfo = owner_->GetChildMaterialXlsInfo(index_);
	childSyncInfo = owner_->syncInfo.mutable_child_list(index);

	CalculateLocalInfo();

	BoxCreate();

	ChildCreate(false);

}

// 
void UEditEntityChildComponent::End()
{
	if (owner_.IsValid() == false || owner_->gameMode->sceneStaticMeshView == nullptr)return;

	BoxDestroy();

	DestroySelectMesh();

	ChildDestroy(false);


}

/*----------------------------------------------------*/

// ?????? ??????
void UEditEntityChildComponent::EntityUpdate()
{
	if (owner_->gameMode->sceneStaticMeshView == nullptr)return;
	CalculateLocalInfo();
	BoxUpdate();

	if (selectMeshComponent)
	{// ????????????
		selectMeshComponent->SetWorldTransform(meshInfo.wTransform);
	}
	else
	{
		ChildUpdate(meshInfo.wTransform);
	}
}

// ?????? ????????????
void UEditEntityChildComponent::ReplaceMesh()
{
	childMeshXlsInfo = owner_->GetChildMeshXlsInfo(index);
	childMaterialXlsInfo = owner_->GetChildMaterialXlsInfo(index);
	childSyncInfo = owner_->syncInfo.mutable_child_list(index);

	//1. ????????????,???????????????????????????,??????????????????????????????,child????????????????????????,?????????????????????????????????.
	if (selectMeshComponent)
	{
		ChildDestroy(true);
		ChildCreate(true);
		selectMeshComponent->ReplaceMesh();
	}
	//2. ???????????????,?????????????????????
	else
	{
		ChildDestroy(true);
		ChildCreate(false);
	}
}

// ?????? ?????????????????????
void UEditEntityChildComponent::SetLocationAndRotator(FTransform& newTransform)
{
	if (selectMeshComponent)
	{
		selectMeshComponent->SetWorldTransform(newTransform);
	}
	else
	{
		ChildUpdate(newTransform);
	}
}

// ?????? ??????????????????????????????
void UEditEntityChildComponent::SetEditStartTransform()
{
	auto oldWorldtransform = UCommonlyObject::PBToUe4Transform(owner_->observerSyncInfo.mutable_transform());
	auto worldTransform = UCalculationObject::RelativeToWorldTransform(
		oldWorldtransform, meshInfo.rTransform);
	ChildUpdate(worldTransform);
}

/*----------------------------------------------------*/

// ????????????
void UEditEntityChildComponent::ChildCreate(bool hide)
{

	if (childMeshXlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Mesh)
	{
		if (owner_->IsDynamicColorChange())
		{
			CreateDynamicColorMesh(hide);
		}
		else
		{
			CreateMeshInstance(hide);
		}
	}

}

// ????????????
void UEditEntityChildComponent::ChildUpdate(FTransform& newTransform)
{

	if (childMeshXlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Mesh)
	{
		if (owner_->IsDynamicColorChange() && colorMeshComponent)
		{
			colorMeshComponent->SetWorldTransform(newTransform);
		}
		else
		{
			owner_->gameMode->sceneStaticMeshView->UpdateInstance(meshIstanceId, newTransform);// mesh
		}
	}
}

// ????????????
void UEditEntityChildComponent::ChildDestroy(bool thisFrame)
{

	if (childMeshXlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Mesh)
	{
		DestroyDynamicColorMesh();
		DestroyMeshInstance(thisFrame);
	}
}

/*----------------------------------------------------*/

// ???????????? ??????
void UEditEntityChildComponent::CreateMeshInstance(bool hide)
{

	bool castShadow = owner_->IsUseTimeline() ? false : true;
	FString meshPath = UTF8_TO_TCHAR(childMeshXlsInfo->MeshPath_.c_str());
	FString materialPath = UTF8_TO_TCHAR(childMaterialXlsInfo->MaterialPath_.c_str());
	if (childMaterialXlsInfo->ID_ == 0)materialPath = TEXT("");

	ECollisionEnabled::Type type = ECollisionEnabled::QueryOnly;
	if (owner_->entityId == PreviewEntityID)type = ECollisionEnabled::NoCollision;

	FTransform transform;
	if (hide)
	{
		transform = FTransform(FRotator::ZeroRotator, FVector(0, 0, -500000), FVector::ZeroVector);
	}
	else
	{
		transform = meshInfo.wTransform;
	}
	Gamedata::FVector color = childSyncInfo->mesh().color();

	FTempSceneStaticInfo tempInfo;
	tempInfo.openCollision = type;
	tempInfo.castShadow = castShadow;
	tempInfo.entityId = owner_->entityId;
	tempInfo.materialPath = materialPath;
	tempInfo.color = FVector(color.x(), color.y(), color.z());
	tempInfo.highlight = childSyncInfo->mesh().highlight();
	tempInfo.materialId = childMaterialXlsInfo->ID_;

	meshIstanceId = owner_->gameMode->sceneStaticMeshView->CreateInstance(ASceneStaticMeshView::ESceneActorType::ActorType_Item,
		meshPath, transform, false, tempInfo);
}

// ???????????? ??????
void UEditEntityChildComponent::DestroyMeshInstance(bool thisFrame)
{

	if (meshIstanceId != 0)
	{
		owner_->gameMode->sceneStaticMeshView->DestroyInstance(meshIstanceId, thisFrame);
		meshIstanceId = 0;
	}
}

// ?????????????????? ??????
void UEditEntityChildComponent::CreateDynamicColorMesh(bool hide)
{
	FString name = FString::Printf(TEXT("colorMeshComponent_%d"), index);
	colorMeshComponent = NewObject<UEditEntityColorMeshComponent>(this, *name);
	colorMeshComponent->RegisterComponent();
	colorMeshComponent->AttachToComponent(owner_->sceneRoot, FAttachmentTransformRules::KeepWorldTransform);
	colorMeshComponent->Start(index);
	owner_->AddInstanceComponent(colorMeshComponent);

	FTransform transform;
	if (hide)
	{
		transform = FTransform(FRotator::ZeroRotator, FVector(0, 0, -500000), FVector::ZeroVector);
	}
	else
	{
		transform = meshInfo.wTransform;
	}
	colorMeshComponent->SetWorldTransform(transform);
}

// ?????????????????? ??????
void UEditEntityChildComponent::DestroyDynamicColorMesh()
{
	if (colorMeshComponent == nullptr)return;
	owner_->RemoveInstanceComponent(colorMeshComponent);
	colorMeshComponent->End();
	if (colorMeshComponent->IsRegistered())
	{
		colorMeshComponent->UnregisterComponent();
	}
	colorMeshComponent->DestroyComponent();
	colorMeshComponent = nullptr;

}

// ?????????????????? ??????
void UEditEntityChildComponent::CreateSelectMesh()
{
	FString name = FString::Printf(TEXT("selectMeshComponent_%d"), index);
	selectMeshComponent = NewObject<UEditEntitySelectMeshComponent>(this, *name);
	selectMeshComponent->RegisterComponent();
	selectMeshComponent->AttachToComponent(owner_->sceneRoot, FAttachmentTransformRules::KeepWorldTransform);
	selectMeshComponent->SetWorldTransform(meshInfo.wTransform);
	selectMeshComponent->Start(index);
	owner_->AddInstanceComponent(selectMeshComponent);

	FTransform tempT(FRotator::ZeroRotator, FVector(0, 0, -5000));
	ChildUpdate(tempT);// ?????????????????????
}

// ?????????????????? ??????
void UEditEntityChildComponent::DestroySelectMesh()
{
	if (selectMeshComponent == nullptr)return;

	owner_->RemoveInstanceComponent(selectMeshComponent);
	if (selectMeshComponent->IsRegistered())
	{
		selectMeshComponent->UnregisterComponent();
	}
	selectMeshComponent->DestroyComponent();
	selectMeshComponent = nullptr;

	ChildUpdate(meshInfo.wTransform);
}

// ???????????? ?????? 
void UEditEntityChildComponent::BoxCreate()
{

	FString name = UTF8_TO_TCHAR(childMeshXlsInfo->Name_.c_str());
	FString collisionName = FString::Printf(
		TEXT("CollisionBox_%d_%s"), index, *name);
	boxComponent = NewObject<UBoxComponent>(this, *collisionName);
	boxComponent->RegisterComponent();
	boxComponent->AttachToComponent(owner_->sceneRoot, FAttachmentTransformRules::KeepWorldTransform); // GetOwner()->GetRootComponent()
	boxComponent->SetBoxExtent(FVector(1, 1, 1));

	boxComponent->SetGenerateOverlapEvents(false);
	boxComponent->SetCollisionObjectType(BuildObject);
	boxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComponent->SetEnableGravity(false);

	boxComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	boxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	boxComponent->SetHiddenInGame(true);

	UEditEntityManageData::PlaceBoxInfo placeBoxInfo;
	placeBoxInfo.entityId = owner_->syncInfo.entityid();
	placeBoxInfo.index = index;
	editEntityManage->data->collisionBoxMap.Add(boxComponent, placeBoxInfo);

	boxComponent->SetWorldTransform(meshInfo.wTransform);
	FTransform rTransform(FRotator(0, 0, 0).Quaternion(), FVector::ZeroVector, FVector(50, 50, 50));
	BoxCallback(true, rTransform);
}

// ???????????? ??????(???????????????????????????????????????)
void UEditEntityChildComponent::BoxCallback(bool isStart, FTransform rTransform)
{
	if (isStart)
	{
		FString meshPath = UTF8_TO_TCHAR(childMeshXlsInfo->MeshPath_.c_str());
		owner_->editEntityManage->scanMesh->SetBoxEnter(meshPath, this);
		//collisionInfo.rTransform = rTransform;
		//BoxUpdate();
	}
	else
	{
		collisionInfo.rTransform = rTransform;
		BoxUpdate();
	}
}

// ???????????? ??????
void UEditEntityChildComponent::BoxUpdate()
{
	auto wTransform = UCalculationObject::RelativeToWorldTransform(meshInfo.wTransform, collisionInfo.rTransform);
	boxComponent->SetWorldTransform(wTransform);
}

// ???????????? ??????
void UEditEntityChildComponent::BoxDestroy()
{

	if (editEntityManage->data->collisionBoxMap.Find(boxComponent))
	{
		editEntityManage->data->collisionBoxMap.Remove(boxComponent);
	}
	if (boxComponent->IsRegistered())
	{
		boxComponent->UnregisterComponent();
	}
	boxComponent->DestroyComponent();
	boxComponent = nullptr;

}

/*----------------------------------------------------*/

// ???????????? ??????,??????????????????mesh??????
void UEditEntityChildComponent::ShowCollisionMeshComponent(bool show)
{
	if (show)
	{
		CreateCollisionMeshComponent();
	}
	else
	{
		DestroyCollisonMeshComponent();
	}
}

// ???????????? ??????????????????????????????????????????
bool UEditEntityChildComponent::CreateCollisionMeshComponent()
{
	FString name = UTF8_TO_TCHAR(childMeshXlsInfo->Name_.c_str());
	FString collisionName = FString::Printf(TEXT("????????????_%d_%s"), index, *name);

	UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this, *collisionName);
	meshComponent->RegisterComponent();
	meshComponent->AttachToComponent(owner_->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	meshComponent->bReceivesDecals = false;
	meshComponent->CastShadow = false;
	meshComponent->SetGenerateOverlapEvents(false);
	meshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	meshComponent->SetCollisionObjectType(BuildObject);
	meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	meshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	auto mesh = GetMesh();
	if (mesh)
	{
		meshComponent->SetStaticMesh(mesh);
	}

	meshComponent->SetHiddenInGame(true);
	meshComponent->SetWorldTransform(meshInfo.wTransform);
	collisionMeshComponent = meshComponent;

	UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [CreateCollisionMeshComponent] ???????????? [%s]"), this, *collisionName);

	return true;
}

// ???????????? ??????????????????????????????????????????
void UEditEntityChildComponent::DestroyCollisonMeshComponent()
{
	if (collisionMeshComponent->IsRegistered())
	{
		collisionMeshComponent->UnregisterComponent();
	}
	collisionMeshComponent->DestroyComponent();

	collisionMeshComponent = nullptr;

	FString name = UTF8_TO_TCHAR(childMeshXlsInfo->Name_.c_str());
	FString collisionName = FString::Printf(TEXT("????????????_%d_%s"), index, *name);
	UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [DestroyCollisonMeshComponent] ????????????  [%s]"), this, *collisionName);
}

/*----------------------------------------------------*/
