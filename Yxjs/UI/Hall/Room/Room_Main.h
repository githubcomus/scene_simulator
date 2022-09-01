// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Room_Main.generated.h"


//class UShortcutBarPanelUserWidget;

class UTextBlock;
class UImage;


UCLASS()
class YXJS_API URoom_Main : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	//TWeakObjectPtr<UShortcutBarPanelUserWidget> shortcutBar;

	// 是否初始化
	bool isInit = false;

	/*------------------------------------------------------------------*/

	void Init();
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow, bool useAnimation);
	void Start();
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
