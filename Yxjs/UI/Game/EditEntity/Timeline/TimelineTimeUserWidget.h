// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "TimelineInfo.h"
#include "TimelineTimeUserWidget.generated.h"


class UTextBlock;

/**
 * 时间轴设计,y轴0之上不会有数据了,左右无限扩充
 * -x   0    +x
 *
 *
 *
 *      +y
 */



UCLASS()
class YXJS_API UTimelineTimeUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UTextBlock> textBlockUp;

	//
	//TWeakObjectPtr<UTextBlock> textBlockDown;

	//

	// 是否初始化
	bool isInit = false;


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

	void SetStr(FString str);
	void InitData(struct FTimelineTimeInfo* info);
};
