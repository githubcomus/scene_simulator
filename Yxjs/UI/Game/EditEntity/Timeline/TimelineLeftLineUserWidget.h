// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "TimelineInfo.h"
#include "TimelineLeftLineUserWidget.generated.h"


class UTimelinePanelUserWidget;

class UTextBlock;
class UImage;


UCLASS()
class YXJS_API UTimelineLeftLineUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UTextBlock> textBlock;

	//
	// TWeakObjectPtr < UImage> image;

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UTimelinePanelUserWidget> panel;

	// 
	int32 entityId = 0;

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
};
