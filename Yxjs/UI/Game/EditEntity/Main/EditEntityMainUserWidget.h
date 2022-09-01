// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "UI/BaseHUD.h"
//#include "TimelineInfo.h"
#include "EditEntityMainUserWidget.generated.h"


class UEditEntityMainButtonUserWidget;
class UEditEntityMainBaseSyncInfoUserWidget;
class AEditEntityActor;

class UTextBlock;
class UImage;
class USlider;


UCLASS()
class YXJS_API UEditEntityMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 临时数据 选中实例数据
	struct CvEditEntityManageInfo
	{
		//std::list<int> entityIdList; // 时间轴id
		int entityId = 0;			// id
	};

	// 临时数据 时间轴数据
	struct CvTimelineInfo
	{
		int entityId = 0;			// 时间轴id
	};

	// 临时数据 命令触发数据
	struct CvCommandTriggerInfo
	{
		int entityId = 0;			// 时间轴id
	};


	// 临时数据
	struct CvTempUIInfo
	{
		EUITempDateType type;
		CvEditEntityManageInfo editEntity;
		CvTimelineInfo timelineInfo;
		CvCommandTriggerInfo commandTrigger;
	};

	// 临时数据
	CvTempUIInfo tempUIInfo;

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UEditEntityMainButtonUserWidget> BP_UI_EditEntity_Button_merge;

	//
	TWeakObjectPtr<UEditEntityMainButtonUserWidget> BP_UI_EditEntity_Button_break_down;

	//
	TWeakObjectPtr<UEditEntityMainBaseSyncInfoUserWidget> BP_UI_EditEntity_BaseSyncInfo;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_ctrl;

	//
	TWeakObjectPtr<USlider> Slider_speed;

	/*------------------------------------------------------------------*/

	void Init();
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow, bool useAnimation);
	UFUNCTION(BlueprintCallable)
		void SaveTempDate(EUITempDateType type);
	UFUNCTION(BlueprintCallable)
		void UseTempDate();

	void SetButtonStopEnter(std::vector<int>& selectList);
	void EnterCtrl(bool enter);
	UFUNCTION(BlueprintCallable)
		void MyOnValueChanged(float value);
	TWeakObjectPtr<AEditEntityActor> GetEntityAcotr(int entityId);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

};
