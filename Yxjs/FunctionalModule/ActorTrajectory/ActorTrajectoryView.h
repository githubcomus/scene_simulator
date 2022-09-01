// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <list>
#include <map>
#include <vector>
#include "ActorTrajectoryView.generated.h"


class UBoxComponent;
class UMaterial;
class UBoxComponent;
class USceneComponent;
class USceneStaticMeshComponent;
class APawnLineActor;

////using namespace std;

UCLASS()
class  AActorTrajectoryView : public AActor
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//// 资源类型
	//enum ESceneActorType
	//{
	//	ActorType_None,
	//	ActorType_Pack,
	//	ActorType_Foliage,
	//	ActorType_Collect,
	//	ActorType_Item,
	//};


	//// 静态网格信息
	//struct StaticMeshViewInfo
	//{
	//	// id  (actor map id)
	//	std::string actorMapId = "";

	//	// instance mesh id
	//	int onlyId = 0;

	//	// 类型
	//	ESceneActorType type = ActorType_None;

	//	// 变换
	//	FTransform transform;

	//	// 路径
	//	FString path = TEXT("");

	//	// 开碰撞与查询
	//	bool openCollision = false;
	//};

	/*------------------------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	//// 最大tick数
	//int maxCount = 50;

	///*------------------------------------------------------------------*/

	//// 实例字典 key:actorMapId 或者 id+index
	//std::map<std::string, StaticMeshViewInfo> instanceMap;

	//// 组件字典 key是组件名称
	//UPROPERTY()
	//TMap<FString, FSceneComponentInfo> componentMap;

	//// 当前创建实例数量
	//int createCount = 0;

	//// 当前销毁实例数量
	//int destroyCount = 0;

	UPROPERTY()
		TMap<int, APawnLineActor*> lineActorMap;

	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	AActorTrajectoryView();

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

	void OnPlayerCreate(int playerId);
	void OnPlayerDestroy(int playerId);
	void OnUpdatePoint(int playerId, std::vector<FVector>& pointList);
	void OnUpdatePoint(int playerId, FVector point);
	virtual void PostInitializeComponents() override;
	//void Tick_CreateInstance(int& count, FSceneComponentInfo& sceneComponentInfo);
	//void Tick_DestroyInstance(int& count, FSceneComponentInfo& sceneComponentInfo);
	//bool IsComponentLoadRes(USceneStaticMeshComponent* meshComponent);
	//FString GetNameWithMapId(std::string actorMapId);
	//USceneStaticMeshComponent* GetMeshComponent(std::string actorMapId);
	//std::string IntToString(int64 var);
	//std::string GetKye(ESceneActorType type, int64 id);
	//FString GetNameWithPath(FString path);
	//FString GetMeshObejctPath(UStaticMesh* mesh);


	//void LoadRes(bool start, FString path);


	//void CreateInstance(std::string actorMapId, ESceneActorType type, FString path, FTransform transform,
	//                    bool openCollision);


	//void DestroyInstance(std::string actorMapId);

	//void UpdateInstance(std::string actorMapId, FTransform transform);
	//void CreateMeshComponent(FString name, FString path, bool openCollision);
	//void RemoveMeshComponent(FString name);
};
