//// Fill out your copyright notice in the Description page of Project Settings.
//
//#include "PlaceViewLaserBeamComponent.h"
//#include "FunctionalModule/EditEntity/TempComponent/PlaceViewLaserBeamComponent.h"
//#include "FunctionalModule/EditEntity/EditEntityManageController.h"
//#include "FunctionalModule/EditEntity/EditEntityManageData.h"
//#include "FunctionalModule/EditEntity/EditEntityManage.h"
//#include "Core/BaseGameInstance.h"
//#include "Core/Component/ZwXlsDataActorComponent.h"
//#include "GameMode/BaseGameMode.h"
//#include "Pawn/Character/BaseCharacter.h"
//#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
//#include "Tools/CommonlyObject.h"
//
//#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
//#include "Runtime/Engine/Classes/Components/ActorComponent.h"
//#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
//#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
//#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
//#include "Runtime/Engine/Classes/Components/BoxComponent.h"
//#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
//#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
//#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
//#include "UObject/ConstructorHelpers.h"
//#include "Runtime/Engine/Classes/Materials/Material.h"
//#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
//#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
//#include "Runtime/Engine/Classes/GameFramework/Character.h"
//#include "Camera/CameraComponent.h"
//#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
//#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
//#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
//#include "Engine/AssetManager.h"
//#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
//#include "Runtime/UMG/Public/Components/WidgetComponent.h"
//#include "Misc/OutputDeviceDebug.h"
//#include "Components/Image.h"
//#include "Components/CanvasPanelSlot.h"
//#include <Components/HierarchicalInstancedStaticMeshComponent.h>
//
////#include<vector>
//
///*
//data
//*/
//
///*------------------------------------------------------------------*/
//
//// UPlaceViewLaserBeamComponent
//UPlaceViewLaserBeamComponent::UPlaceViewLaserBeamComponent()
//{
//	PrimaryComponentTick.bCanEverTick = false;
//}
//
//// BeginPlay
//void UPlaceViewLaserBeamComponent::Start(AEditEntityManage* view_)
//{
//	view = view_;
//
//	LoadSelectMaterial();
//}
//
//// EndPlay
//void UPlaceViewLaserBeamComponent::End()
//{
//}
//
///*------------------------------------------------------------------*/
//
//// ????????????
//void UPlaceViewLaserBeamComponent::LoadRes(bool satrt, FString path, ELoadResourcesType type, UObject* obj)
//{
//	if (satrt)
//	{
//		if (resMap.Find(obj))
//		{
//			resMap.Remove(obj);
//		}
//		FSoftObjectPath softObj;
//		softObj.SetPath(path);
//
//		CvResourcesInfo cvResourcesInfo;
//		cvResourcesInfo.path = path;
//		cvResourcesInfo.handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObj,
//			FStreamableDelegate::
//			CreateUObject(
//				this, &UPlaceViewLaserBeamComponent::
//				LoadRes, false, path, type,
//				obj), 0, false);
//
//		resMap.Add(obj, cvResourcesInfo);
//	}
//	else
//	{
//		if (resMap.Find(obj) == false)
//		{
//			return;
//		}
//
//		// ????????????path???map???path????????????,???????????????
//		if (path.Equals(resMap[obj].path) == false)
//		{
//			return;
//		}
//
//		auto handle = resMap[obj].handle;
//		UObject* resObj = handle->GetLoadedAsset();
//
//		// ??????????????????
//		if (type == ELoadResourcesType::LoadRes_Mesh)
//		{
//			auto component = Cast<UHierarchicalInstancedStaticMeshComponent>(obj);
//			UStaticMesh* mesh = Cast<UStaticMesh>(resObj);
//			if (component && mesh)
//			{
//				bool flag = component->SetStaticMesh(mesh);
//
//				// ????????????????????????
//				TArray<int32> list = meshInstanceCachingMap[component];
//				for (int32 entityId : list)
//				{
//					// ??????????????????
//					auto syncInfo = view->data->GetSyncInfo(entityId);
//					auto localInfo = view->data->GetLocalInfo(entityId);
//					if (syncInfo)
//					{
//						//localInfo->staticMesh = mesh;
//						// ??????????????????
//						MeshLoadMaterial(syncInfo, localInfo);
//						// ??????????????????
//						CreateMeshInstance(syncInfo, localInfo);
//					}
//				}
//
//				// ????????????
//				meshInstanceCachingMap[component].Empty();
//			}
//		}
//		// ??????????????????
//		else if (type == ELoadResourcesType::LoadRes_Mesh2)
//		{
//			// ????????????
//			auto component = Cast<UStaticMeshComponent>(obj);
//			UStaticMesh* mesh = Cast<UStaticMesh>(resObj);
//			if (component && mesh)
//			{
//				component->SetStaticMesh(mesh);
//			}
//		}
//		// ??????????????????
//		else if (type == ELoadResourcesType::LoadRes_Mesh3)
//		{
//			// ????????????
//			auto component = Cast<UStaticMeshComponent>(obj);
//			UStaticMesh* mesh = Cast<UStaticMesh>(resObj);
//			if (cvSelectMeshInfo.isntanceMap.Find(component))
//			{
//				auto entityId = cvSelectMeshInfo.isntanceMap[component];
//				auto syncInfo = view->data->GetSyncInfo(entityId);
//				auto localInfo = view->data->GetLocalInfo(entityId);
//				component->SetStaticMesh(mesh);
//				// ????????????
//				localInfo->laser.selectStaticMesh = mesh;
//				// ??????????????????
//				ReplaceSelectMaterial(syncInfo, localInfo);
//			}
//		}
//		// ????????????
//		else if (type == ELoadResourcesType::LoadRes_Material2)
//		{
//			// ????????????
//			auto component = Cast<UStaticMeshComponent>(obj);
//			UMaterialInstanceConstant* materialInstanceConstant = Cast<UMaterialInstanceConstant>(resObj);
//			if (component && component->GetStaticMesh() && materialInstanceConstant)
//			{
//				for (int32 i = 0; i < component->GetStaticMesh()->GetStaticMaterials().Num(); i++)
//				{
//					component->SetMaterial(i, materialInstanceConstant);
//				}
//			}
//		}
//		if (path.Equals(resMap[obj].path))
//		{
//			resMap.Remove(obj);
//		}
//	}
//}
//
//// ????????????
//void UPlaceViewLaserBeamComponent::MeshLoadMaterial(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	// ?????????0?????????????????????,?????????????????????????????????
//	if (localInfo->laser.meshComponent->GetInstanceCount() == 0)
//	{
//		//// ??????????????????
//		//auto materialXlsInfo = view->data->GetMaterialInfoWithId(syncInfo->materialid());
//		//if (syncInfo->materialid() != 0 && materialXlsInfo != nullptr)
//		//{
//		//	FString materialName = UTF8_TO_TCHAR(materialXlsInfo->Name_.c_str());
//		//	FString materialPath = UTF8_TO_TCHAR(materialXlsInfo->MaterialPath_.c_str());
//		//	if (localInfo->laser.meshName.IsEmpty() || materialPath.IsEmpty())
//		//	{
//		//		UE_LOG(LogTemp, Log, TEXT("[%x] [MeshLoadMaterial] ERR [%s] [%s] "), this, *localInfo->laser.meshName,
//		//			*materialPath);
//		//		return;
//		//	}
//
//		//	LoadRes(true, materialPath, ELoadResourcesType::LoadRes_Material2, localInfo->laser.meshComponent.Get());
//		//}
//		//// ??????
//		//else
//		//{
//		//}
//	}
//}
//
///*------------------------------------------------------------------*/
//
//// ???????????? ??????????????????
//void UPlaceViewLaserBeamComponent::CreateMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	// ????????????(??????/??????)
//
//	// ????????????
//	if (CreateMeshComponent(syncInfo, localInfo) == false)
//	{
//		return;
//	}
//
//	// ?????????,????????????
//	if (localInfo->laser.meshComponent->GetStaticMesh())
//	{
//		localInfo->laser.meshInstanceIndex = localInfo->laser.meshComponent->AddInstanceWorldSpace(localInfo->laser.laserWTransform);
//		UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [UPlaceViewLaserBeamComponent::CreateMeshInstance] ???????????? [entityId:%d] [xlsId:%d] [name:%s]  "),
//			this, syncInfo->entityid(), syncInfo->xlsid(), *localInfo->laser.meshName);
//		if (ChectMeshInstance(syncInfo, localInfo) == false)
//		{
//			RealUpdateMeshInstance(syncInfo, localInfo);
//		}
//	}
//	// ?????????,????????????
//	else
//	{
//		if (meshInstanceCachingMap.Find(localInfo->laser.meshComponent.Get()))
//		{
//			meshInstanceCachingMap[localInfo->laser.meshComponent.Get()].Add(syncInfo->entityid());
//		}
//		else
//		{
//			TArray<int32> list;
//			list.Add(syncInfo->entityid());
//			meshInstanceCachingMap.Add(localInfo->laser.meshComponent.Get(), list);
//		}
//	}
//}
//
//// ???????????? ??????????????????
//void UPlaceViewLaserBeamComponent::DestroyMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	if (localInfo->laser.meshComponent == nullptr || localInfo->laser.meshComponent->GetInstanceCount() == 0)
//	{
//		return;
//	}
//
//	int32 meshInstanceIndex;
//	bool removeFlag = GetMeshInstance(localInfo->laser.meshComponent.Get(), localInfo->laser.laserWTransform, meshInstanceIndex);
//	if (removeFlag)
//	{
//		bool flag = localInfo->laser.meshComponent->RemoveInstance(meshInstanceIndex);
//		UE_LOG(LogTemp, Log, TEXT("[%x] [DestroyMeshInstance] ???????????? [entityId:%d] [xlsId:%d] [name:%s]  "),
//			this, syncInfo->entityid(), syncInfo->xlsid(), *localInfo->laser.meshName);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Log,
//			TEXT("[%x] [DestroyMeshInstance]  ???????????????????????? 1  [meshInstanceIndex:%d] [xlsId:%d] [count:%d]  "),
//			this, syncInfo->entityid(), syncInfo->xlsid(), localInfo->laser.meshComponent->GetInstanceCount());
//	}
//
//	RealUpdateMeshInstance(syncInfo, localInfo);
//
//	DestroyMeshComponent(syncInfo, localInfo);
//}
//
//// ???????????? ???????????????????????????
//void UPlaceViewLaserBeamComponent::CalculateLocation(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());
//
//	//
//}
//
//// ???????????? ????????????
//void UPlaceViewLaserBeamComponent::UpdateMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());
//
//	localInfo->laser.meshComponent->UpdateInstanceTransform(localInfo->laser.meshInstanceIndex, localInfo->laser.laserWTransform, true,
//		true, true);
//
//	// if (ChectMeshInstance(syncInfo, localInfo) == false)
//	// {
//	// 	RealUpdateMeshInstance(syncInfo, localInfo);
//	// }
//
//	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlaceViewLaserBeamComponent::UpdateMeshInstance]  [%s] "), this, *localInfo->laser.laserWTransform.GetRotation().Rotator().ToString());
//}
//
//// ???????????? ????????????
//bool UPlaceViewLaserBeamComponent::ChectMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	// ????????????11?????????????????????????????????
//
//	bool flag = false;
//	FTransform wTransform;
//	bool flag2 = localInfo->laser.meshComponent->GetInstanceTransform(localInfo->laser.meshInstanceIndex, wTransform, true);
//	if (flag2)
//	{
//		if (localInfo->laser.laserWTransform.Equals(wTransform, 0.2))
//		{
//			flag = true;
//		}
//	}
//	if (flag == false)
//	{
//		UE_LOG(LogTemp, Log, TEXT("[%x] [ChectMeshInstance] ?????????????????? [??????] [bulletIndex:%d]  [a:%s]  [b:%s]"),
//			this, syncInfo->entityid(), *localInfo->laser.laserWTransform.ToString(), *wTransform.ToString());
//		//UKismetSystemLibrary::DrawDebugLine(view->GetWorld(), wTransform.GetLocation(),
//		//                                    localBulletInfo->meshInstanceWTransform.GetLocation(), FColor(0, 255, 255), 0.1, 1.0f);
//	}
//	return flag;
//}
//
//// ???????????? ??????????????????
//void UPlaceViewLaserBeamComponent::RealUpdateMeshInstance(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	// ????????????????????????
//	//for (auto it = view->data->syncEntityInfoMap.CreateConstIterator(); it; ++it)
//
//	for (auto it : view->actorMap)
//	{
//		auto bulletIndex = it.Key;
//		//auto syncInfo2 = view->data->GetSyncInfo(bulletIndex);
//		auto syncInfo2 = &it.Value->syncInfo;
//		if (syncInfo2->xlsid() == syncInfo->xlsid())
//		{
//			auto localInfo2 = view->data->GetLocalInfo(bulletIndex);
//
//			int32 meshInstanceIndex2 = 0;
//			if (GetMeshInstance(localInfo->laser.meshComponent.Get(), localInfo2->laser.laserWTransform, meshInstanceIndex2))
//			{
//				if (localInfo2->laser.meshInstanceIndex != meshInstanceIndex2)
//				{
//					UE_LOG(LogTemp, Log,
//						TEXT(
//							"[%x] [RealUpdateMeshInstance] ?????? [??????] [bulletIndex:%d] [meshInstanceIndex:%d]-->[meshInstanceIndex:%d]"
//						),
//						this, syncInfo2->entityid(), localInfo2->laser.meshInstanceIndex, meshInstanceIndex2);
//					localInfo2->laser.meshInstanceIndex = meshInstanceIndex2;
//				}
//			}
//		}
//	}
//}
//
//// ???????????? ?????????????????????????????????????????????
//bool UPlaceViewLaserBeamComponent::GetMeshInstance(UHierarchicalInstancedStaticMeshComponent* meshComponent, FTransform wTransform,
//	int32& meshInstanceIndex)
//{
//	bool flag = false;
//	meshInstanceIndex = 0;
//	if (meshComponent && meshComponent->GetInstanceCount() > 0)
//	{
//		for (; meshInstanceIndex < meshComponent->GetInstanceCount(); meshInstanceIndex++)
//		{
//			FTransform wTransform2;
//			meshComponent->GetInstanceTransform(meshInstanceIndex, wTransform2, true);
//			if (wTransform.Equals(wTransform2, 0.2))
//			{
//				flag = true;
//				break;
//			}
//		}
//	}
//	return flag;
//}
//
//// ???????????? ??????????????????
//bool UPlaceViewLaserBeamComponent::CreateMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());
//
//	FString meshPath = UTF8_TO_TCHAR(xlsInfo->MeshPath_.c_str());
//	if (localInfo->laser.meshName.IsEmpty() || meshPath.IsEmpty())
//	{
//		UE_LOG(LogTemp, Log, TEXT("[%x] [CreateMeshComponent] ERR [%s] [%s] "), this, *localInfo->laser.meshName, *meshPath);
//		return false;
//	}
//
//	//if (view->data->meshMap.Find(localInfo->laser.meshName) == nullptr)
//	//{
//	//	// ??????????????????
//	//	UHierarchicalInstancedStaticMeshComponent* meshComponent = NewObject<UHierarchicalInstancedStaticMeshComponent>(
//	//		this, *localInfo->laser.meshName);
//	//	meshComponent->RegisterComponent();
//	//	meshComponent->AttachToComponent(view->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
//
//	//	// ??????????????????
//	//	meshComponent->bReceivesDecals = false;
//	//	meshComponent->CastShadow = false;
//
//	//	meshComponent->SetGenerateOverlapEvents(false);
//	//	meshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
//	//	meshComponent->SetCollisionObjectType(BuildObject);
//	//	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	//	meshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
//	//
//	//	LoadRes(true, meshPath, ELoadResourcesType::LoadRes_Mesh, meshComponent);
//
//	//	meshComponent->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator::ZeroRotator);
//
//	//	// ??????????????????
//	//	localInfo->laser.meshComponent = meshComponent;
//
//	//	view->data->meshMap.Add(localInfo->laser.meshName, meshComponent);
//
//	//	//UE_LOG(LogTemp, Log, TEXT("[%x] [CreateMeshComponent] ?????????????????? [mesh:%x] [name:%s] [xlsId:%d] "),
//	//	//       this, localInfo->laser.meshComponent, *localInfo->laser.meshName, xlsInfo->ID_);
//	//}
//	//localInfo->laser.meshComponent = view->data->meshMap[localInfo->laser.meshName];
//	return true;
//}
//
//// ???????????? ??????????????????
//void UPlaceViewLaserBeamComponent::DestroyMeshComponent(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	//if (localInfo->laser.meshComponent == nullptr)return;
//	//if (localInfo->laser.meshComponent->GetInstanceCount() > 0)return;
//
//	//auto temp = localInfo->laser.meshComponent;
//
//	//if (localInfo->laser.meshComponent->IsRegistered())
//	//{
//	//	localInfo->laser.meshComponent->UnregisterComponent();
//	//}
//	//localInfo->laser.meshComponent->DestroyComponent();
//	////UnloadMeshRes(localInfo->laser.meshComponent);
//	//localInfo->laser.meshComponent = nullptr;
//
//	//view->data->meshMap.Remove(localInfo->laser.meshName);
//
//	// UE_LOG(LogTemp, Log, TEXT("[%x] [DestroyMeshComponent] ?????????????????? [mesh:%x] [%s] "), this, temp, *localInfo->laser.meshName);
//}
//
///*------------------------------------------------------------------*/
//
//// ???????????? ??????
//void UPlaceViewLaserBeamComponent::CreateSelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	xlsdata::PlaceTableDefine* xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());
//
//	FString meshPath = UTF8_TO_TCHAR(xlsInfo->MeshPath_.c_str());
//	if (localInfo->laser.meshName.IsEmpty() || meshPath.IsEmpty())
//	{
//		return;
//	}
//
//	UStaticMeshComponent* staticMesh = NewObject<UStaticMeshComponent>(this, *localInfo->laser.selectMeshName);
//	staticMesh->RegisterComponent();
//	staticMesh->AttachToComponent(view->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
//	staticMesh->bReceivesDecals = false;
//	staticMesh->SetGenerateOverlapEvents(false);
//	staticMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
//	staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	staticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
//	staticMesh->SetCastShadow(false);
//
//	LoadRes(true, meshPath, ELoadResourcesType::LoadRes_Mesh3, staticMesh);
//
//	staticMesh->SetWorldTransform(localInfo->laser.laserWTransform);
//	localInfo->laser.selectMesh = staticMesh;
//	cvSelectMeshInfo.isntanceMap.Add(localInfo->laser.selectMesh, syncInfo->entityid());
//}
//
//// ???????????? ??????
//void UPlaceViewLaserBeamComponent::UpdateSelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	if (localInfo->laser.selectMesh == nullptr || IsValid(localInfo->laser.selectMesh) == false)return;
//	localInfo->laser.selectMesh->SetWorldTransform(localInfo->laser.laserWTransform);
//}
//
//// ???????????? ??????
//void UPlaceViewLaserBeamComponent::DestroySelectMesh(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	if (localInfo->laser.selectMesh == nullptr || IsValid(localInfo->laser.selectMesh) == false)return;
//
//	cvSelectMeshInfo.isntanceMap.Remove(localInfo->laser.selectMesh);
//
//	if (localInfo->laser.selectMesh->IsRegistered())
//	{
//		localInfo->laser.selectMesh->UnregisterComponent();
//	}
//	localInfo->laser.selectMesh->DestroyComponent();
//	localInfo->laser.selectMesh = nullptr;
//}
//
//// ???????????? ??????????????????
//void UPlaceViewLaserBeamComponent::LoadSelectMaterial()
//{
//	FString path = TEXT(
//		"MaterialInstanceConstant'/Game/Resources/Material/Selected/Outline/MI_Outline_Inst.MI_Outline_Inst'");
//	selectMaterial = LoadObject<UMaterialInstanceConstant>(NULL, *path);
//}
//
//// ????????????  ??????????????????
//void UPlaceViewLaserBeamComponent::ReplaceSelectMaterial(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
//{
//	if (localInfo->laser.selectMesh == nullptr || IsValid(localInfo->laser.selectMesh) == false)return;
//	for (int32 i = 0; i < localInfo->laser.selectStaticMesh->GetStaticMaterials().Num(); i++)
//	{
//		localInfo->laser.selectMesh->SetMaterial(i, selectMaterial);
//	}
//}
//
///*------------------------------------------------------------------*/
//
