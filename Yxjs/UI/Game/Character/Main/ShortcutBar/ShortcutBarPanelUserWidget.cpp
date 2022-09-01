// Fill out your copyright notice in the Description page of Project Settings.

#include "ShortcutBarPanelUserWidget.h"

#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Tools/CalculationObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerCommandComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"

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
#include "ShortcutBarItemUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 外部数据 获取 动态获取view
TWeakObjectPtr<AEditEntityManage> UShortcutBarPanelUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> UShortcutBarPanelUserWidget::GetEntityAcotr(int entityId) {
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// 获取当前选中的actor
TWeakObjectPtr<AEditEntityActor> UShortcutBarPanelUserWidget::GetSelectEntityActor() {

	auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto character = Cast<ABaseCharacter>(player->GetPawn());
	if (character == nullptr)return nullptr;
	if (character->selectComponent == nullptr)return nullptr;
	auto entityList = &character->selectComponent->cvMultipleInstanceInfo.selectedList;

	auto& entityList_ = *entityList;
	if (entityList_.Num() == 0)return nullptr;
	int masterEntityId = entityList_[entityList_.Num() - 1].entityId;

	auto actor = GetEntityAcotr(masterEntityId);
	return actor;
}

// 玩家数据
Gamedata::PlayerInfo* UShortcutBarPanelUserWidget::GetPlayerSyncInfo() {
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return nullptr;

	auto syncInfo = character->myData->GetSyncInfo();
	return syncInfo;

}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UShortcutBarPanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UShortcutBarPanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

// UI创建的初始化,各种组件变量
void UShortcutBarPanelUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	auto shortcutBar0 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_0")));
	auto shortcutBar1 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_1")));
	auto shortcutBar2 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_2")));
	auto shortcutBar3 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_3")));
	auto shortcutBar4 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_4")));
	auto shortcutBar5 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_5")));
	auto shortcutBar6 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_6")));
	auto shortcutBar7 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_7")));
	auto shortcutBar8 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_8")));
	auto shortcutBar9 = Cast<UShortcutBarItemUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Item_9")));
	itemList.Add(shortcutBar0);
	itemList.Add(shortcutBar1);
	itemList.Add(shortcutBar2);
	itemList.Add(shortcutBar3);
	itemList.Add(shortcutBar4);
	itemList.Add(shortcutBar5);
	itemList.Add(shortcutBar6);
	itemList.Add(shortcutBar7);
	itemList.Add(shortcutBar8);
	itemList.Add(shortcutBar9);


	//UE_LOG(LogTemp, Log, TEXT("[%x] [快捷栏] Init"), this);
}

//
void UShortcutBarPanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

// UI回调 展开收起事件
void UShortcutBarPanelUserWidget::OnExpandCollapse(bool isShow, UUserWidget* owner_) {


	FString str = isShow ? TEXT("展开") : TEXT("折叠");
	UE_LOG(LogTemp, Log, TEXT("[%x] [快捷栏] [OnExpandCollapse] [%s]"), this, *str);

	if (isShow) {
		OpenUI();
	}
	else {
		CloseUI();
	}

	itemList[0]->OnExpandCollapse(isShow, this, 0);
	itemList[1]->OnExpandCollapse(isShow, this, 1);
	itemList[2]->OnExpandCollapse(isShow, this, 2);
	itemList[3]->OnExpandCollapse(isShow, this, 3);
	itemList[4]->OnExpandCollapse(isShow, this, 4);
	itemList[5]->OnExpandCollapse(isShow, this, 5);
	itemList[6]->OnExpandCollapse(isShow, this, 6);
	itemList[7]->OnExpandCollapse(isShow, this, 7);
	itemList[8]->OnExpandCollapse(isShow, this, 8);
	itemList[9]->OnExpandCollapse(isShow, this, 9);

}

// UI回调 展开
void UShortcutBarPanelUserWidget::OpenUI() {

	changeCount = 0;


}

// UI回调 收起
void UShortcutBarPanelUserWidget::CloseUI() {

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	if (character->commandComponent == nullptr)return;

}

// 键盘输入转发到UI事件(快捷键)
void UShortcutBarPanelUserWidget::Input_Number(int keyIndex, bool enter) {

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	if (character->commandComponent == nullptr)return;
	auto syncInfo = GetPlayerSyncInfo();
	if (syncInfo == nullptr)return;

	itemList[keyIndex]->UICall_Event_ShowBorder(enter);

	if (enter) {


		// UI中寻找当前快捷键的索引值
		int selectIndex = itemList[keyIndex]->GetNextButtonIndex();
		itemList[keyIndex]->UICall_Event_SelectButton(selectIndex);

		std::string commondStr = "";
		bool getCommandFlag = false;
		auto pbInfo = syncInfo->mutable_entity_button_info();

		if (pbInfo->key_list_size() > keyIndex) {
			auto node = pbInfo->key_list(keyIndex);
			auto command_list = node.mutable_command_list();
			if (node.command_list_size() > selectIndex) {
				commondStr = node.command_list(selectIndex);
				getCommandFlag = true;
			}
		}

		if (getCommandFlag == false)return;

		character->commandComponent->UICallback_EnterNumber(commondStr);

	}
}

// UI按钮输入
void UShortcutBarPanelUserWidget::UICallback_ItemButtonEnter(int keyIndex, int valueIndex) {

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	auto syncInfo = GetPlayerSyncInfo();
	if (syncInfo == nullptr)return;

	std::string commondStr = "";
	bool getCommandFlag = false;
	auto pbInfo = syncInfo->mutable_entity_button_info();

	if (pbInfo->key_list_size() > keyIndex) {
		auto node = pbInfo->key_list(keyIndex);
		auto command_list = node.mutable_command_list();
		if (node.command_list_size() > valueIndex) {
			commondStr = node.command_list(valueIndex);
			getCommandFlag = true;
		}
	}

	if (getCommandFlag == false)return;

	character->commandComponent->UICallback_EnterNumber(commondStr);
}
