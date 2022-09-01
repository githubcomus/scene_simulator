// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "PlaceViewLaserLightActor.generated.h"

struct FLocalEntityInfo;
class AEditEntityManage;

class UBoxComponent;
class UMaterial;
//class UHierarchicalInstancedStaticMeshComponent;
class USceneComponent;
class UArrowComponent;
class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;
class UMaterialInstanceConstant;
class UStaticMeshComponent;
class USpotLightComponent;

UCLASS()
class APlaceViewLaserLightActor : public AActor
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 加载图片类型
	enum ELoadResourcesType
	{
		LoadRes_Image,
		LoadRes_Mesh,
		LoadRes_Mesh2,
		LoadRes_Mesh3,
		LoadRes_Mesh4_light,
		LoadRes_Material,
		LoadRes_Material2,
	};

	// 资源信息
	struct CvResourcesInfo
	{
		//
		TSharedPtr<struct FStreamableHandle> handle;

		// 路径
		FString path;
	};

	/*------------------------------------------------------------------*/

	// 根场景
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneRoot;

	// 基座网格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* bottomLayerMeshComponent;

	// 灯泡网格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* lightBulbMeshComponent;

	// 聚光灯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpotLightComponent* spotLight;

	// 激光(粒子系统)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* particleSystemComponent;

	// 动态灯管静态网格+发光材质
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* lightMeshComponent;

	// 资源信息
	TMap<UObject*, struct CvResourcesInfo> resMap;

	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	APlaceViewLaserLightActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// EndPlay
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TickMove(int time);
	void RealRelativeRotator(FRotator& rotator);
	void LoadRes(bool satrt, FString path, ELoadResourcesType type, UObject* obj);
	void LaserLightUpdate(Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);
	void SetPreviewRotator(FRotator rotator);

	// PostInitializeComponents
	virtual void PostInitializeComponents() override;

	/*------------------------------------------------------------------*/

	// START
	void Start(AEditEntityManage* view, Gamedata::EntityInfo* syncInfo, FLocalEntityInfo* localInfo);

	// END
	void End();

	/*------------------------------------------------------------------*/
};
