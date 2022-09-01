// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceViewImageComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewImageComponent.h"
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

//#include<vector>

/*
data
*/

/*------------------------------------------------------------------*/

// UPlaceViewImageComponent
UPlaceViewImageComponent::UPlaceViewImageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// BeginPlay
void UPlaceViewImageComponent::Start(AEditEntityManage* view_)
{
	view = view_;
}

// EndPlay
void UPlaceViewImageComponent::End()
{
}

/*------------------------------------------------------------------*/

// 图片组件 创建
void UPlaceViewImageComponent::CreateImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//auto xlsInfo = view->data->GetXlsInfoWithXlsId(syncInfo->xlsid());

	//UClass* class_ = LoadClass<UUserWidget>(
	//	view->GetWorld(), TEXT("WidgetBlueprint'/Game/Yxjs/BP/FunctionalModule/EditEntity/BP_UI_Image.BP_UI_Image_C'"));
	//if (class_ == nullptr)
	//{
	//	return;
	//}
	//// 正面
	//{
	//	FString name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());
	//	FString name2 = FString::Printf(TEXT("Image_A_%d_%s"), syncInfo->entityid(), *name);
	//	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(this, *name2);
	//	widgetComponent->RegisterComponent();
	//	widgetComponent->AttachToComponent(view->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	//	{
	//		auto t2 = syncInfo->transform();
	//		auto t = UCommonlyObject::PBToUe4Transform(&t2);
	//		// FTransform t = syncInfo->spawnWTransform;
	//		auto scale = t.GetScale3D();
	//		scale.X = 1;
	//		t.SetScale3D(scale);
	//		widgetComponent->SetWorldTransform(t);
	//	}
	//	widgetComponent->SetWidgetClass(class_);

	//	localInfo->image.widgetPositiveComponent = widgetComponent;
	//}
	//// 背面
	//{
	//	FString name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());
	//	FString name2 = FString::Printf(TEXT("Image_B_%d_%s"), syncInfo->entityid(), *name);
	//	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(this, *name2);
	//	widgetComponent->RegisterComponent();
	//	widgetComponent->AttachToComponent(view->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	//	{
	//		auto t2 = syncInfo->transform();
	//		auto t = UCommonlyObject::PBToUe4Transform(&t2);
	//		// FTransform t = syncInfo->spawnWTransform;
	//		auto scale = t.GetScale3D();
	//		scale.X = -1;
	//		t.SetScale3D(scale);
	//		widgetComponent->SetWorldTransform(t);
	//	}
	//	widgetComponent->SetWidgetClass(class_);

	//	localInfo->image.widgetBackComponent = widgetComponent;
	//}

	//// 设置图片
	//CallUIReadySetImage(syncInfo, localInfo);
	//// UE_LOG(LogTemp, Log, TEXT("[%x] [CreateImage] 创建图片 [entityId:%d] [name:%s]  "),
	////        this, syncInfo->entityid(), *localInfo->mesh.meshName);
}

// 图片组件 更新
void UPlaceViewImageComponent::UpdateImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//if (localInfo->image.widgetPositiveComponent == nullptr || IsValid(localInfo->image.widgetPositiveComponent) ==
	//	false)return;
	//if (localInfo->image.widgetBackComponent == nullptr || IsValid(localInfo->image.widgetBackComponent) == false)
	//	return;

	//{
	//	auto t2 = syncInfo->transform();
	//	auto t = UCommonlyObject::PBToUe4Transform(&t2);
	//	// FTransform t = syncInfo->spawnWTransform;
	//	auto scale = t.GetScale3D();
	//	scale.X = 1;
	//	t.SetScale3D(scale);
	//	localInfo->image.widgetPositiveComponent->SetWorldTransform(t);
	//}

	//{
	//	auto t2 = syncInfo->transform();
	//	auto t = UCommonlyObject::PBToUe4Transform(&t2);
	//	// FTransform t = syncInfo->spawnWTransform;
	//	auto scale = t.GetScale3D();
	//	scale.X = -1;
	//	t.SetScale3D(scale);
	//	localInfo->image.widgetBackComponent->SetWorldTransform(t);
	//}

	//// url更变则更新图片
	//CallUIReadySetImage(syncInfo, localInfo);

	// UE_LOG(LogTemp, Log, TEXT("[%x] [UpdateImage]  [entityId:%d] [GetScale3D:%s]  "),
	//        this, syncInfo->entityid(), *syncInfo->spawnWTransform.GetScale3D().ToString());
	// SetImageSize(syncInfo, localInfo);
}

