// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "SceneStaticMeshComponent.generated.h"

/**
 *
 */
UCLASS()
class  USceneStaticMeshComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:

	// 唯一id
	int onlyId = 1000;

	// 唯一ID对应索引表
	TMap<int, int> onlyIdIndexMap;

	// 唯一id列表
	TArray<int> onlyIdList;

	//
	int32 AddInstanceWorld(const FTransform& InstanceTransform);

	void ShowLog();
	//
	bool RemoveInstanceWorld(int32 InstanceIndex);
	void UpdateInstanceTransformWorld(int32 onlyId_, const FTransform& WorldTransform);
};
