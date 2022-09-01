// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "SelectedControllerComponent.generated.h"


class USelectedDataComponent;
class ASelectedViewActor;
class UArrowComponent;
class ABaseCharacter;

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  USelectedControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/


	// tick数据
	struct CvTickInfo
	{
		// 函数最后执行时间
		float lastTime;

	};


	// 管理类
	USelectedDataComponent* data;

	// 视图类
	ASelectedViewActor* view;

	// tick数据
	CvTickInfo cvTickInfo;

	//
	float delayTime = 1.0 / 30;

	// 按下松开的临时数据.
	ESelectedMoveStrategy tempSelectedMoveStrategy = ESelectedMoveStrategy::None;

	/*------------------------------------------------------------------*/

	//
	USelectedControllerComponent();

	//Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*------------------------------------------------------------------*/

	//
	void Start(USelectedDataComponent* data_, ASelectedViewActor* view_);

	//
	void End();
	void FrameSetScale();
	ABaseCharacter* GetLocalCharacter();

	ESelectedWorkMode GetWorkMode();
	// 设置 还原所有坐标轴箭头颜色
	void RestoreAxisArrowColor(ESelectedMoveStrategy type);

	// 设置 改变坐标轴箭头颜色
	void ChangeAxisArrowColor(ESelectedMoveStrategy type);

	// 设置 工作模式
	void SetWorkMode(ESelectedWorkMode selectedWorkMode_);

	// 设置 只显示当前选中策略
	void SetShowSelectedMoveStrategySingle(ESelectedMoveStrategy selectedMoveStrategy_);

	// 设置 还原当前工作模式显示
	void SetShowSelectedMoveStrategyAll();

	// 设置 将旋转轴切换到指定方向
	void SetScaleAxis(FTransform originWTransform);

	// 设置 将旋转轴切换到指定方向
	void SetWorldLocation();

	// 设置 将旋转轴切换到指定方向
	void SetRelativelyLocation(FTransform originWTransform);
	void SetIsSelected(bool flag);
	void SetRelativelyRotation(FTransform worldTransform);
};
