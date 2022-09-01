// Fill out your copyright notice in the Description page of Project Settings.

#include "InputHelp_Panel.h"

#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Tools/CalculationObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerCommandComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerInputComponent.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include <Components/EditableText.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/Button.h>
#include <Components/CheckBox.h>
#include <Components/Image.h>
#include "InputHelp_Item.h"
#include "../../../BaseHUDInfo.h"

/*
*/


/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/
//
void UInputHelp_Panel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UInputHelp_Panel::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UInputHelp_Panel::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UInputHelp_Panel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UInputHelp_Panel::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UInputHelp_Panel::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UInputHelp_Panel::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

// UI创建的初始化,各种组件变量
void UInputHelp_Panel::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	FString bpPath = TEXT(
		"WidgetBlueprint'/Game/Yxjs/UI/Permanent/InputHelp/BP_UI_InputHelp_item.BP_UI_InputHelp_item_C'");
	class_BP_UI_InputHelp_item = LoadClass<UInputHelp_Item>(this, *bpPath);
	if (class_BP_UI_InputHelp_item == nullptr)
	{
		return;
	}
	ScrollBox_line = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_line")));



	{
		HelpInputInfo info;
		info.childUI = EUIChildName::ui_name_mainui;
		info.operateType = PlayerOperateType::PlayerOperateType_None;
		info.strList.Add(TEXT("WASDQE:前后左右上下"));
		info.strList.Add(TEXT("Tab:编辑实例"));
		info.strList.Add(TEXT("I:设置界面"));
		info.strList.Add(TEXT("O:场景选择"));
		info.strList.Add(TEXT("P:角色选择"));
		info.strList.Add(TEXT("0-9:快捷键"));
		info.strList.Add(TEXT("Space:跳跃"));
		FString key = FString::Printf(TEXT("%d_%d"), info.childUI, info.operateType);
		helpInputMap.Add(key, info);
	}

	{
		HelpInputInfo info;
		info.childUI = EUIChildName::ui_name_sky_level;
		info.operateType = PlayerOperateType::PlayerOperateType_None;
		info.strList.Add(TEXT("WASDQE:前后左右上下"));
		info.strList.Add(TEXT("I:设置界面"));
		info.strList.Add(TEXT("O:场景选择"));
		info.strList.Add(TEXT("P:角色选择"));
		info.strList.Add(TEXT("ESC:设置界面"));
		FString key = FString::Printf(TEXT("%d_%d"), info.childUI, info.operateType);
		helpInputMap.Add(key, info);
	}

	{
		HelpInputInfo info;
		info.childUI = EUIChildName::ui_name_edit_entity;
		info.operateType = PlayerOperateType::PlayerOperateType_None;
		info.strList.Add(TEXT("WASDQE:前后左右上下"));
		info.strList.Add(TEXT("Tab:玩家游览"));
		info.strList.Add(TEXT("鼠标左键:选中实例"));
		info.strList.Add(TEXT("鼠标中间:撤销选中"));
		info.strList.Add(TEXT("鼠标右键:旋转方向"));
		info.strList.Add(TEXT("鼠标滚轮:切换模式"));
		info.strList.Add(TEXT("CTRL+鼠标左键:多选"));
		info.strList.Add(TEXT("CTRL+V:复制"));
		info.strList.Add(TEXT("←:删除实例"));
		info.strList.Add(TEXT("I:背包"));
		info.strList.Add(TEXT("O:材质"));
		info.strList.Add(TEXT("P:收纳"));
		FString key = FString::Printf(TEXT("%d_%d"), info.childUI, info.operateType);
		helpInputMap.Add(key, info);
	}

	// 背包默认
	{
		HelpInputInfo info;
		info.childUI = EUIChildName::ui_name_entity_backpack;
		info.operateType = PlayerOperateType::PlayerOperateType_None;
		info.strList.Add(TEXT("鼠标左键按下:选中实例"));
		info.strList.Add(TEXT("鼠标左键松开:放置实例"));
		info.strList.Add(TEXT("鼠标中间:撤销选中"));
		FString key = FString::Printf(TEXT("%d_%d"), info.childUI, info.operateType);
		helpInputMap.Add(key, info);
	}
	// 背包拖拽
	{
		HelpInputInfo info;
		info.childUI = EUIChildName::ui_name_entity_backpack;
		info.operateType = PlayerOperateType::PlayerOperateType_entity_backpack_drop;
		info.strList.Add(TEXT("鼠标移动:拖拽预览"));
		FString key = FString::Printf(TEXT("%d_%d"), info.childUI, info.operateType);
		helpInputMap.Add(key, info);
	}


	// 材质默认
	{
		HelpInputInfo info;
		info.childUI = EUIChildName::ui_name_entity_material;
		info.operateType = PlayerOperateType::PlayerOperateType_None;
		info.strList.Add(TEXT("鼠标左键:选中实例"));
		info.strList.Add(TEXT("鼠标左键:点击材质"));
		info.strList.Add(TEXT("鼠标中间:撤销选中"));
		FString key = FString::Printf(TEXT("%d_%d"), info.childUI, info.operateType);
		helpInputMap.Add(key, info);
	}


	// 时间轴不可编辑
	{
		HelpInputInfo info;
		info.childUI = EUIChildName::ui_name_timeline;
		info.operateType = PlayerOperateType::PlayerOperateType_timeline_cant_edit;
		info.strList.Add(TEXT("鼠标左键:拖动时间轴"));
		info.strList.Add(TEXT("鼠标左键双击:插入帧"));
		info.strList.Add(TEXT("鼠标中间:关闭时间轴"));
		info.strList.Add(TEXT("按钮:插入帧"));
		FString key = FString::Printf(TEXT("%d_%d"), info.childUI, info.operateType);
		helpInputMap.Add(key, info);
	}
	// 时间轴可编辑
	{
		HelpInputInfo info;
		info.childUI = EUIChildName::ui_name_timeline;
		info.operateType = PlayerOperateType::PlayerOperateType_timeline_edit;
		info.strList.Add(TEXT("鼠标左键:拖动坐标轴"));
		info.strList.Add(TEXT("鼠标左键:拖动旋转轴"));
		info.strList.Add(TEXT("鼠标左键:拖动关键帧"));
		info.strList.Add(TEXT("鼠标滚轮:切换平移旋转"));
		info.strList.Add(TEXT("颜色区域:修改颜色"));
		info.strList.Add(TEXT("按钮:删除帧"));
		FString key = FString::Printf(TEXT("%d_%d"), info.childUI, info.operateType);
		helpInputMap.Add(key, info);
	}
}

