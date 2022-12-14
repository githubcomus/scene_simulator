// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntitySelectMeshComponent.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
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
#include "FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"

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
#include <Materials/MaterialInterface.h>


/*

*/

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

//// ?????????????????????
//bool UEditEntitySelectMeshComponent::IsHightlight() {
//
//	if (childComponent->childSyncInfo->mesh().material_id() == 1000)return true;
//	return false;
//}
//
//// ?????????????????????????????????
//FString UEditEntitySelectMeshComponent::GetHightlightMaterialPath() {
//	FString path = TEXT("");
//	if (childComponent->childSyncInfo->mesh().highlight()) {
//		path = TEXT("Material'/Game/Resources/Material/CustomColor/M_CustomColor_Highlight_Inst.M_CustomColor_Highlight_Inst'");
//	}
//	else {
//		path = TEXT("Material'/Game/Resources/Material/CustomColor/M_CustomColor_Base_Inst.M_CustomColor_Base_Inst'");
//	}
//	return path;
//}

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// ?????????
/*/////////////////////////////////////////////////////////////*/

// UEditEntitySelectMeshComponent
UEditEntitySelectMeshComponent::UEditEntitySelectMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bReceivesDecals = false;
	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCastShadow(true);

	SetRenderCustomDepth(true);
	SetCustomDepthStencilValue(251);

}

// BeginPlay
void UEditEntitySelectMeshComponent::Start(int index_)
{
	owner_ = Cast<AEditEntityActor>(GetOwner());
	index = index_;
	childComponent = owner_->childComponentList[index];
	FString meshPath = UTF8_TO_TCHAR(childComponent->childMeshXlsInfo->SelectMeshPath_.c_str());

	LoadMesh(true, meshPath, false);
}

// EndPlay
void UEditEntitySelectMeshComponent::End()
{
}

// ????????????,??????view component ???????????????,???????????????????????????????????????,??????????????????????????????.
void UEditEntitySelectMeshComponent::ReplaceMesh()
{

	FString meshPath = UTF8_TO_TCHAR(childComponent->childMeshXlsInfo->SelectMeshPath_.c_str());
	LoadMesh(true, meshPath, true);
}

// ????????????
void UEditEntitySelectMeshComponent::LoadMesh(bool start, FString path, bool loadMaterial)
{
	TWeakObjectPtr<USceneComponent> component = this;
	if (component.IsValid() == false)return;
	if (this->IsValidLowLevel() == false)return;// ?????????actor???????????????????????????

	if (start)
	{
		if (handleMap.Find(component.Get()))
		{
			handleMap.Remove(component.Get());
		}
		TSharedPtr<FStreamableHandle> handle;
		handleMap.Add(component.Get(), handle);

		FSoftObjectPath softObjectPath;
		softObjectPath.SetPath(path);
		handleMap[component.Get()] = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObjectPath,
			FStreamableDelegate::CreateUObject(
				this, &UEditEntitySelectMeshComponent::LoadMesh,
				false, path, loadMaterial),
			FStreamableManager::AsyncLoadHighPriority,
			false);
	}
	else
	{
		if (handleMap.Find(component.Get()) == nullptr)
		{
			return;
		}

		bool flag = false;
		auto& handle_ = handleMap[component.Get()];
		if (handle_ && handle_.IsValid() && component.IsValid())
		{
			if (handle_->IsActive() && handle_->HasLoadCompleted())
			{
				//----------------------------------------------------//
				UStaticMesh* ass = Cast<UStaticMesh>(handle_->GetLoadedAsset());
				UStaticMeshComponent* pComponent = Cast<UStaticMeshComponent>(component.Get());
				if (ass && pComponent)
				{
					pComponent->SetStaticMesh(ass);
					flag = true;
				}
				//----------------------------------------------------//
			}
		}
		handleMap.Remove(component.Get());
		if (flag)
		{
			LoadMaterial(loadMaterial);
		}
	}
}

// ????????????
void UEditEntitySelectMeshComponent::LoadMaterial(bool loadMaterial)
{
	// ?????????????????????????????????
	if (loadMaterial)
	{
		FString path = TEXT("");

		TWeakObjectPtr < UEditEntityManageData> data = owner_->editEntityManage->data;
		EMyMeshMaterialType myMeshMaterialType = data->GetChildMaterialType(&owner_->syncInfo, childComponent->index);
		if (myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_StaticNormalColor
			|| myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_DynamicNormalColor
			)
		{
			path = data->GetHightlightMaterialPath(false);
		}
		else if (myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_StaticHighhight
			|| myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_DynamicHighhight)
		{
			path = data->GetHightlightMaterialPath(true);
		}
		else if (myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_WhiteMold)
		{
			return;
		}
		else if (myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_OrdinaryMaterial)
		{
			path = UTF8_TO_TCHAR(childComponent->childMaterialXlsInfo->MaterialPath_.c_str());
		}

		LoadMaterialReal(true, path);
	}
	// ?????????????????????????????????????????????????????????
	else
	{
		if (owner_->IsDynamicColorChange())
		{
			UMaterialInterface* materialInterface = childComponent->colorMeshComponent->GetUMaterial();
			ReplaceSelectMaterial(materialInterface);
		}
		else
		{
			UMaterialInterface* materialInterface = owner_->gameMode->sceneStaticMeshView->GetUMaterial(childComponent->meshIstanceId);
			ReplaceSelectMaterial(materialInterface);
		}
	}
}

