// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "NewTimelineInfo.h"
#include "NewTimelineMainUserWidget.generated.h"

class UTimelinePanelUserWidget;

class UTextBlock;
class UImage;

UCLASS()
class YXJS_API UNewTimelineMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UTimelinePanelUserWidget> timelinePanel;

	// 是否初始化
	bool isInit = false;

	/*------------------------------------------------------------------*/

	// 测试数据

	// 网络(本地模拟操作)同步数据
	Gamedata::EntityInfo syncInfo;

	/*------------------------------------------------------------------*/

	void Init();
	virtual void NativeOnInitialized();
	//virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	// FTimelineEnterPointInfo* GetTimelinePointInfo();

	void SetStr(FString str);
	void SetSelect(bool isSelect);
	void OnMouseEnterLeave(bool enter);
	void LoadTestData();
	void AddOneData();
};
