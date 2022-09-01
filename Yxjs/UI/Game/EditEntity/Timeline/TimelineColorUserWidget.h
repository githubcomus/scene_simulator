// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Yxjs/Protobuf/hall.pb.h"
#include "Yxjs/Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "TimelineInfo.h"
#include "TimelineColorUserWidget.generated.h"

class UTimelinePanelUserWidget;

class UTextBlock;
class UImage;
class UBorder;

UCLASS()
class YXJS_API UTimelineColorUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr < UImage> Sign;

	//
	TWeakObjectPtr < UBorder> Border_91;


	// 是否初始化
	bool isInit = false;

	//
	//TWeakObjectPtr<UTimelinePanelUserWidget> panel;


	/*------------------------------------------------------------------*/

	void Init();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	void OnMouseMove(bool isPanel, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	UFUNCTION(BlueprintCallable)
		void OnEnterColor(FVector color);
	void Test(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
};
