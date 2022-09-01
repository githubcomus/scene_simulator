// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "SelectedViewActor.generated.h"

class USelectedDataComponent;
class USelectedControllerComponent;
class UPlayerRangedWeaponComponent;

class UBoxComponent;
class UMaterial;
class USceneComponent;
class UArrowComponent;
class UStaticMeshComponent;
class ACharacter;
class UStaticMeshComponent;
class UMaterialInstanceConstant;

UCLASS()
class   ASelectedViewActor : public AActor
{
	GENERATED_BODY()

public:

	// 根场景
	UPROPERTY(VisibleAnywhere)
		USceneComponent* sceneRoot;

	// 边缘盒体
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* edgeBox;

	// 中间网格
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myPlaneMove;

	// 平移x
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myLocationX;

	// 平移y
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myLocationY;

	// 平移z
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myLocationZ;

	// 旋转x
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myRotatorPitch;

	// 旋转y
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myRotatorYaw;

	// 旋转z
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myRotatorRoll;

	// 缩放x
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myScaleX;

	// 缩放y
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myScaleY;

	// 缩放z
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* myScaleZ;

	// 旋转x材质实例
	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceConstant* rotatorPitchMaterial;

	// 旋转y材质实例
	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceConstant* rotatorYawMaterial;

	// 旋转z材质实例
	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceConstant* rotatorRollMaterial;

	// 旋转选中材质实例
	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceConstant* rotatorSelectMaterial;

	// 数据
	UPROPERTY(VisibleAnywhere)
		USelectedDataComponent* data;

	// 控制
	UPROPERTY(VisibleAnywhere)
		USelectedControllerComponent* controller;

	// 辅助线0
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* my2TempMesh0;

	// 辅助线1
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* my2TempMesh1;

	// 辅助线2
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* my2TempMesh2;

	// 辅助线3
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* my2TempMesh3;

	// 辅助线4
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* my2TempMesh4;

	// 辅助线5
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* my2TempMesh5;


	// 辅助显示坐标轴
	TArray<TWeakObjectPtr<UMeshComponent>> showXyzComponentList;

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	// Sets default values for this actor's properties
	ASelectedViewActor();
	void PlaneMesh();

	UStaticMeshComponent* CreateTempMesh(int index);
	// 构造 创建坐标轴
	void CreateAxis();

	// 构造 创建旋转网格
	void CreateSpinMesh();

	// 构造 创建缩放网格
	void CreateZoomMesh();

	// 构造 加载材质
	void LoadMaterial();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// EndPlay
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// PostInitializeComponents
	virtual void PostInitializeComponents() override;

	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/

	// START
	void Start();

	// END
	void End();

	// 将辅助面xy调整到人对其的面
	void RollFaceXY(ACharacter* character);

	//
	//UFUNCTION(BlueprintCallable)
	//	static void TestFunc(AActor* obj);

	//
	void RestoreAxisArrowColor();

	//
	void ChangeAxisArrowColor();

	// 设置工作模式
	void SetWorkMode();

	// 设置 只显示当前选中策略
	void SetShowSelectedMoveStrategySingle(ESelectedMoveStrategy selectedMoveStrategy_);

	// 设置 还原当前工作模式显示
	void SetShowSelectedMoveStrategyAll();

	void ShowHideAuxiliaryLine(bool isShow);
	/*------------------------------------------------------------------*/
	/*------------------------------------------------------------------*/
};
