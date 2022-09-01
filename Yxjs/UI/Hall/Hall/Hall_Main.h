// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Hall_Main.generated.h"


//class UShortcutBarPanelUserWidget;

class UTextBlock;
class UImage;
class UEditableText;
class UButton;
class UTextBlock;


UCLASS()
class YXJS_API UHall_Main : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UEditableText> EditableText_账号;
	//
	TWeakObjectPtr<UEditableText> EditableText_密码;
	//
	TWeakObjectPtr<UEditableText> EditableText_IP;
	//
	TWeakObjectPtr<UEditableText> EditableText_端口;
	//
	TWeakObjectPtr<UButton> Button_登陆服务端;
	//
	TWeakObjectPtr<UTextBlock> TextBlock_button_str;

	// 是否初始化
	bool isInit = false;

	// 登录状态
	bool isLogin = false;

	/*------------------------------------------------------------------*/

	void Init();
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow, bool useAnimation);
	UFUNCTION(BlueprintCallable)
		void MyButtonClicked();
	void Start();
	void ResponseLoginFail();
	void SaveUIData();
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
