// Fill out your copyright notice in the Description page of Project Settings.

#include "SceneStaticMeshView.h"
#include "SceneStaticMeshComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include <Materials/MaterialInstanceConstant.h>
#include <Materials/MaterialInterface.h>

// 功能模块
// view
/*
 * 外部创建实例->创建实例->创建组件->加入tick队列->资源回调完成->tick队列工作
 * 外部移除实例->移除实例->加入tick队列->tick队列工作->移除完成->卸载组件
 *
 */

 /*/////////////////////////////////////////////////////////////*/
 // 数据
 /*/////////////////////////////////////////////////////////////*/

 // 获取 资源回调加载完毕
bool ASceneStaticMeshView::IsComponentLoadRes(USceneStaticMeshComponent* meshComponent)
{
	return false;
}

// 获取网格路径ID
int32 ASceneStaticMeshView::GetMeshPathId(FString path)
{
	if (path == TEXT(""))return 0;

	if (auto node = meshIdMap.Find(path))
	{
		return meshIdMap[path];
	}
	else
	{
		int value = meshIdMap.Num() + 1;
		meshIdMap.Add(path, value);
		return value;
	}
}

// 获取材质路径ID
int32 ASceneStaticMeshView::GetMaterailPathId(FString path)
{
	if (path == TEXT(""))return 0;

	if (auto node = materialIdMap.Find(path))
	{
		return materialIdMap[path];
	}
	else
	{
		int value = materialIdMap.Num() + 1;
		materialIdMap.Add(path, value);
		return value;
	}
}

// 获取 组件名称
FString ASceneStaticMeshView::GetNameWithMapId(int myInstanceId)
{
	auto instance = instanceMap.find(myInstanceId);
	if (instance != instanceMap.end())
	{
		FTempSceneStaticInfo& tempInfo = instance->second.tempInfo;
		FString str = FString::Printf(TEXT("_碰撞_%d_阴影_%d_%d_%d_高光_%d_%.3f_%.3f_%.3f"),
			(int)tempInfo.openCollision, (int)tempInfo.castShadow, GetMeshPathId(instance->second.meshPath), GetMaterailPathId(tempInfo.materialPath)
			, tempInfo.highlight, tempInfo.color.X, tempInfo.color.Y, tempInfo.color.Z
		);
		return "SceneStaticMeshView_" + str;
	}
	else
	{
		return TEXT("");
	}
}

// 获取 组件
USceneStaticMeshComponent* ASceneStaticMeshView::GetMeshComponent(int myInstanceId)
{
	USceneStaticMeshComponent* meshComponent = nullptr;
	auto instance = instanceMap.find(myInstanceId);
	if (instance != instanceMap.end())
	{
		FString componentName = GetNameWithMapId(myInstanceId);
		if (componentMap.Find(componentName) == nullptr)
		{
			FTempSceneStaticInfo& tempInfo = instance->second.tempInfo;
			CreateMeshComponent(componentName, instance->second.meshPath, tempInfo);
		}
		meshComponent = componentMap[componentName].component;
	}
	return meshComponent;
}

// 获取材质
UMaterialInterface* ASceneStaticMeshView::GetUMaterial(int myInstanceId)
{
	UMaterialInterface* ret = nullptr;

	auto component = GetMeshComponent(myInstanceId);
	if (component)
	{
		TArray<class UMaterialInterface*> myList = component->GetMaterials();
		if (myList.Num() > 0)
		{
			ret = myList[0];
		}
	}
	return ret;
}

