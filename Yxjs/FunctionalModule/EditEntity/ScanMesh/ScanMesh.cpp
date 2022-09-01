// Fill out your copyright notice in the Description page of Project Settings.


#include "ScanMesh.h"

#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "../EditEntityManage.h"
#include "../Actor/ControllerComponent/EditEntityChildComponent.h"


// 组件模块

//
UScanMesh::UScanMesh()
{

}

//
void UScanMesh::Start(TWeakObjectPtr<AEditEntityManage> owner)
{

	UE_LOG(LogTemp, Log, TEXT("[%x] [扫描网格] [Start] "), this);
	owner = owner_;

	//ScanMeshJob();
}

//
void UScanMesh::End()
{

}

// 测试
void UScanMesh::ScanMeshJob()
{
	FString meshPath1 = TEXT("StaticMesh'");
	FString meshPath2 = TEXT("StaticMesh'");

	auto func = [&](FString meshPath) {

		auto mesh = Cast<UStaticMesh>(LoadObject<UStaticMesh>(NULL, *meshPath));
		if (mesh)
		{

			UE_LOG(LogTemp, Log, TEXT("[%x] [扫描网格] [ScanMeshJob] [%s] [%s] [%s] [%s]"),
				this,

				*mesh->PositiveBoundsExtension.ToString(),
				*mesh->NegativeBoundsExtension.ToString(),
				*mesh->ExtendedBounds.Origin.ToString(),
				*mesh->ExtendedBounds.BoxExtent.ToString()
			);
		}
	};

	func(meshPath1);
	func(meshPath2);
}

// 设置child
void UScanMesh::SetBoxEnter(FString meshPath, TWeakObjectPtr<UEditEntityChildComponent> child)
{

	FTransform transform;
	if (SearchBoxEnter(meshPath, transform))
	{
		child->BoxCallback(false, transform);
	}
	else
	{
		LoadMeshStart(true, meshPath, child);
	}
}

// 查询数据
bool UScanMesh::SearchBoxEnter(FString meshPath, FTransform& transform)
{
	if (auto it = meshTransformMap.Find(meshPath))
	{
		transform = *it;
		return true;
	}
	return false;
}

// 加载资源
void UScanMesh::LoadMeshStart(bool isStart, FString meshPath, TWeakObjectPtr<UEditEntityChildComponent> child)
{

	if (isStart)
	{
		//1. 首次加载
		auto it = meshLoadMap.Find(meshPath);
		if (it == nullptr)
		{

			MeshLoadInfo meshLoadInfo;
			meshLoadInfo.childList.Add(child);
			meshLoadInfo.meshPath = meshPath;
			meshLoadMap.Add(meshPath, meshLoadInfo);

			FSoftObjectPath softObjectPath;
			softObjectPath.SetPath(meshPath);
			meshLoadMap[meshPath].handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObjectPath,
				FStreamableDelegate::CreateUObject(
					this, &UScanMesh::LoadMeshStart,
					false, meshPath, child),
				FStreamableManager::AsyncLoadHighPriority,
				false);

			UE_LOG(LogTemp, Log, TEXT("[%x] [扫描材质] [LoadMeshStart] 1.[首次加载] [meshPath:%s]"), this, *meshPath);
		}
		//2. 加入队列
		else
		{
			it->childList.Add(child);
			UE_LOG(LogTemp, Log, TEXT("[%x] [扫描材质] [LoadMeshStart] 2.[加入队列] [meshPath:%s]"), this, *meshPath);
		}
	}
	else
	{

		auto it = meshLoadMap.Find(meshPath);
		if (it == nullptr)
		{
			return;
		}

		//3. 加载完成
		if (it->handle->IsActive() && it->handle->HasLoadCompleted())
		{
			UStaticMesh* mesh = Cast<UStaticMesh>(it->handle->GetLoadedAsset());
			if (mesh)
			{
				it->transform.SetLocation(mesh->ExtendedBounds.Origin);
				it->transform.SetScale3D(mesh->ExtendedBounds.BoxExtent);

				//ThumbnailInfo
				//mesh->ThumbnailInfo.
				UE_LOG(LogTemp, Log, TEXT("[%x] [扫描材质] [LoadMeshStart] 3.[加载完成] [meshPath:%s]"), this, *meshPath);
				Broadcast(*it);
			}
		}

		//4. 从map中移除资源
		meshLoadMap.Remove(meshPath);
	}
}

// 设置所有数据
void UScanMesh::Broadcast(MeshLoadInfo& meshLoadInfo)
{

	meshTransformMap.Add(meshLoadInfo.meshPath, meshLoadInfo.transform);
	for (auto node : meshLoadInfo.childList)
	{
		if (node.IsValid())
		{
			node->BoxCallback(false, meshLoadInfo.transform);
		}
	}
}
