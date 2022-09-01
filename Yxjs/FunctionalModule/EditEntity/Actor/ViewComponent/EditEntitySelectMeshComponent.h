// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include <string>
#include <memory>
#include <list>
#include <map>

#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "Tools/StringRingBuffer.h"
#include "EditEntitySelectMeshComponent.generated.h"

/**
 *
 */
class ABaseGameMode;
class AEditEntityManage;
class UEditEntityManageController;
class UPlayerRangedWeaponComponent;
class UEditEntityManageData;
class AEditEntityActor;
class UEditEntityChildComponent;

struct FTimerHandle;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterial;
class UBoxComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UImage;
class UMaterialInterface;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UEditEntitySelectMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<AEditEntityActor> owner_;

	//
	TWeakObjectPtr<UEditEntityChildComponent> childComponent;

	//
	int index = 0;

	// 资源加载句柄
	TMap<UObject*, TSharedPtr<struct FStreamableHandle>>handleMap;

	/*------------------------------------------------------------------*/

	//bool IsHightlight();
	//FString GetHightlightMaterialPath();
	//
	UEditEntitySelectMeshComponent();
	void Start(int index_);
	void End();

	void ReplaceMesh();
	void LoadMesh(bool start, FString path, bool loadMaterial);
	void LoadMaterial(bool loadMaterial);
	void LoadMaterialReal(bool start, FString path);
	void ReplaceSelectMaterial(UMaterialInterface* selectMaterial);
	void InitColor();
	void ChangeMaterialsInstanceValue(FVector color);
};