// split
std::vector<std::string> split(const std::string& s, const std::string& delim)
{
	std::vector<std::string> elems;
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return elems;
	while (pos < len)
	{
		int find_pos = s.find(delim, pos);
		if (find_pos < 0)
		{
			elems.push_back(s.substr(pos, len - pos));
			break;
		}
		elems.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}

// 获取资源名称
FString ASceneStaticMeshView::GetNameWithPath(FString path)
{
	std::string tempStr(TCHAR_TO_UTF8(*path));
	std::vector<std::string> strList = split(tempStr, ".");
	if (strList.size() > 0)
	{
		auto name = strList[strList.size() - 1];
		return UTF8_TO_TCHAR(name.c_str());
	}
	return TEXT("");
}

// 通过资源获取路径
FString ASceneStaticMeshView::GetMeshObejctPath(UStaticMesh* mesh)
{
	FString path = TEXT("");
	if (mesh && mesh->GetPackage())
	{
		std::string tempStr(TCHAR_TO_UTF8(*mesh->GetPackage()->FileName.ToString()));
		std::vector<std::string> strList = split(tempStr, "/");
		if (strList.size() > 0)
		{
			auto name = strList[strList.size() - 1];
			path = "StaticMesh'" + mesh->GetPackage()->FileName.ToString() + "." + UTF8_TO_TCHAR(name.c_str()) + "'";
		}
	}
	return path;
}

// 静态网格 获取实例组件
TWeakObjectPtr<USceneStaticMeshComponent> ASceneStaticMeshView::GetEntityMeshComponent(int myInstanceId)
{
	FString componentName = GetNameWithMapId(myInstanceId);
	if (componentMap.Find(componentName))
	{
		auto& node = componentMap[componentName];

		auto instance = instanceMap.find(myInstanceId);
		if (instance != instanceMap.end())
		{
			return node.component;
		}
	}
	return TWeakObjectPtr<USceneStaticMeshComponent>();
}

// 拷贝数据
void ASceneStaticMeshView::TempInfoAToB(FTempSceneStaticInfo& sourceTempInfo, FTempSceneStaticInfo& targeTempInfo)
{

	targeTempInfo.castShadow = sourceTempInfo.castShadow;
	targeTempInfo.color = sourceTempInfo.color;
	targeTempInfo.entityId = sourceTempInfo.entityId;
	targeTempInfo.highlight = sourceTempInfo.highlight;
	targeTempInfo.materialPath = sourceTempInfo.materialPath;
	targeTempInfo.openCollision = sourceTempInfo.openCollision;

}

// 获取自定义颜色材质路径
FString ASceneStaticMeshView::GetHightlightMaterialPath(bool highlight)
{
	FString path = TEXT("");
	if (highlight)
	{
		path = TEXT("Material'/Game/Resources/Material/CustomColor/M_CustomColor_Highlight_Inst.M_CustomColor_Highlight_Inst'");
	}
	else
	{
		path = TEXT("Material'/Game/Resources/Material/CustomColor/M_CustomColor_Base_Inst.M_CustomColor_Base_Inst'");
	}
	return path;
}

// 队列是否在工作
bool ASceneStaticMeshView::IsRunJob()
{
	return operationCount > 0 ? true : false;
}

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
ASceneStaticMeshView::ASceneStaticMeshView()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;

	sceneRoot->SetShouldUpdatePhysicsVolume(false);

}

//
void ASceneStaticMeshView::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void ASceneStaticMeshView::BeginPlay()
{
	Super::BeginPlay();
	Start();
}

//
void ASceneStaticMeshView::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	End();
	Super::EndPlay(EndPlayReason);
}

//
void ASceneStaticMeshView::Start()
{
}

//
void ASceneStaticMeshView::End()
{
}

/*------------------------------------------------------------------*/

