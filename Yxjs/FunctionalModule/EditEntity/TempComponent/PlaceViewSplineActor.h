// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceViewSplineActor.generated.h"


class UPlayerRangedWeaponComponent;
class ABuildManageActor;

class UBoxComponent;
class UMaterial;
class UPlaceStaticMeshComponent;
class USceneComponent;
class UArrowComponent;
class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;
class UMaterialInstanceConstant;
class UStaticMeshComponent;

UCLASS()
class   APlaceViewSplineActor : public AActor
{
    GENERATED_BODY()

public:

    /*------------------------------------------------------------------*/

    // 网格资源
    struct CvResourcesInfo
    {
        // 网格组件,key是组件指针,value是回调加载handle对象
        TMap<class USceneComponent*, TSharedPtr<struct FStreamableHandle>> resHandleMap;

        // 网格组件,key是组件指针,value是模型对象
        TMap<class USceneComponent*, struct FSoftObjectPath> resPathMap;
    };

    // 根场景
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* sceneRoot;

    // 样条线
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USplineComponent* splineComponent;

    //
    TArray<USplineMeshComponent*>splineMeshList;

    // 最大初始样条线节点数量
    int maxPointCount = 50;

    // 资源
    CvResourcesInfo ri;

    /*------------------------------------------------------------------*/

    // Sets default values for this actor's properties
    APlaceViewSplineActor();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // EndPlay
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // PostInitializeComponents
    virtual void PostInitializeComponents() override;

    /*------------------------------------------------------------------*/

    // START
    void Start(UPlayerRangedWeaponComponent* playerRangedWeaponComponent_);

    // END
    void End();

    bool LoadRes(FString meshPath, USceneComponent* component);
    void CallBackLoadRes(int32 var, USceneComponent* component);
    void UnloadRes(USceneComponent* component);

    // 创建网格组件
    void CreateSplineMeshComponent(FVector location, FString meshPath);

    // 关闭所有网格组件
    void CloseSplineMeshComponent(USplineMeshComponent* meshComponent);

    // 抛物线 更新网格位置
    void UpdateSplineMesh(int meshIndex);

    // 抛物线 重置样条线节点与网格
    void ResetLinePoint(TArray<FVector>& pointList);

    // 测试 增加样条线节点
    void TestAddPoint();

    /*------------------------------------------------------------------*/
};
