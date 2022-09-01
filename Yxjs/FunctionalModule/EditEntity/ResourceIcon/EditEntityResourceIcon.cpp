// Fill out your copyright notice in the Description page of Project Settings.


#include "EditEntityResourceIcon.h"

#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "../EditEntityManage.h"
#include "../Actor/ControllerComponent/EditEntityChildComponent.h"
//#include <ObjectTools.h>
#include <IImageWrapper.h>
#include <IImageWrapperModule.h>
#include <Kismet/KismetRenderingLibrary.h>
#include <Engine/Texture2D.h>


// 资源图标

//
UEditEntityResourceIcon::UEditEntityResourceIcon()
{

}

//
void UEditEntityResourceIcon::Start(TWeakObjectPtr<AEditEntityManage> owner)
{

	UE_LOG(LogTemp, Log, TEXT("[%x] [资源图标] [Start] "), this);
	owner = owner_;

	EditEntityResourceIconJob();
}

//
void UEditEntityResourceIcon::End()
{

}

//
void UEditEntityResourceIcon::EditEntityResourceIconJob()
{
	FString meshPath1 = TEXT("StaticMesh'/Game/ResPackage/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
	//FString meshPath2 = TEXT("StaticMesh'");

	auto func = [&](FString meshPath) {

		auto mesh = Cast<UStaticMesh>(LoadObject<UStaticMesh>(NULL, *meshPath));
		if (mesh)
		{

			//UE_LOG(LogTemp, Log, TEXT("[%x] [扫描网格] [EditEntityResourceIconJob] [%s] [%s] [%s] [%s]"),
			//	this,

			UTexture2D* asdf = GetTexture2D(mesh);
			int i = 0;
		}
	};

	func(meshPath1);
	//func(meshPath2);
}

UTexture2D* UEditEntityResourceIcon::GetTexture2D(UObject* MeshObject)
{


	//int32 _imageRes = 128;
	//FObjectThumbnail _objectThumnail;
	//ThumbnailTools::RenderThumbnail(MeshObject, _imageRes, _imageRes, ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush, NULL, &_objectThumnail);
	//TArray<uint8> _myData = _objectThumnail.GetUncompressedImageData();

	//TArray<FColor> _imageRawColor;
	//for (int i = 0; i < _myData.Num(); i += 4)
	//{
	//	_imageRawColor.Add(FColor(_myData[i + 2], _myData[i + 1], _myData[i], _myData[i + 3]));
	//}
	////FString _fileName = OutputPath.ToString() + "/" + obj->GetName() + FString(".jpg");
	//IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	//TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	//ImageWrapper->SetRaw(_imageRawColor.GetData(), _imageRawColor.GetAllocatedSize(), _imageRes, _imageRes, ERGBFormat::BGRA, 8);
	//const TArray64<uint8>& _ImageData = ImageWrapper->GetCompressed(100);
	//TArray<uint8> tempData;
	//for (auto node : _ImageData) {
	//	tempData.Add(node);
	//}
	//UTexture2D* MyTexture2D = UKismetRenderingLibrary::ImportBufferAsTexture2D(GetWorld(), tempData);
	//return MyTexture2D;

	return nullptr;
}


// 设置child
void UEditEntityResourceIcon::SetBoxEnter(FString meshPath, TWeakObjectPtr<UEditEntityChildComponent> child)
{

	//FTransform transform;
	//if (SearchBoxEnter(meshPath, transform)) {
	//	child->BoxCallback(false, transform);
	//}
	//else {
	//	LoadMeshStart(true, meshPath, child);
	//}
}

// 查询数据
bool UEditEntityResourceIcon::SearchBoxEnter(FString meshPath, FTransform& transform)
{
	//if (auto it = meshTransformMap.Find(meshPath)) {
	//	transform = *it;
	//	return true;
	//}
	return false;
}

// 加载资源
void UEditEntityResourceIcon::LoadMeshStart(bool isStart, FString meshPath, TWeakObjectPtr<UEditEntityChildComponent> child)
{

	//if (isStart) {
	//	//1. 首次加载
	//	auto it = meshLoadMap.Find(meshPath);
	//	if (it == nullptr) {

	//		MeshLoadInfo meshLoadInfo;
	//		meshLoadInfo.childList.Add(child);
	//		meshLoadInfo.meshPath = meshPath;
	//		meshLoadMap.Add(meshPath, meshLoadInfo);

	//		FSoftObjectPath softObjectPath;
	//		softObjectPath.SetPath(meshPath);
	//		meshLoadMap[meshPath].handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObjectPath,
	//			FStreamableDelegate::CreateUObject(
	//				this, &UEditEntityResourceIcon::LoadMeshStart,
	//				false, meshPath, child),
	//			FStreamableManager::AsyncLoadHighPriority,
	//			false);

	//		UE_LOG(LogTemp, Log, TEXT("[%x] [扫描材质] [LoadMeshStart] 1.[首次加载] [meshPath:%s]"), this, *meshPath);
	//	}
	//	//2. 加入队列
	//	else {
	//		it->childList.Add(child);
	//		UE_LOG(LogTemp, Log, TEXT("[%x] [扫描材质] [LoadMeshStart] 2.[加入队列] [meshPath:%s]"), this, *meshPath);
	//	}
	//}
	//else {

	//	auto it = meshLoadMap.Find(meshPath);
	//	if (it == nullptr) {
	//		return;
	//	}

	//	//3. 加载完成
	//	if (it->handle->IsActive() && it->handle->HasLoadCompleted()) {
	//		UStaticMesh* mesh = Cast<UStaticMesh>(it->handle->GetLoadedAsset());
	//		if (mesh) {
	//			it->transform.SetLocation(mesh->ExtendedBounds.Origin);
	//			it->transform.SetScale3D(mesh->ExtendedBounds.BoxExtent);

	//			//ThumbnailInfo
	//			//mesh->ThumbnailInfo.
	//			UE_LOG(LogTemp, Log, TEXT("[%x] [扫描材质] [LoadMeshStart] 3.[加载完成] [meshPath:%s]"), this, *meshPath);
	//			Broadcast(*it);
	//		}
	//	}

	//	//4. 从map中移除资源
	//	meshLoadMap.Remove(meshPath);
	//}
}

// 设置所有数据
void UEditEntityResourceIcon::Broadcast(MeshLoadInfo& meshLoadInfo)
{

	//meshTransformMap.Add(meshLoadInfo.meshPath, meshLoadInfo.transform);
	//for (auto node : meshLoadInfo.childList) {
	//	if (node.IsValid()) {
	//		node->BoxCallback(false, meshLoadInfo.transform);
	//	}
	//}
}
