// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <list>
#include <map>
#include "SceneStaticMeshView.generated.h"

class UBoxComponent;
class UMaterial;
class UBoxComponent;
class USceneComponent;
class USceneStaticMeshComponent;

class UMaterialInterface;

// 资源加载情况
enum ESceneResLoadType
{
	// 未加载
	ResLoadType_None,
	// 加载成功
	ResLoadType_Succesful,
	// 加载失败
	ResLoadType_Failed,
};

// 辅助信息(用于各个结构体传递)
USTRUCT()
struct FTempSceneStaticInfo
{

	GENERATED_BODY()
public:
	ECollisionEnabled::Type openCollision = ECollisionEnabled::QueryOnly;
	bool castShadow = true;
	int entityId = 0;
	FString materialPath = "";
	bool highlight = false;			// 高光 false:基础颜色 true:高光
	FVector color = FVector::ZeroVector;			// 颜色
	int materialId = 0;
};

// 组件信息
USTRUCT()
struct FSceneComponentInfo
{
	GENERATED_BODY()
public:

	// 操作枚举
	enum EOperationType
	{
		EOperationType_add,
		EOperationType_update,
		EOperationType_remove,
		// 该操作丢弃
		EOperationType_none
	};

	// 操作信息
	struct CvOperationInfo
	{
		//
		EOperationType type;

		// id
		int myInstanceId;
	};

	// 名称
	FString componentName = TEXT("");

	// 网格路径
	FString filePath = TEXT("");

	// 网格文件名称
	FString fileName = TEXT("");

	// 组件名称
	UPROPERTY()
		USceneStaticMeshComponent* component = nullptr;

	// 资源句柄
	TSharedPtr<struct FStreamableHandle> handle;

	// 开碰撞与查询
	ECollisionEnabled::Type openCollision = ECollisionEnabled::QueryAndPhysics;

	// 资源是否加载完毕
	ESceneResLoadType isLoadRes = ESceneResLoadType::ResLoadType_None;

	// 操作队列
	std::list< CvOperationInfo> operationList;

	// 辅助信息
	FTempSceneStaticInfo tempInfo;
};

UCLASS()
class  ASceneStaticMeshView : public AActor
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 资源类型
	enum ESceneActorType
	{
		ActorType_None,
		ActorType_Place,
		ActorType_Pack,
		ActorType_Foliage,
		ActorType_Collect,
		ActorType_Item,
	};

	// 静态网格信息
	struct StaticMeshViewInfo
	{
		// 唯一ID
		int myInstanceId = 0;

		// instance mesh id
		int onlyId = 0;

		// 类型
		ESceneActorType type = ActorType_None;

		// 变换
		FTransform transform;

		// 网格路径
		FString meshPath = TEXT("");

		//
		TWeakObjectPtr<USceneStaticMeshComponent> component = nullptr;

		// 加载情况
		bool isLoad = false;

		// 辅助信息
		FTempSceneStaticInfo tempInfo;
	};

	/*------------------------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	// 最大tick数
	int maxCount = 5;

	// log
	bool isLog = false;

	// 当前id
	int thisInstancId = 1000;

	/*------------------------------------------------------------------*/

	// 实例字典 key:myInstanceId 或者 id+index
	std::map<int, StaticMeshViewInfo> instanceMap;

	// 组件字典 key是组件名称
	UPROPERTY()
		TMap<FString, FSceneComponentInfo> componentMap;

	// 操作实例数量
	int operationCount = 0;

	// 启动测试box
	bool useTestBox = false;

	// 测试box
	UPROPERTY()
		TMap<int32, UBoxComponent*> boxMap;

	// 资源加载句柄
	TMap<UObject*, TSharedPtr<struct FStreamableHandle>>handleMap;

	// 查询网格序号
	TMap<FString, int32> meshIdMap;

	// 查询材质序号
	TMap<FString, int32> materialIdMap;

	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	ASceneStaticMeshView();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//
	void Start();

	//
	void End();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Tick_OperationInstance(int& count, FSceneComponentInfo& sceneComponentInfo);
	void CreateInstanceReal(StaticMeshViewInfo& instance, USceneStaticMeshComponent* component);
	virtual void PostInitializeComponents() override;
	bool IsComponentLoadRes(USceneStaticMeshComponent* meshComponent);
	int32 GetMeshPathId(FString path);
	int32 GetMaterailPathId(FString path);
	FString GetNameWithMapId(int myInstanceId);
	USceneStaticMeshComponent* GetMeshComponent(int myInstanceId);
	UMaterialInterface* GetUMaterial(int myInstanceId);
	FString GetNameWithPath(FString path);
	FString GetMeshObejctPath(UStaticMesh* mesh);

	TWeakObjectPtr<USceneStaticMeshComponent> GetEntityMeshComponent(int myInstanceId);
	void LoadMeshRes(bool start, FString path);

	void LoadMaterialRes(bool start, TWeakObjectPtr<UObject> component, FTempSceneStaticInfo tempInfo);
	void SetHighlight(USceneStaticMeshComponent* component_, FTempSceneStaticInfo& tempInfo);
	int CreateInstance(ESceneActorType type, FString path, FTransform transform, bool mast, FTempSceneStaticInfo tempInfo);

	void DestroyInstance(int myInstanceId, bool mast);

	void StartMove();
	void DestroyInstanceReal(USceneStaticMeshComponent* component, int myInstanceId);
	void UpdateInstance(int myInstanceId, FTransform& transform);
	void EndMove();
	void CreateMeshComponent(FString name, FString path, FTempSceneStaticInfo tempInfo);
	void RemoveMeshComponent(FString name);
	void TempInfoAToB(FTempSceneStaticInfo& sourceTempInfo, FTempSceneStaticInfo& targeTempInfo);
	FString GetHightlightMaterialPath(bool highlight);
	bool IsRunJob();
	void CreateBox(USceneStaticMeshComponent* meshComponent, FTransform& worldTransform, int32 myInstanceId, int32 entityId);
	void RemoveBox(int32 myInstanceId);
	void UpdateBox(int32 myInstanceId, FTransform& worldTransform);
};
