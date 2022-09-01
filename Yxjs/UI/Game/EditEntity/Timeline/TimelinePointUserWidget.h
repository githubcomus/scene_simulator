// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "TimelineInfo.h"
#include "TimelinePointUserWidget.generated.h"


class UTimelinePanelUserWidget;

class UTextBlock;
class UImage;

/**
 * 时间轴设计,y轴0之上不会有数据了,左右无限扩充
 * -x   0    +x
 *
 *
 *
 *      +y
 */



UCLASS()
class YXJS_API UTimelinePointUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UTextBlock> TextBlock_str;

	//
	TWeakObjectPtr < UImage> image;

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UTimelinePanelUserWidget> panel;

	// 
	FTimelinePointIndex pointIndexInfo;

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
	FTimelineEnterPointInfo* GetTimelinePointInfo();

	void SetStr(FString str);
	void SetSelect(bool isSelect);
	void OnMouseEnterLeave(bool enter);
};