// 调试 box 创建
void ASceneStaticMeshView::CreateBox(USceneStaticMeshComponent* meshComponent, FTransform& worldTransform, int32 myInstanceId, int32 entityId)
{
	if (useTestBox == false)return;

	FString str1 = TEXT("");

	auto instance = instanceMap.find(myInstanceId);
	if (instance != instanceMap.end())
	{

		FTempSceneStaticInfo& tempInfo = instance->second.tempInfo;
		switch (tempInfo.openCollision)
		{
		case ECollisionEnabled::NoCollision:
			str1 = TEXT("无碰撞");
			break;
		case ECollisionEnabled::QueryOnly:
			str1 = TEXT("仅查询");
			break;
		case ECollisionEnabled::QueryAndPhysics:
			str1 = TEXT("查询和碰撞");
			break;
		}
	}
	else
	{
		return;
	}
	FTempSceneStaticInfo& tempInfo = instance->second.tempInfo;

	FString collisionName = FString::Printf(
		TEXT("TestBox_%s_%d_%d"), *str1, myInstanceId, tempInfo.entityId);
	UBoxComponent* boxComponent = NewObject<UBoxComponent>(this, *collisionName);

	boxComponent->RegisterComponent();
	boxComponent->AttachToComponent(meshComponent, FAttachmentTransformRules::KeepWorldTransform); // GetOwner()->GetRootComponent()
	boxComponent->SetBoxExtent(FVector(100, 100, 100));
	worldTransform.SetScale3D(FVector::OneVector);
	boxComponent->SetWorldTransform(worldTransform);

	boxComponent->SetGenerateOverlapEvents(false);
	boxComponent->SetCollisionObjectType(BuildObject);
	boxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComponent->SetEnableGravity(false);
	boxComponent->SetSimulatePhysics(false);

	boxComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	AddInstanceComponent(boxComponent);
	boxComponent->SetHiddenInGame(false);

	boxMap.Add(myInstanceId, boxComponent);
}

// 调试 box 销毁
void ASceneStaticMeshView::RemoveBox(int32 myInstanceId)
{
	if (useTestBox == false)return;

	if (boxMap.Find(myInstanceId))
	{
		UBoxComponent* boxComponent = boxMap[myInstanceId];
		RemoveInstanceComponent(boxComponent);
		boxMap.Remove(myInstanceId);
		if (boxComponent->IsRegistered())
		{
			boxComponent->UnregisterComponent();
		}
		//UE_LOG(LogTemp, Log, TEXT("[%x] [销毁] [ASceneStaticMeshView::RemoveBox] "), this);
		boxComponent->DestroyComponent();
	}
}

// 调试 box 移动
void ASceneStaticMeshView::UpdateBox(int32 myInstanceId, FTransform& worldTransform)
{
	if (useTestBox == false)return;

	if (boxMap.Find(myInstanceId))
	{
		boxMap[myInstanceId]->SetWorldTransform(worldTransform);
	}
}

/*------------------------------------------------------------------*/

// tick
void ASceneStaticMeshView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (operationCount == 0)return;

	int count = 0;
	for (auto it = componentMap.CreateConstIterator(); it; ++it)
	{
		FString key = it.Key();
		FSceneComponentInfo& node = componentMap[key];
		if (node.isLoadRes != ESceneResLoadType::ResLoadType_None)
		{
			if (operationCount > 0 && node.operationList.size() > 0)
			{
				Tick_OperationInstance(count, node);
			}
		}
		if (count > maxCount)break;
	}
}

// tick 操作实例
void ASceneStaticMeshView::Tick_OperationInstance(int& count, FSceneComponentInfo& sceneComponentInfo)
{
	auto& operationList = sceneComponentInfo.operationList;
	if (operationList.size() > 0)
	{
		auto& component = sceneComponentInfo.component;
		if (component)
		{
			while (operationList.size() > 0 && count < maxCount)
			{
				auto it = operationList.begin();
				auto& node = *it;

				// 资源建在成功
				if (sceneComponentInfo.isLoadRes == ESceneResLoadType::ResLoadType_Succesful)
				{
					auto instance = instanceMap.find(node.myInstanceId);
					if (instance != instanceMap.end())
					{
						if (node.type == FSceneComponentInfo::EOperationType::EOperationType_add)
						{
							CreateInstanceReal(instance->second, component);
						}
						else if (node.type == FSceneComponentInfo::EOperationType::EOperationType_remove)
						{
							DestroyInstanceReal(component, node.myInstanceId);
						}
						else if (node.type == FSceneComponentInfo::EOperationType::EOperationType_none)
						{
						}
					}
				}
				// 资源加载失败,啥都不干把队列清理干净
				else if (sceneComponentInfo.isLoadRes == ESceneResLoadType::ResLoadType_Failed)
				{
				}

				if (operationCount > 0)operationCount--;
				operationList.erase(it);

				count++;
			}

			// 队列清空
			if (operationList.size() == 0)
			{
				//if(isLog)UE_LOG(LogTemp, Log, TEXT("[%x] [场景静态物体] [Tick_CreateInstance] 加载完毕 [time:%f] [fileName:%s] "), this,
				//	GetWorld()->TimeSeconds, *sceneComponentInfo.fileName);
			}
		}
	}
}

