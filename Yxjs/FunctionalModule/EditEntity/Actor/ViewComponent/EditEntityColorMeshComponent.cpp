// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityColorMeshComponent.h"
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

?????????????????????????????????
???????????????????????????
????????????????????????????????????

*/

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

// UEditEntityColorMeshComponent
UEditEntityColorMeshComponent::UEditEntityColorMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bReceivesDecals = false;
	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCastShadow(false);
}

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

FString UEditEntityColorMeshComponent::GetPath()
{

	if (owner_->syncInfo.has_transform_timeline() == false)return TEXT("");

	FString path = owner_->editEntityManage->data->GetHightlightMaterialPath(owner_->syncInfo.transform_timeline().timeline_highlight() == Gamedata::TimelineHighlightType::TimelineHighlightType_hightlight ? true : false);
	return path;
}

/*/////////////////////////////////////////////////////////////*/
// ?????????
/*/////////////////////////////////////////////////////////////*/

// BeginPlay
void UEditEntityColorMeshComponent::Start(int index_)
{
	owner_ = Cast<AEditEntityActor>(GetOwner());
	index = index_;
	childComponent = owner_->childComponentList[index];
	FString meshPath = UTF8_TO_TCHAR(childComponent->childMeshXlsInfo->MeshPath_.c_str());
	LoadMesh(true, meshPath);

}

// EndPlay
void UEditEntityColorMeshComponent::End()
{
}

// ????????????
void UEditEntityColorMeshComponent::LoadMesh(bool start, FString path)
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
				this, &UEditEntityColorMeshComponent::LoadMesh,
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
			FString materialPath_ = GetPath();//UTF8_TO_TCHAR(childComponent->childMaterialXlsInfo->MaterialPath_.c_str());
			LoadMaterial(true, materialPath_);
		}
	}
}

// ????????????
void UEditEntityColorMeshComponent::LoadMaterial(bool start, FString path)
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
				this, &UEditEntityColorMeshComponent::LoadMaterial,
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
				UMaterialInstanceConstant* ass = Cast<UMaterialInstanceConstant>(handle_->GetLoadedAsset());
				UStaticMeshComponent* pComponent = Cast<UStaticMeshComponent>(component.Get());
				if (ass && pComponent)
				{
					ReplaceSelectMaterial(ass);
					InitColor();
				}
				//----------------------------------------------------//
			}
		}
		handleMap.Remove(component.Get());
	}
}

// ????????????
void UEditEntityColorMeshComponent::ReplaceSelectMaterial(UMaterialInstanceConstant* selectMaterial)
{
	matericalList.Reset();
	if (GetStaticMesh() == nullptr || selectMaterial == nullptr)return;
	for (int32 i = 0; i < GetStaticMesh()->GetStaticMaterials().Num(); i++)
	{
		SetMaterial(i, selectMaterial);
		auto MatDynamic = CreateAndSetMaterialInstanceDynamic(i);
		matericalList.Add(MatDynamic);
	}
}

// ??????????????????????????????????????????
void UEditEntityColorMeshComponent::InitColor()
{
	FVector color = owner_->myTimelineComponent->GetActorTimelineColor(0);
	ChangeMaterialsInstanceValue(color);
}

// ????????????????????????
void UEditEntityColorMeshComponent::ChangeMaterialsInstanceValue(FVector color)
{

	for (auto node : matericalList)
	{
		node->SetVectorParameterValue(TEXT("Color"), color);
	}
	//	//MatDynamic->SetScalarParameterValue(FName("DiffuseBrightness"), 1.0f); //??????  
}

// ????????????/????????????
void UEditEntityColorMeshComponent::ChangeHighlight()
{

	//FString materialPath_ = UTF8_TO_TCHAR(childComponent->childMaterialXlsInfo->MaterialPath_.c_str());

	FString materialPath_ = GetPath();
	LoadMaterial(true, materialPath_);
}

// ????????????
UMaterialInterface* UEditEntityColorMeshComponent::GetUMaterial()
{
	UMaterialInterface* ret = nullptr;

	TArray<class UMaterialInterface*> myList = GetMaterials();
	if (myList.Num() > 0)
	{
		ret = myList[0];
	}
	return ret;
}