// ????????????
void UEditEntitySelectMeshComponent::LoadMaterialReal(bool start, FString path)
{
	if (path == TEXT(""))return;

	TWeakObjectPtr<USceneComponent> component = this;
	if (component.IsValid() == false)return;
	if (this->IsValidLowLevel() == false)return;// ?????????actor???????????????????????????

	if (start)
	{
		if (handleMap.Find(component.Get()))
		{
			handleMap.Remove(component.Get());
		}
		TSharedPtr<FStreamableHandle> handle;
		handleMap.Add(component.Get(), handle);

		FSoftObjectPath softObjectPath;
		softObjectPath.SetPath(path);
		handleMap[component.Get()] = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObjectPath,
			FStreamableDelegate::CreateUObject(
				this, &UEditEntitySelectMeshComponent::LoadMaterialReal,
				false, path),
			FStreamableManager::AsyncLoadHighPriority,
			false);
	}
	else
	{
		if (handleMap.Find(component.Get()) == nullptr)
		{
			return;
		}
		auto& handle_ = handleMap[component.Get()];
		if (handle_ && handle_.IsValid() && component.IsValid())
		{
			if (handle_->IsActive() && handle_->HasLoadCompleted())
			{
				//----------------------------------------------------//
				UMaterialInterface* ass = Cast<UMaterialInterface>(handle_->GetLoadedAsset());
				UStaticMeshComponent* pComponent = Cast<UStaticMeshComponent>(component.Get());
				if (ass && pComponent)
				{
					ReplaceSelectMaterial(ass);
				}
				//----------------------------------------------------//
			}
		}
		handleMap.Remove(component.Get());
	}
}

// ????????????
void UEditEntitySelectMeshComponent::ReplaceSelectMaterial(UMaterialInterface* selectMaterial)
{
	if (GetStaticMesh() == nullptr || selectMaterial == nullptr)return;


	for (int32 i = 0; i < GetStaticMesh()->GetStaticMaterials().Num(); i++)
	{
		SetMaterial(i, selectMaterial);
	}

	TWeakObjectPtr < UEditEntityManageData> data = owner_->editEntityManage->data;
	EMyMeshMaterialType myMeshMaterialType = data->GetChildMaterialType(&owner_->syncInfo, childComponent->index);

	// ????????????/??????
	if (myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_StaticNormalColor
		|| myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_StaticHighhight
		)
	{

		Gamedata::FVector color = childComponent->childSyncInfo->mesh().color();
		for (int32 i = 0; i < GetStaticMesh()->GetStaticMaterials().Num(); i++)
		{
			auto MatDynamic = CreateAndSetMaterialInstanceDynamic(i);
			MatDynamic->SetVectorParameterValue(TEXT("Color"), FVector(color.x(), color.y(), color.z()));
		}
	}
	// ????????????/??????
	else if (myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_DynamicNormalColor
		|| myMeshMaterialType == EMyMeshMaterialType::EMyMeshMaterialType_DynamicHighhight)
	{

		UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [ReplaceSelectMaterial] ????????????/?????? [entityId:%d]  [childIndex:%d]"), this, owner_->entityId, childComponent->index);
		// ??????????????????????????????
		//owner_->
		InitColor();
	}



}

// ??????????????????????????????????????????
void UEditEntitySelectMeshComponent::InitColor()
{

	FVector color = owner_->myTimelineComponent->GetActorTimelineColor(0);
	ChangeMaterialsInstanceValue(color);
}

// ????????????????????????
void UEditEntitySelectMeshComponent::ChangeMaterialsInstanceValue(FVector color)
{

	UStaticMesh* mesh = GetStaticMesh();
	if (mesh == nullptr)return;
	TArray<FStaticMaterial>& materialList = mesh->GetStaticMaterials();
	if (materialList.Num() == 0)return;

	for (int32 i = 0; i < materialList.Num(); i++)
	{
		auto MatDynamic = CreateAndSetMaterialInstanceDynamic(i);
		MatDynamic->SetVectorParameterValue(TEXT("Color"), color);
	}
	//	//MatDynamic->SetScalarParameterValue(FName("DiffuseBrightness"), 1.0f); //??????  
}