// tick 创建实例
void ASceneStaticMeshView::CreateInstanceReal(StaticMeshViewInfo& instance, USceneStaticMeshComponent* component)
{
	instance.onlyId = component->AddInstanceWorld(instance.transform);
	instance.component = component;
	instance.isLoad = true;

	FTempSceneStaticInfo& tempInfo = instance.tempInfo;
	CreateBox(component, instance.transform, instance.myInstanceId, tempInfo.entityId);
}

// tick 销毁实例
void ASceneStaticMeshView::DestroyInstanceReal(USceneStaticMeshComponent* component, int myInstanceId)
{
	auto instance = instanceMap.find(myInstanceId);
	if (instance != instanceMap.end())
	{
		RemoveBox(instance->second.myInstanceId);

		bool flag = component->RemoveInstanceWorld(instance->second.onlyId);
		if (flag == false)
		{
			if (isLog)UE_LOG(LogTemp, Error, TEXT("[%x] [场景静态物体] ERR [Tick_OperationInstance] 删除实例失败 [id:%d] "), this, myInstanceId);
		}
		//if (isLog)UE_LOG(LogTemp, Log, TEXT("[%x] [场景静态物体] [DestroyInstance] 销毁实例(实时) [time:%f] [myInstanceId:%d] [name:%s] [flag:%d]"), this,
		//	GetWorld()->TimeSeconds, myInstanceId, *node.fileName, flag);
		instanceMap.erase(instance);
	}
}

/*------------------------------------------------------------------*/

// 静态网格 组件创建
void ASceneStaticMeshView::CreateMeshComponent(FString name, FString path, FTempSceneStaticInfo tempInfo)
{
	USceneStaticMeshComponent* meshComponent = nullptr;

	// 网格组件创建
	meshComponent = NewObject<USceneStaticMeshComponent>(this, *name);
	meshComponent->RegisterComponent();
	meshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	AddInstanceComponent(meshComponent);
	// 设置组件参数
	meshComponent->bReceivesDecals = false;
	meshComponent->CastShadow = true;

	meshComponent->SetGenerateOverlapEvents(false);
	meshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	meshComponent->SetCollisionObjectType(ECC_WorldStatic);

	//Registerac

	// 无碰撞
	if (tempInfo.openCollision == ECollisionEnabled::NoCollision)
	{
		meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		meshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		meshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
		meshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}
	// 纯查询 or 查询+物理
	else
	{
		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		meshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		meshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		meshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
		meshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		meshComponent->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);

		//meshComponent->SetSimulatePhysics(true);
	}

	meshComponent->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator::ZeroRotator);

	FSceneComponentInfo sceneComponentInfo;
	sceneComponentInfo.filePath = path;
	sceneComponentInfo.component = meshComponent;
	sceneComponentInfo.fileName = GetNameWithPath(path);
	sceneComponentInfo.componentName = name;
	//sceneComponentInfo.materialPath = materialPath;
	sceneComponentInfo.tempInfo = tempInfo;
	componentMap.Add(name, sceneComponentInfo);

	FString collsionStr = tempInfo.openCollision == ECollisionEnabled::NoCollision ? TEXT("无碰撞") : TEXT("启用碰撞");
	FString castShadowStr = tempInfo.castShadow ? TEXT("有光照") : TEXT("无光照");
	if (isLog)UE_LOG(LogTemp, Log, TEXT("[%x] [场景静态物体] [CreateMeshComponent] 创建组件 [name:%s] %s %s"),
		this, *sceneComponentInfo.fileName, *collsionStr, *castShadowStr);

	LoadMeshRes(true, name);
}