// 图片组件 销毁
void UPlaceViewImageComponent::DestroyImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//if (localInfo->image.widgetPositiveComponent == nullptr || IsValid(localInfo->image.widgetPositiveComponent) ==
	//	false)return;
	//if (localInfo->image.widgetBackComponent == nullptr || IsValid(localInfo->image.widgetBackComponent) == false)
	//	return;

	//// UE_LOG(LogTemp, Log, TEXT("[%x] [DestroyImage] 销毁图片 [entityId:%d] [widgetPositiveComponent:%x] [name:%s]  "),
	////        this, syncInfo->entityid(), localInfo->image.widgetPositiveComponent, *localInfo->mesh.meshName);

	//{
	//	auto userWidgetObject = localInfo->image.widgetPositiveComponent->GetUserWidgetObject();
	//	if (userWidgetObject)
	//	{
	//		FString cmd = FString::Printf(TEXT("Event_End"));
	//		FOutputDeviceDebug device;
	//		userWidgetObject->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
	//	}
	//	if (localInfo->image.widgetPositiveComponent->IsRegistered())
	//	{
	//		localInfo->image.widgetPositiveComponent->UnregisterComponent();
	//	}
	//	localInfo->image.widgetPositiveComponent->DestroyComponent();
	//	localInfo->image.widgetPositiveComponent = nullptr;
	//}
	//{
	//	auto userWidgetObject = localInfo->image.widgetBackComponent->GetUserWidgetObject();
	//	if (userWidgetObject)
	//	{
	//		FString cmd = FString::Printf(TEXT("Event_End"));
	//		FOutputDeviceDebug device;
	//		userWidgetObject->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
	//	}
	//	if (localInfo->image.widgetBackComponent->IsRegistered())
	//	{
	//		localInfo->image.widgetBackComponent->UnregisterComponent();
	//	}
	//	localInfo->image.widgetBackComponent->DestroyComponent();
	//	localInfo->image.widgetBackComponent = nullptr;
	//}
}

// 图片组件 通知UI结束设置图片
void UPlaceViewImageComponent::CallUIOverSetImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo,
	UImage* image,
	TWeakObjectPtr<UTexture2D> texture2D)
{
	//// UE_LOG(LogTemp, Log, TEXT("[%x] [SetImageSize] 设置绘制大小 [entityId:%d] [widgetPositiveComponent:%x] [name:%s]  "),
	////        this, syncInfo->entityid(), localInfo->image.widgetPositiveComponent, *localInfo->mesh.meshName);

	//if (localInfo->image.widgetPositiveComponent == nullptr || IsValid(localInfo->image.widgetPositiveComponent) ==
	//	false)return;
	//if (localInfo->image.widgetBackComponent == nullptr || IsValid(localInfo->image.widgetBackComponent) == false)
	//	return;

	//if (image && texture2D.IsValid())
	//{
	//	image->SetBrushFromTexture(texture2D.Get());
	//	auto imageX = texture2D->GetSizeX();
	//	auto imageY = texture2D->GetSizeY();

	//	UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(image->Slot);
	//	if (canvasPanelSlot)
	//	{
	//		FMargin margin;
	//		margin.Left = -(imageX / 2);
	//		margin.Top = -(imageY / 2);
	//		margin.Right = imageX;
	//		margin.Bottom = imageY;
	//		canvasPanelSlot->LayoutData.Offsets = margin;
	//		auto temp = canvasPanelSlot->LayoutData;
	//		temp.Offsets = margin;
	//		canvasPanelSlot->SetLayout(temp);
	//	}

	//	auto brushX = image->Brush.ImageSize.X;
	//	auto brushY = image->Brush.ImageSize.Y;

	//	image->RenderTransform.Scale.X = brushX;
	//	image->RenderTransform.Scale.Y = brushY;

	//	image->SetBrushSize(FVector2D(imageX, imageY));
	//}

	//localInfo->image.widgetPositiveComponent->SetDrawSize(localInfo->image.imageSize);
	//localInfo->image.widgetBackComponent->SetDrawSize(localInfo->image.imageSize);
}

// 图片组件 通知UI准备设置图片
void UPlaceViewImageComponent::CallUIReadySetImage(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo)
{
	//if (localInfo->image.widgetPositiveComponent == nullptr || IsValid(localInfo->image.widgetPositiveComponent) ==
	//	false)return;
	//if (localInfo->image.widgetBackComponent == nullptr || IsValid(localInfo->image.widgetBackComponent) == false)
	//	return;

	//std::string url2 = syncInfo->child_list(0).image().url();
	//FString url = UCommonlyObject::StringToFString(url2);
	//if (url.IsEmpty() == false
	//	&& url.Equals(localInfo->image.url) == false)
	//{
	//	{
	//		auto userWidgetObject = localInfo->image.widgetPositiveComponent->GetUserWidgetObject();
	//		if (userWidgetObject)
	//		{
	//			FString cmd = FString::Printf(TEXT("Event_Start %s %d"), *url, syncInfo->entityid());
	//			FOutputDeviceDebug device;
	//			userWidgetObject->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
	//		}
	//	}
	//	{
	//		auto userWidgetObject = localInfo->image.widgetBackComponent->GetUserWidgetObject();
	//		if (userWidgetObject)
	//		{
	//			FString cmd = FString::Printf(TEXT("Event_Start %s %d"), *url, syncInfo->entityid());
	//			FOutputDeviceDebug device;
	//			userWidgetObject->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
	//		}
	//	}
	//}
	//localInfo->image.url = url;
}

/*------------------------------------------------------------------*/