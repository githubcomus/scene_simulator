// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceViewMeshComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewMeshComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Core/BaseGameInstance.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Tools/CommonlyObject.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshView.h"

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
#include <Components/HierarchicalInstancedStaticMeshComponent.h>

//#include<vector>

/*
data
*/

/*------------------------------------------------------------------*/

// UPlaceViewMeshComponent
UPlaceViewMeshComponent::UPlaceViewMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// BeginPlay
void UPlaceViewMeshComponent::Start(AEditEntityManage* view_)
{
	view = view_;

	LoadSelectMaterial();
}

// EndPlay
void UPlaceViewMeshComponent::End()
{
}

/*------------------------------------------------------------------*/

// ????????????
void UPlaceViewMeshComponent::LoadRes(bool satrt, FString path, ELoadResourcesType type, UObject* obj)
{
	if (satrt)
	{
		if (resMap.Find(obj))
		{
			resMap.Remove(obj);
		}
		FSoftObjectPath softObj;
		softObj.SetPath(path);

		CvResourcesInfo cvResourcesInfo;
		cvResourcesInfo.path = path;
		cvResourcesInfo.handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObj,
			FStreamableDelegate::
			CreateUObject(
				this, &UPlaceViewMeshComponent::
				LoadRes, false, path, type,
				obj), 0, false);

		resMap.Add(obj, cvResourcesInfo);
	}
	else
	{
		if (resMap.Find(obj) == false)
		{
			return;
		}

		// ????????????path???map???path????????????,???????????????
		if (path.Equals(resMap[obj].path) == false)
		{
			return;
		}

		auto handle = resMap[obj].handle;
		UObject* resObj = handle->GetLoadedAsset();

		// ??????????????????
		if (type == ELoadResourcesType::LoadRes_Mesh)
		{
			//auto component = Cast<UHierarchicalInstancedStaticMeshComponent>(obj);
			//UStaticMesh* mesh = Cast<UStaticMesh>(resObj);
			//if (component && mesh)
			//{
			//	bool flag = component->SetStaticMesh(mesh);

			//	UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [LoadRes] ???????????? [name:%s]  "),
			//		this,   *mesh->GetName());

			//	// ????????????????????????
			//	TArray<int32> list = meshInstanceCachingMap[component];
			//	for (int32 entityId : list)
			//	{
			//		// ??????????????????
			//		auto syncInfo = view->data->GetSyncInfo(entityId);
			//		auto localInfo = view->data->GetLocalInfo(entityId);
			//		if (syncInfo)
			//		{
			//			//localInfo->staticMesh = mesh;
			//			// ??????????????????
			//			MeshLoadMaterial(syncInfo, localInfo);
			//			// ??????????????????
			//			CreateMeshInstance(syncInfo, localInfo);
			//		}
			//	}

			//	// ????????????
			//	meshInstanceCachingMap[component].Empty();
			//}
		}
		// ??????????????????
		else if (type == ELoadResourcesType::LoadRes_Mesh2)
		{
			// ????????????
			auto component = Cast<UStaticMeshComponent>(obj);
			UStaticMesh* mesh = Cast<UStaticMesh>(resObj);
			if (component && mesh)
			{
				component->SetStaticMesh(mesh);
			}
		}
		// ??????????????????
		else if (type == ELoadResourcesType::LoadRes_Mesh3)
		{
			// ????????????
			auto component = Cast<UStaticMeshComponent>(obj);
			UStaticMesh* mesh = Cast<UStaticMesh>(resObj);
			if (cvSelectMeshInfo.isntanceMap.Find(component))
			{
				auto entityId = cvSelectMeshInfo.isntanceMap[component];
				auto syncInfo = view->data->GetSyncInfo(entityId);
				//auto localInfo = view->data->GetLocalInfo(entityId);
				component->SetStaticMesh(mesh);
				// ????????????
				//localInfo->mesh.selectStaticMesh = mesh;
				// ??????????????????
				//ReplaceSelectMaterial(syncInfo, localInfo);
			}
		}
		// ????????????
		else if (type == ELoadResourcesType::LoadRes_Material2)
		{
			// ????????????
			auto component = Cast<UStaticMeshComponent>(obj);
			UMaterialInstanceConstant* materialInstanceConstant = Cast<UMaterialInstanceConstant>(resObj);
			if (component && component->GetStaticMesh() && materialInstanceConstant)
			{
				for (int32 i = 0; i < component->GetStaticMesh()->GetStaticMaterials().Num(); i++)
				{
					component->SetMaterial(i, materialInstanceConstant);
				}
			}
		}
		if (path.Equals(resMap[obj].path))
		{
			resMap.Remove(obj);
		}
	}
}

// ????????????
void UPlaceViewMeshComponent::MeshLoadMaterial(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	// ?????????0?????????????????????,?????????????????????????????????
	//if (localInfo->mesh.meshComponent->GetInstanceCount() == 0)
	//{
	//	// ??????????????????
	//	auto materialXlsInfo = view->data->GetMaterialInfoWithId(syncInfo->materialid());
	//	if (syncInfo->materialid() != 0 && materialXlsInfo != nullptr)
	//	{
	//		FString materialName = UTF8_TO_TCHAR(materialXlsInfo->Name_.c_str());
	//		FString materialPath = UTF8_TO_TCHAR(materialXlsInfo->MaterialPath_.c_str());
	//		if (localInfo->mesh.meshName.IsEmpty() || materialPath.IsEmpty())
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("[%x] [MeshLoadMaterial] ERR [%s] [%s] "), this, *localInfo->mesh.meshName,
	//			       *materialPath);
	//			return;
	//		}

	//		LoadRes(true, materialPath, ELoadResourcesType::LoadRes_Material2, localInfo->mesh.meshComponent.Get());
	//	}
	//		// ??????
	//	else
	//	{
	//	}
	//}
}