// 静态网格 组件移除
void ASceneStaticMeshView::RemoveMeshComponent(FString name)
{
	if (componentMap.Find(name))
	{
		auto& node = componentMap[name];
		auto& component = node.component;
		if (component)
		{
			RemoveInstanceComponent(component);
			if (component->IsRegistered())
			{
				component->UnregisterComponent();
			}
			//UE_LOG(LogTemp, Log, TEXT("[%x] [销毁] [ASceneStaticMeshView::RemoveMeshComponent] "), this);
			component->DestroyComponent();

			if (isLog)UE_LOG(LogTemp, Log, TEXT("[%x] [场景静态物体] [RemoveMeshComponent] 移除组件 [销毁] [name:%s]"), this, *node.fileName);
			componentMap.Remove(name);
		}
	}
}

// 静态网格 实例创建
int ASceneStaticMeshView::CreateInstance(ESceneActorType type, FString meshPath, FTransform transform,
	bool mast, FTempSceneStaticInfo tempInfo)
{
	// 资源路径为空
	if (meshPath.Equals(TEXT("")))
	{
		return 0;
	}

	auto tempThisInstancId = thisInstancId;
	thisInstancId++;
	if (thisInstancId > 2147483645)return 0;

	// 创建实例
	StaticMeshViewInfo nodeInfo;
	nodeInfo.type = type;
	nodeInfo.myInstanceId = tempThisInstancId;
	nodeInfo.meshPath = meshPath;
	nodeInfo.transform = transform;
	//nodeInfo.openCollision = openCollision;
	//nodeInfo.castShadow = castShadow;
	//nodeInfo.entityId = entityId;
	//nodeInfo.materialPath = materialPath;
	//TempInfoAToB(tempInfo, nodeInfo.tempInfo);
	nodeInfo.tempInfo = tempInfo;


	instanceMap.emplace(tempThisInstancId, nodeInfo);

	// 获取组件(创建或者直接返回)
	GetMeshComponent(tempThisInstancId);

	// 加入队列
	FString componentName = GetNameWithMapId(tempThisInstancId);
	if (componentMap.Find(componentName))
	{
		auto& node = componentMap[componentName];

		// 当前帧加载

		FSceneComponentInfo::CvOperationInfo operationInfo;

		operationInfo.myInstanceId = tempThisInstancId;
		operationInfo.type = FSceneComponentInfo::EOperationType::EOperationType_add;
		node.operationList.push_back(operationInfo);
		operationCount++;

		if (isLog)UE_LOG(LogTemp, Log, TEXT("[%x] [场景静态物体] [CreateInstance] 创建实例 [time:%f] [myInstanceId:%d] [name:%s]"), this,
			GetWorld()->TimeSeconds, tempThisInstancId, *node.fileName);
	}

	return tempThisInstancId;
}

// 静态网格 实例销毁
void ASceneStaticMeshView::DestroyInstance(int myInstanceId, bool thisFrame)
{
	// 加入队列
	FString componentName = GetNameWithMapId(myInstanceId);
	if (componentMap.Find(componentName))
	{
		auto& node = componentMap[componentName];

		if (thisFrame == false)
		{
			FSceneComponentInfo::CvOperationInfo operationInfo;
			operationInfo.myInstanceId = myInstanceId;
			operationInfo.type = FSceneComponentInfo::EOperationType::EOperationType_remove;
			node.operationList.push_back(operationInfo);
			operationCount++;

			if (isLog)UE_LOG(LogTemp, Log, TEXT("[%x] [场景静态物体] [DestroyInstance] 销毁实例(队列) [time:%f] [myInstanceId:%d] [name:%s]"), this,
				GetWorld()->TimeSeconds, myInstanceId, *node.fileName);
		}
		else
		{
			if (node.isLoadRes == ESceneResLoadType::ResLoadType_Succesful)
			{
				DestroyInstanceReal(node.component, myInstanceId);
			}
		}
	}
}

// 静态网格 实例 当前帧开始移动
void ASceneStaticMeshView::StartMove()
{

}

