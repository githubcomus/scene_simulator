// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceViewCollisionComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewCollisionComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Core/BaseGameInstance.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Tools/CommonlyObject.h"

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
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"

//#include<vector>

/*
data
*/

/*------------------------------------------------------------------*/

// UPlaceViewCollisionComponent
UPlaceViewCollisionComponent::UPlaceViewCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// BeginPlay
void UPlaceViewCollisionComponent::Start(AEditEntityManage* view_)
{
	view = view_;
}

// EndPlay
void UPlaceViewCollisionComponent::End()
{
}

/*------------------------------------------------------------------*/

//// ???????????? ??????
//void UPlaceViewCollisionComponent::CreateBox(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	// xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());
//	auto xlsInfo = view->data->GetXlsInfo(syncInfo->entityid());
//	if (xlsInfo == nullptr)return;
//	FString collisionBoxName = FString::Printf(TEXT("Place_Box_%d"), syncInfo->entityid());
//
//	// ??????????????????
//	UBoxComponent* box = NewObject<UBoxComponent>(this, *collisionBoxName);
//	box->RegisterComponent();
//	box->AttachToComponent(view->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
//
//	// ??????
//	box->SetGenerateOverlapEvents(false);
//	box->SetCollisionObjectType(BuildObject);
//	box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//	box->SetCollisionResponseToAllChannels(ECR_Ignore);
//	box->SetEnableGravity(false);
//
//	// // ?????????????????????
//	// if (xlsInfo->UsingModelCollisions_ == false)
//	// {
//	// 	box->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
//	// 	box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
//	// 	box->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
//	// 	box->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
//	// }
//	// // ??????????????????
//	// else
//	{
//		box->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
//		box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
//		box->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
//		box->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
//	}
//
//	// ?????????????????????-1,????????????????????????????????????
//	// box->SetBoxExtent(localInfo->collisionExten);
//	box->SetBoxExtent(FVector(1, 1, 1));
//
//	box->SetHiddenInGame(true);
//	box->SetWorldTransform(localInfo->multipleCollision[0].wTransform);
//
//	localInfo->multipleCollision[0].boxComponent = box;
//
//	UEditEntityManageData::PlaceBoxInfo placeBoxInfo;
//	placeBoxInfo.entityId = syncInfo->entityid();
//	placeBoxInfo.index = 0;
//	view->data->collisionBoxMap.Add(localInfo->multipleCollision[0].boxComponent, placeBoxInfo);
//
//	// ??????
//	if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Image)
//		// if (syncInfo->type() == Gamedata::EntityType::EntityType_Image)
//	{
//		localInfo->multipleCollision[0].boxComponent->SetWorldScale3D(FVector(20, 222, 222));
//		box->SetHiddenInGame(false);
//
//		// localInfo->collision.boxComponent->SetBoxExtent(exten);
//		// box->SetBoxExtent(FVector(1, 1, 1));
//	}
//	// ??????
//	else if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_laser)
//		// else if (syncInfo->type() == Gamedata::EntityType::EntityType_Light)
//	{
//		localInfo->multipleCollision[0].boxComponent->SetWorldScale3D(FVector(40, 40, 40));
//		box->SetHiddenInGame(true);
//
//		// localInfo->collision.boxComponent->SetBoxExtent(exten);
//		// box->SetBoxExtent(FVector(1, 1, 1));
//	}
//	// ????????????
//	else if (xlsInfo->Type_ == xlsdata::PlaceType::PlaceType_Mesh)
//		// else if(syncInfo->type() == Gamedata::EntityType::EntityType_Mesh)
//	{
//	}
//
//	// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceViewCollisionComponent::CreateBox] [entityId:%d]  [box:%x]"),
//	// 	this, syncInfo->entityid(),  localInfo->collision.boxComponent);
//}
//
//// ???????????? ??????
//void UPlaceViewCollisionComponent::DestroyBox(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	if (localInfo->multipleCollision[0].boxComponent && IsValid(localInfo->multipleCollision[0].boxComponent))
//	{
//		view->data->collisionBoxMap.Remove(localInfo->multipleCollision[0].boxComponent);
//		if (localInfo->multipleCollision[0].boxComponent->IsRegistered())
//		{
//			localInfo->multipleCollision[0].boxComponent->UnregisterComponent();
//		}
//		localInfo->multipleCollision[0].boxComponent->DestroyComponent();
//		localInfo->multipleCollision[0].boxComponent = nullptr;
//	}
//}
//
//// ???????????? ??????
//void UPlaceViewCollisionComponent::UpdateBox(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo, bool changeScale)
//{
//	xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());
//	if (changeScale)
//	{
//		localInfo->multipleCollision[0].boxComponent->SetWorldTransform(localInfo->multipleCollision[0].wTransform);
//	}
//	else
//	{
//		localInfo->multipleCollision[0].boxComponent->SetWorldLocationAndRotation(localInfo->multipleCollision[0].wTransform.GetLocation(), localInfo->multipleCollision[0].wTransform.GetRotation().Rotator());
//	}
//}
//
//// ???????????? ????????????
//void UPlaceViewCollisionComponent::BoxSetImageSize(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	if (localInfo->multipleCollision[0].boxComponent && IsValid(localInfo->multipleCollision[0].boxComponent))
//	{
//		FVector exten(2, localInfo->image.imageSize.X / 2, localInfo->image.imageSize.Y / 2);
//
//		auto t = syncInfo->transform();
//		auto t2 = UCommonlyObject::PBToUe4Transform(&t);
//
//		float scaleX = exten.X * 1;
//		float scaleY = exten.Y * t2.GetScale3D().Y;
//		float scaleZ = exten.Z * t2.GetScale3D().Z;
//		auto setVar = FVector(scaleX, scaleY, scaleZ);
//		localInfo->multipleCollision[0].rTransform.SetScale3D(setVar);
//		localInfo->multipleCollision[0].boxComponent->SetWorldScale3D(setVar);
//
//		localInfo->multipleCollision[0].boxComponent->SetHiddenInGame(true);
//		// UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceViewCollisionComponent::BoxSetImageSize] [entityId:%d] [size:%s] [set:%s] [box:%x]"),
//		//        this, syncInfo->entityid(), *localInfo->image.imageSize.ToString(), *setVar.ToString(),localInfo->collision.boxComponent);
//	}
//}

