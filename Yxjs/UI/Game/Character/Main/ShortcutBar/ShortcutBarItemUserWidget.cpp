// Fill out your copyright notice in the Description page of Project Settings.

#include "ShortcutBarItemUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ShortcutBarPanelUserWidget.h"
#include <Components/Button.h>

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UShortcutBarItemUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UShortcutBarItemUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UShortcutBarItemUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	//shortcutBar = Cast<UShortcutBarPanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_CommandButton_Panel")));
	button1.TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_1")));
	button2.TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_2")));
	button3.TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_3")));
	button1.Button = Cast<UButton>(GetWidgetFromName(TEXT("Button_1")));
	button2.Button = Cast<UButton>(GetWidgetFromName(TEXT("Button_2")));
	button3.Button = Cast<UButton>(GetWidgetFromName(TEXT("Button_3")));

}

//
void UShortcutBarItemUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

//// 当前按钮的数据
//google::protobuf::RepeatedPtrField< ::std::string>* UShortcutBarItemUserWidget::GetButtonInfo(int keyIndex_) {
//
//	auto syncInfo = owner->GetPlayerSyncInfo();
//	if (syncInfo == nullptr)return nullptr;
//
//	bool flag = false;
//	if (pbInfo->key_list_size() > keyIndex) {
//		auto node = pbInfo->key_list(keyIndex);
//		auto command_list = node.mutable_command_list();
//		//if (node.command_list_size() > selectIndex) {
//		//	//commondStr = node.command_list(selectIndex);
//		//	flag = true;
//		//}
//	}
//
//}

// 获取 数量
int UShortcutBarItemUserWidget::GetButtonIndex() {

	int tempVar = 0;
	auto syncInfo = owner->GetPlayerSyncInfo();
	if (syncInfo == nullptr)return 0;

	auto pbInfo = syncInfo->mutable_entity_button_info();
	if (pbInfo->key_list_size() <= keyIndex) { return 0; }

	auto node = pbInfo->key_list(keyIndex);
	auto command_list = node.mutable_command_list();

	if (node.command_list_size() == 0) {
		return 0;
	}
	else if (node.command_list_size() == 1) {
		tempVar = 1;
	}
	else if (node.command_list_size() == 2) {
		tempVar = 2;
	}
	else if (node.command_list_size() >= 3) {
		tempVar = 3;
	}
	return tempVar;
}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

// UI回调 展开收起事件
void UShortcutBarItemUserWidget::OnExpandCollapse(bool isShow, UUserWidget* owner_, int keyIndex_) {

	if (isShow) {

		owner = Cast<UShortcutBarPanelUserWidget>(owner_);
		keyIndex = keyIndex_;

		SetText(keyIndex_);

		buttonIndex = GetButtonIndex() - 1;
		//UICall_Event_SelectButton(0);
	}
}

// 根据不同的最顶层索引来设置数据
void UShortcutBarItemUserWidget::SetText(int keyIndex_) {

	auto syncInfo = owner->GetPlayerSyncInfo();
	if (syncInfo == nullptr)return;

	auto pbInfo = syncInfo->mutable_entity_button_info();
	if (pbInfo->key_list_size() <= keyIndex) {
		SetText(false, false, false, "", "", "");
		return;
	}

	auto node = pbInfo->key_list(keyIndex);
	auto command_list = node.mutable_command_list();

	if (node.command_list_size() == 0) {
		SetText(false, false, false, "", "", "");
	}
	else if (node.command_list_size() == 1) {
		auto str0 = node.command_list(0);
		SetText(true, false, false, str0, "", "");
	}
	else if (node.command_list_size() == 2) {

		auto str0 = node.command_list(0);
		auto str1 = node.command_list(1);
		SetText(true, true, false, str0, str1, "");
	}
	else if (node.command_list_size() >= 3) {
		auto str0 = node.command_list(0);
		auto str1 = node.command_list(1);
		auto str2 = node.command_list(2);
		SetText(true, true, true, str0, str1, str2);
	}

}

// 设置UI文字
void UShortcutBarItemUserWidget::SetText(bool show1, bool show2, bool show3, const std::string str1, const std::string str2, const std::string str3) {
	FString str1_ = UTF8_TO_TCHAR(str1.c_str());
	FString str2_ = UTF8_TO_TCHAR(str2.c_str());
	FString str3_ = UTF8_TO_TCHAR(str3.c_str());
	button1.TextBlock->SetText(FText::FromString(str1_));
	button2.TextBlock->SetText(FText::FromString(str2_));
	button3.TextBlock->SetText(FText::FromString(str3_));

	ESlateVisibility visibility1 = show1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	button1.Button->SetVisibility(visibility1);
	ESlateVisibility visibility2 = show2 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	button2.Button->SetVisibility(visibility2);
	ESlateVisibility visibility3 = show3 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	button3.Button->SetVisibility(visibility3);
}

// UI回调 按钮按下
void UShortcutBarItemUserWidget::UICallback_ButtonEnter(int buttonIndex_) {
	owner->UICallback_ItemButtonEnter(keyIndex, buttonIndex_);
}

// UI调用 显示边框
void UShortcutBarItemUserWidget::UICall_Event_ShowBorder(bool enter)
{
	if (UFunction* func = FindFunction("Event_ShowBorder"))
	{
		struct TempParam
		{
			bool enter;
		};
		TempParam tempParam;
		tempParam.enter = enter;
		ProcessEvent(func, &tempParam);
	}
}

// UI调用 设置选中按钮
void UShortcutBarItemUserWidget::UICall_Event_SelectButton(int index)
{
	if (UFunction* func = FindFunction("SetButtonSelect"))
	{
		struct TempParam
		{
			int index;
		};
		TempParam tempParam;
		tempParam.index = index;
		ProcessEvent(func, &tempParam);
	}
}

// 输入调用 切换当前index
int UShortcutBarItemUserWidget::GetNextButtonIndex() {


	auto syncInfo = owner->GetPlayerSyncInfo();
	if (syncInfo == nullptr)return 0;

	auto pbInfo = syncInfo->mutable_entity_button_info();
	if (pbInfo->key_list_size() <= keyIndex) { return 0; }

	auto node = pbInfo->key_list(keyIndex);
	auto command_list = node.mutable_command_list();

	if (node.command_list_size() == 0) {
		return 0;
	}
	else if (node.command_list_size() == 1) {
		buttonIndex = 0;
	}
	else if (node.command_list_size() == 2) {
		if (buttonIndex == 0) {
			buttonIndex = 1;
		}
		else {
			buttonIndex = 0;
		}
	}
	else if (node.command_list_size() >= 3) {
		if (buttonIndex == 0) {
			buttonIndex = 1;
		}
		else if (buttonIndex == 1) {
			buttonIndex = 2;
		}
		else if (buttonIndex == 2) {
			buttonIndex = 0;
		}
	}
	return buttonIndex;
}