// 静态网格 实例移动
void ASceneStaticMeshView::UpdateInstance(int myInstanceId, FTransform& transform)
{
	auto it = instanceMap.find(myInstanceId);
	if (it != instanceMap.end())
	{
		if (it->second.isLoad)
		{
			//if (it->second.component->bAutoRebuildTreeOnInstanceChanges)it->second.component->bAutoRebuildTreeOnInstanceChanges = false;

			it->second.component->UpdateInstanceTransformWorld(it->second.onlyId, transform);
		}
	}
}

// 静态网格 实例 当前帧结束移动
void ASceneStaticMeshView::EndMove()
{

}

// 静态网格 网格回调加载
void ASceneStaticMeshView::LoadMeshRes(bool start, FString componentName)
{
	if (componentMap.Find(componentName) == nullptr)
	{
		return;
	}
	auto& componentInfo = componentMap[componentName];
	if (componentInfo.component == nullptr)return;

	if (start)
	{
		FSoftObjectPath softObjectPath;
		softObjectPath.SetPath(componentInfo.filePath);
		componentInfo.handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObjectPath,
			FStreamableDelegate::
			CreateUObject(
				this, &ASceneStaticMeshView::
				LoadMeshRes, false,
				componentName),
			FStreamableManager::
			AsyncLoadHighPriority,
			false);
	}
	else
	{
		UStaticMesh* mesh = Cast<UStaticMesh>(componentInfo.handle->GetLoadedAsset());
		if (mesh)
		{
			bool flag = componentInfo.component->SetStaticMesh(mesh);
			componentInfo.isLoadRes = ESceneResLoadType::ResLoadType_Succesful;
			if (isLog)UE_LOG(LogTemp, Log, TEXT("[%x] [场景静态物体] [LoadRes] 资源加载完成 [path:%s]"), this, *componentInfo.fileName);

			FTempSceneStaticInfo& tempInfo = componentInfo.tempInfo;
			LoadMaterialRes(true, componentInfo.component, tempInfo);
		}
		else
		{
			componentInfo.isLoadRes = ESceneResLoadType::ResLoadType_Failed;
			if (isLog)UE_LOG(LogTemp, Error, TEXT("[%x] [场景静态物体] [LoadRes] 资源加载异常 [path:%s]"), this, *componentInfo.filePath);
		}
	}
}

// 静态网格 材质回调加载
void ASceneStaticMeshView::LoadMaterialRes(bool start, TWeakObjectPtr<UObject> component, FTempSceneStaticInfo tempInfo)
{


	if (component.IsValid() == false)return;
	if (this->IsValidLowLevel() == false)return;// 回调时actor被销毁则不继续执行

	if (start)
	{


		FString path = tempInfo.materialPath;
		if (path == TEXT(""))return;
		if (tempInfo.materialId == 1000)
		{
			path = GetHightlightMaterialPath(tempInfo.highlight);
		}

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
				this, &ASceneStaticMeshView::LoadMaterialRes,
				false, component, tempInfo),
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
				UMaterialInstance* res1 = Cast<UMaterialInstance>(handle_->GetLoadedAsset());
				USceneStaticMeshComponent* component_ = Cast<USceneStaticMeshComponent>(component.Get());
				if (res1 && component_)
				{
					for (auto i = 0; i < component_->GetMaterials().Num(); i++)
					{
						component_->SetMaterial(i, res1);
					}
					SetHighlight(component_, tempInfo);
				}
				//----------------------------------------------------//
			}
		}
		handleMap.Remove(component.Get());
	}
}

// 静态网格 填入高光参数
void ASceneStaticMeshView::SetHighlight(USceneStaticMeshComponent* component_, FTempSceneStaticInfo& tempInfo)
{

	if (tempInfo.materialId != 1000)return;
	if (component_->GetStaticMesh() == nullptr)return;
	for (int32 i = 0; i < component_->GetStaticMesh()->GetStaticMaterials().Num(); i++)
	{
		auto MatDynamic = component_->CreateAndSetMaterialInstanceDynamic(i);
		MatDynamic->SetVectorParameterValue(TEXT("Color"), tempInfo.color);
	}
}

/*------------------------------------------------------------------*/
