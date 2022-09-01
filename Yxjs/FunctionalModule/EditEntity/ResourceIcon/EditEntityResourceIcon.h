// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EditEntityResourceIcon.generated.h"

class AEditEntityManage;
class UEditEntityChildComponent;

/**
 *
 */
UCLASS()
class  UEditEntityResourceIcon : public UObject
{
	GENERATED_BODY()

public:

	TWeakObjectPtr<AEditEntityManage> owner_;

	// key:mesh路径 value:child变换
	TMap<FString, FTransform> meshTransformMap;

	//
	struct MeshLoadInfo
	{
		TSharedPtr<struct FStreamableHandle> handle;    // 资源句柄
		TArray<TWeakObjectPtr<UEditEntityChildComponent>> childList;  // child列表
		FTransform transform;                           // 相对变换
		FString meshPath;
	};

	// key:mesh路径 value:加载信息
	TMap<FString, MeshLoadInfo> meshLoadMap;

	UEditEntityResourceIcon();

	void Start(TWeakObjectPtr<AEditEntityManage> owner);
	void End();
	void EditEntityResourceIconJob();
	UTexture2D* GetTexture2D(UObject* MeshObject);
	void SetBoxEnter(FString meshPath, TWeakObjectPtr<UEditEntityChildComponent> child);
	bool SearchBoxEnter(FString meshPath, FTransform& transform);
	void LoadMeshStart(bool isStart, FString meshPath, TWeakObjectPtr<UEditEntityChildComponent> child);
	void LoadMesh();
	void Broadcast(MeshLoadInfo& meshLoadInfo);
};