/*------------------------------------------------------------------*/
//
//// ???????????? ??????????????????????????????????????????
//bool UPlaceViewCollisionComponent::CreateCollisionMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());
//
//	FString meshPath = UTF8_TO_TCHAR(xlsInfo->MeshPath_.c_str());
//	if (localInfo->multipleMesh[0].meshName.IsEmpty() || meshPath.IsEmpty())
//	{
//		UE_LOG(LogTemp, Log, TEXT("[%x] [CreateCollisionMeshComponent] ERR [%s] [%s] "), this, *localInfo->multipleMesh[0].meshName,
//			*meshPath);
//		return false;
//	}
//
//	// ??????????????????
//	if (view->data->collisionMeshMap.Find(localInfo->multipleCollision[0].collisionName))
//	{
//		localInfo->multipleCollision[0].collisionMesh = view->data->collisionMeshMap[localInfo->multipleCollision[0].collisionName];
//		view->data->collisionMeshCountMap[localInfo->multipleCollision[0].collisionName]++;
//	}
//	else
//	{
//		// ??????????????????
//		UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this, *localInfo->multipleCollision[0].collisionName);
//		meshComponent->RegisterComponent();
//		meshComponent->AttachToComponent(view->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
//
//		// ??????????????????
//		meshComponent->bReceivesDecals = false;
//		meshComponent->CastShadow = false;
//
//		meshComponent->SetGenerateOverlapEvents(false);
//		meshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
//		meshComponent->SetCollisionObjectType(BuildObject);
//		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//		meshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
//
//		// ??????
//		meshComponent->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator::ZeroRotator);
//
//		LoadRes(true, meshPath, ELoadResourcesType::LoadRes_Mesh2, meshComponent);
//
//		view->data->collisionMeshMap.Add(localInfo->multipleCollision[0].collisionName, meshComponent);
//
//		localInfo->multipleCollision[0].collisionMesh = view->data->collisionMeshMap[localInfo->multipleCollision[0].collisionName];
//
//		// ????????????
//		HiddenCollisionMeshComponent(syncInfo, localInfo);
//
//		view->data->collisionMeshCountMap.Add(localInfo->multipleCollision[0].collisionName, 1);
//
//		UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [CreateCollisionMeshComponent] ???????????? [time:%f] [%s]"), this, GetOwner()->GetWorld()->TimeSeconds, *localInfo->multipleMesh[0].meshName);
//	}
//	return true;
//}
//
//// ???????????? ??????????????????????????????????????????
//void UPlaceViewCollisionComponent::DestroyCollisonMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	//if (localInfo->mesh.meshComponent == nullptr)
//	{
//		if (view->data->collisionMeshCountMap.Find(localInfo->multipleCollision[0].collisionName)) {
//			view->data->collisionMeshCountMap[localInfo->multipleCollision[0].collisionName]--;
//			int value = view->data->collisionMeshCountMap[localInfo->multipleCollision[0].collisionName];
//
//			if (value == 0) {
//				if (view->data->collisionMeshMap.Find(localInfo->multipleCollision[0].collisionName))
//				{
//					auto component = view->data->collisionMeshMap[localInfo->multipleCollision[0].collisionName];
//
//					if (component->IsRegistered())
//					{
//						component->UnregisterComponent();
//					}
//					component->DestroyComponent();
//
//					view->data->collisionMeshMap.Remove(localInfo->multipleCollision[0].collisionName);
//					view->data->collisionMeshCountMap.Remove(localInfo->multipleCollision[0].collisionName);
//					UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [DestroyCollisonMeshComponent] ???????????? [time:%f]  [%s]"), this, GetOwner()->GetWorld()->TimeSeconds, *localInfo->multipleMesh[0].meshName);
//				}
//			}
//		}
//	}
//}
//
//// ???????????? ??????,??????????????????mesh??????
//void UPlaceViewCollisionComponent::ShowCollisionMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	if (localInfo->multipleCollision[0].collisionMesh.IsValid())
//	{
//		//auto tempT = localInfo->mesh.meshWTransform;
//		//tempT.SetScale3D(tempT.GetScale3D() * 1.3);
//		//tempT.SetLocation(tempT.GetLocation() + FVector::UpVector * 30);
//		localInfo->multipleCollision[0].collisionMesh->SetWorldTransform(localInfo->multipleMesh[0].meshWTransform);
//		localInfo->multipleCollision[0].collisionMesh->SetHiddenInGame(false);
//
//		//UKismetSystemLibrary::DrawDebugCapsule(GetOwner()->GetWorld(), localInfo->mesh.meshWTransform.GetLocation(), 5, 5, FRotator::ZeroRotator, FLinearColor::Red, 3, 2);
//		//UKismetSystemLibrary::DrawDebugLine(GetOwner(), UGameplayStatics::GetPlayerCharacter(GetOwner(), 0)->GetActorLocation(), localInfo->mesh.meshWTransform.GetLocation(), FLinearColor::Blue, 5, 2);
//	}
//	else {
//		//collisionMeshMap
//		auto& tempMap = view->data->collisionMeshMap;
//
//		UE_LOG(LogTemp, Error, TEXT("[%x] [????????????] [ShowCollisionMeshComponent]  ??????????????????[time:%f]  [%s]"), this, GetOwner()->GetWorld()->TimeSeconds, *localInfo->multipleMesh[0].meshName);
//	}
//}
//
//// ???????????? ??????,??????????????????mesh??????
//void UPlaceViewCollisionComponent::HiddenCollisionMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	if (localInfo->multipleCollision[0].collisionMesh.IsValid())
//	{
//		localInfo->multipleCollision[0].collisionMesh->SetHiddenInGame(true);
//	}
//}

/*------------------------------------------------------------------*/

// ????????????
void UPlaceViewCollisionComponent::LoadRes(bool satrt, FString path, ELoadResourcesType type, UObject* obj)
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
				this, &UPlaceViewCollisionComponent::
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

/*------------------------------------------------------------------*/