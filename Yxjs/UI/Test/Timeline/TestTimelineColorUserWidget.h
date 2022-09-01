// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Yxjs/Protobuf/hall.pb.h"
#include "Yxjs/Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
//#include "TimelineInfo.h"
#include "TestTimelineColorUserWidget.generated.h"

class UTimelinePanelUserWidget;

class UTextBlock;
class UImage;

UCLASS()
class YXJS_API UTestTimelineColorUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	//TWeakObjectPtr < UImage> image;

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

	bool GetParentOffset(UWidget* ui, FVector2D& postion);
	FVector2D GetUIOffset(UWidget* ui);
	FVector2D GetWidgetCenterLocation(UWidget* Widget);
	void OnMouseMove(bool isPanel, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
};