/*------------------------------------------------------------------*/

// ???????????? ??????????????????
void UPlaceViewMeshComponent::CreateMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool mast)
{
	// ????????????(??????/??????)

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (baseGameMode->sceneStaticMeshView == nullptr)return;

	xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->child_list(0).mesh().xls_id());
	FString meshPath = UTF8_TO_TCHAR(xlsInfo->MeshPath_.c_str());

	//baseGameMode->sceneStaticMeshView->CreateInstance(std::to_string(syncInfo->entityid()), ASceneStaticMeshView::ESceneActorType::ActorType_Item, meshPath, localInfo->multipleMesh[0].meshWTransform, localInfo->multipleMesh[0].openCollision, mast);
}

// ???????????? ??????????????????
void UPlaceViewMeshComponent::DestroyMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool mast)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (baseGameMode->sceneStaticMeshView == nullptr)return;
	//baseGameMode->sceneStaticMeshView->DestroyInstance(std::to_string(syncInfo->entityid()), mast);
}

// ???????????? ????????????
void UPlaceViewMeshComponent::UpdateMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (baseGameMode->sceneStaticMeshView == nullptr)return;
	//baseGameMode->sceneStaticMeshView->UpdateInstance(std::to_string(syncInfo->entityid()), localInfo->multipleMesh[0].meshWTransform);
}

/*------------------------------------------------------------------*/

// ???????????? ??????
void UPlaceViewMeshComponent::CreateSelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());

	//FString meshPath = UTF8_TO_TCHAR(xlsInfo->MeshPath_.c_str());
	//if (localInfo->mesh.meshName.IsEmpty() || meshPath.IsEmpty())
	//{
	//	return;
	//}

	//UStaticMeshComponent* staticMesh = NewObject<UStaticMeshComponent>(this, *localInfo->mesh.selectMeshName);
	//staticMesh->RegisterComponent();
	//staticMesh->AttachToComponent(view->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//staticMesh->bReceivesDecals = false;
	//staticMesh->SetGenerateOverlapEvents(false);
	//staticMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	//staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//staticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	//staticMesh->SetCastShadow(false);

	//staticMesh->SetWorldTransform(localInfo->mesh.meshWTransform);
	//localInfo->mesh.selectMeshComponent = staticMesh;
	//
	//LoadRes(true, meshPath, ELoadResourcesType::LoadRes_Mesh3, staticMesh);

	//cvSelectMeshInfo.isntanceMap.Add(localInfo->mesh.selectMeshComponent, syncInfo->entityid());
}

// ???????????? ??????
void UPlaceViewMeshComponent::UpdateSelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//if (localInfo->mesh.selectMeshComponent == nullptr || IsValid(localInfo->mesh.selectMeshComponent) == false)return;
	//localInfo->mesh.selectMeshComponent->SetWorldTransform(localInfo->mesh.meshWTransform);
}

// ???????????? ????????????
void UPlaceViewMeshComponent::SetSelectMeshHide(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool isShow)
{
	//if (localInfo->mesh.selectMeshComponent == nullptr || IsValid(localInfo->mesh.selectMeshComponent) == false)return;
	//localInfo->mesh.selectMeshComponent->SetWorldTransform(localInfo->mesh.meshWTransform);
	//if(isShow)
	//{
	//	localInfo->mesh.selectMeshComponent->SetHiddenInGame(false);
	//}
	//else
	//{
	//	localInfo->mesh.selectMeshComponent->SetHiddenInGame(true);
	//}
}

// ???????????? ??????
void UPlaceViewMeshComponent::DestroySelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//if (localInfo->mesh.selectMeshComponent == nullptr || IsValid(localInfo->mesh.selectMeshComponent) == false)return;

	//cvSelectMeshInfo.isntanceMap.Remove(localInfo->mesh.selectMeshComponent);

	//if (localInfo->mesh.selectMeshComponent->IsRegistered())
	//{
	//	localInfo->mesh.selectMeshComponent->UnregisterComponent();
	//}
	//localInfo->mesh.selectMeshComponent->DestroyComponent();
	//localInfo->mesh.selectMeshComponent = nullptr;
}

// ???????????? ??????????????????
void UPlaceViewMeshComponent::LoadSelectMaterial()
{
	FString path = TEXT(
		"MaterialInstanceConstant'/Game/Resources/Material/Selected/Outline/MI_Outline_Inst.MI_Outline_Inst'");
	selectMaterial = LoadObject<UMaterialInstanceConstant>(NULL, *path);
}

// ????????????  ??????????????????
void UPlaceViewMeshComponent::ReplaceSelectMaterial(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//if (localInfo->mesh.selectMeshComponent == nullptr || IsValid(localInfo->mesh.selectMeshComponent) == false || localInfo->mesh.selectStaticMesh == nullptr)return;
	//for (int32 i = 0; i < localInfo->mesh.selectStaticMesh->GetStaticMaterials().Num(); i++)
	//{
	//	localInfo->mesh.selectMeshComponent->SetMaterial(i, selectMaterial);
	//}
}

/*------------------------------------------------------------------*/

