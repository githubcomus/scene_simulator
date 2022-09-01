// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <list>
#include <map>
#include <vector>
#include "PawnLineActor.generated.h"


class UBoxComponent;
class UMaterial;
class UBoxComponent;
class USceneComponent;
class USceneStaticMeshComponent;
class USplineComponent;

UCLASS()
class  APawnLineActor : public AActor
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 当前轴的状态
	enum ELineType
	{
		// 无事发生(人此时静止)
		ELineType_None,
		// 路径更新中(人在移动中,匀速)
		ELineType_Update,
		// 路径停止更新(人在移动中,最新的更新超时,速度递减到0)
		ELineType_Stop
	};

	// 路径点信息
	struct SPointInfo
	{

		// 点
		FVector point;

		// key
		int index = 0;

		// 开始追逐时间
		float startTime = 0;
	};

	/*------------------------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	// 移动路径
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USplineComponent* splineComponent;

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<APawn> character;

	// 上一次路径更新
	float lastTimeUpdate = 0;

	// 样条线状态
	ELineType lineType = ELineType_None;

	// 消息所在的索引
	int msgPointIndex = 1000;

	// 玩家所在的索引
	int playerPointIndex = 1000;

	// 玩家所在的追逐点
	SPointInfo* playerPointInfo = nullptr;

	// 路径列表
	std::vector<SPointInfo> moveList;

	// 连续消息间隔
	float msgTime = 0.5;

	// 距离最终点的时间
	float differEndPointTime = 0;

	// 距离最终点的距离
	float differEndPointDistance = 0;



	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	APawnLineActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//
	void Start(int playerId);

	//
	void End();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnUpdatePoint(std::vector<FVector>& pointList);
	void OnUpdatePoint(FVector point);
	void CreateSplinePoint(FVector location);
	virtual void PostInitializeComponents() override;
};
