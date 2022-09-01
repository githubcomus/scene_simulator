// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
//#include "TimelineInfo.h"
#include "CommandButtonMainUserWidget.generated.h"


class UCommandButtonPanelUserWidget;

class UTextBlock;
class UImage;


UCLASS()
class YXJS_API UCommandButtonMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UCommandButtonPanelUserWidget> panelUserWidget;

	// 是否初始化
	bool isInit = false;

	/*------------------------------------------------------------------*/

	void Init();
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow, bool useAnimation);
	virtual void NativeOnInitialized();
	////virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

};