// UI回调 展开收起事件
void UInputHelp_Panel::OnExpandCollapse(bool isShow)
{

	FString str = isShow ? TEXT("展开") : TEXT("折叠");
	UE_LOG(LogTemp, Log, TEXT("[%x] [输入提示] [OnExpandCollapse] [%s]"), this, *str);

	if (isShow)
	{
		OpenUI();
	}
	else
	{
		CloseUI();
	}

}

// UI回调 展开
void UInputHelp_Panel::OpenUI()
{

	if (UFunction* func = FindFunction("Event_OpenUI"))
	{
		struct TempParam
		{
		};
		TempParam tempParam;

		ProcessEvent(func, &tempParam);
	}
}

// UI回调 收起
void UInputHelp_Panel::CloseUI()
{

}

// 切换提示
void UInputHelp_Panel::ChangeInputHelp(EUIChildName childUI, PlayerOperateType operateType)
{

	if (ScrollBox_line.IsValid() == false)return;

	if (class_BP_UI_InputHelp_item == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[%x] [输入提示] [ChangeInputHelp] 切换提示 [childUI:%d] [operateType:%d]"), this, childUI, operateType);

	ScrollBox_line->ClearChildren();

	FString key = FString::Printf(TEXT("%d_%d"), childUI, operateType);
	auto it = helpInputMap.Find(key);
	if (it == nullptr)
	{
		return;
	}

	auto addRowFunc = [&](FString str) {

		if (ScrollBox_line.IsValid() == false)return;

		UInputHelp_Item* pItem = Cast<UInputHelp_Item>(
			CreateWidget(GetWorld(), class_BP_UI_InputHelp_item));
		if (pItem == nullptr)
		{
			return;
		}
		ScrollBox_line->AddChild(pItem);
		pItem->Start(this, str);
		UE_LOG(LogTemp, Log, TEXT("[%x] [输入提示] [ChangeInputHelp] [str:%s]"), this, *str);
	};

	for (auto node : it->strList)
	{
		addRowFunc(*node);
	}
}

//