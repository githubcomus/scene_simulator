// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../HUDStaticFunc.h"
#include "../../../BaseHUDInfo.h"
#include "InputHelp_Panel.generated.h"


class AEditEntityManage;
class AEditEntityActor;
class UInputHelp_Item;

class UTextBlock;
class UImage;
class UScrollBox;
class UButton;
class UCheckBox;


// 玩家操作帮助数据
struct HelpInputInfo
{
	// 子UI
	EUIChildName childUI = EUIChildName::ui_none;

	// 操作节点
	PlayerOperateType operateType = PlayerOperateType_None;

	// 显示内容
	TArray<FString> strList;
};

UCLASS()
class YXJS_API UInputHelp_Panel : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UScrollBox> ScrollBox_line;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_InputHelp_item;

	//
	TArray<TWeakObjectPtr<UInputHelp_Item>>itemList;

	//
	bool isInitData = false;

	// 输入帮助信息 key:子UI_操作状态 value:info
	TMap<FString, HelpInputInfo> helpInputMap;

	/*------------------------------------------------------------------*/

	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow);
	void Init();
	void OpenUI();
	void CloseUI();
	void ChangeInputHelp(EUIChildName childUI, PlayerOperateType operateType);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);


};