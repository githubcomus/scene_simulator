// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
//#include "TimelineInfo.h"
#include "ShortcutBarItemUserWidget.generated.h"


class UShortcutBarPanelUserWidget;

class UTextBlock;
class UImage;
class UButton;


UCLASS()
class YXJS_API UShortcutBarItemUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	struct ShortcutButtonInfo {

		//
		TWeakObjectPtr<UTextBlock> TextBlock;

		//
		TWeakObjectPtr<UButton> Button;

		//

	};

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	// 按钮1
	ShortcutButtonInfo button1;

	// 按钮2
	ShortcutButtonInfo button2;

	// 按钮3
	ShortcutButtonInfo button3;

	//
	TWeakObjectPtr<UShortcutBarPanelUserWidget> owner;

	// 所在索引
	int keyIndex = 0;

	// 当前显示的索引(1号按钮)
	int buttonIndex = 0;


	/*------------------------------------------------------------------*/

	void Init();
	UFUNCTION(BlueprintCallable)
		void UICallback_ButtonEnter(int index);
	void UICall_Event_ShowBorder(bool enter);
	void UICall_Event_SelectButton(int index);
	int GetButtonIndex();
	int GetNextButtonIndex();
	void OnExpandCollapse(bool isShow, UUserWidget* owner_, int keyIndex_);
	void SetText(bool show1, bool show2, bool show3, const std::string str1, const std::string str2, const std::string str3);
	void SetText(int keyIndex_);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();

};
