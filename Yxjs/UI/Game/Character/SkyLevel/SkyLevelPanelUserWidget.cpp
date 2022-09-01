// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyLevelPanelUserWidget.h"

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
#include "Core/BaseGameInstance.h"
#include "SkyLevelItemUserWidget.h"

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
#include "../../../../GameMode/Component/BaseGameModeLevelManage.h"

//

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 外部数据 获取 动态获取view
TWeakObjectPtr<AEditEntityManage> USkyLevelPanelUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> USkyLevelPanelUserWidget::GetEntityAcotr(int entityId) {
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// 获取当前选中的actor
TWeakObjectPtr<AEditEntityActor> USkyLevelPanelUserWidget::GetSelectEntityActor() {

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
Gamedata::PlayerInfo* USkyLevelPanelUserWidget::GetPlayerSyncInfo() {
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return nullptr;

	auto syncInfo = character->myData->GetSyncInfo();
	return syncInfo;

}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/
//
void USkyLevelPanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void USkyLevelPanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void USkyLevelPanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply USkyLevelPanelUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply USkyLevelPanelUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void USkyLevelPanelUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void USkyLevelPanelUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply USkyLevelPanelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

// 初始化 
void USkyLevelPanelUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	FString bpPath = TEXT(
		"WidgetBlueprint'/Game/Yxjs/UI/Character/SkyLevel/BP_UI_SkyLevel_item.BP_UI_SkyLevel_Item_C'");
	class_BP_UI_SkyLevel_Item_C = LoadClass<USkyLevelItemUserWidget>(this, *bpPath);
	if (class_BP_UI_SkyLevel_Item_C == nullptr)
	{
		return;
	}

	ScrollBox_line = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_line")));
}

// 事件 展开收起
void USkyLevelPanelUserWidget::OnExpandCollapse(bool isShow) {

	FString str = isShow ? TEXT("展开") : TEXT("折叠");
	UE_LOG(LogTemp, Log, TEXT("[%x] [场景切换] [OnExpandCollapse] [%s]"), this, *str);

	if (isShow) {
		OpenUI();
	}
	else {
		CloseUI();
	}

}

// 展开
void USkyLevelPanelUserWidget::OpenUI() {

	changeCount = 0;

	if (UFunction* func = FindFunction("Event_OpenUI"))
	{
		struct TempParam
		{
		};
		TempParam tempParam;

		ProcessEvent(func, &tempParam);
	}
	Init();
	InitData();
}

// 展开 初始化数据
void USkyLevelPanelUserWidget::InitData() {


	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto levelManage = baseGameMode->levelManage;

	if (isInitData)return;
	isInitData = true;

	auto addRowFunc = [&](int itemIndex, FMySkyMapInfo* info) {
		if (class_BP_UI_SkyLevel_Item_C == nullptr)return;
		USkyLevelItemUserWidget* child = Cast<USkyLevelItemUserWidget>(
			CreateWidget(GetWorld(), class_BP_UI_SkyLevel_Item_C));
		if (child == nullptr)return;
		child->owner = this;
		child->itemIndex = itemIndex;
		child->name = info->showName;
		child->iconPath = info->iconPath;
		ScrollBox_line->AddChild(child);
		itemList.Add(child);
		child->Start();
		UE_LOG(LogTemp, Log, TEXT("[%x] [场景切换] [InitData] [itemIndex:%d]"), this, itemIndex);
	};

	// 清空之前的数据
	itemList.Reset();
	auto childList = ScrollBox_line->GetAllChildren();
	for (auto node : childList) {
		ScrollBox_line->RemoveChild(node);
	}

	int index = 0;
	for (auto node : levelManage->levelInfoList) {
		addRowFunc(index, &node);
		index++;
	}

	OpenUISetSelect();
}

// 展开 打开UI设置选中节点
void USkyLevelPanelUserWidget::OpenUISetSelect() {

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	int sky_level_index = character->myData->GetSyncInfo()->sky_level_index();

	SetSelect(sky_level_index, false);
}

// 收起
void USkyLevelPanelUserWidget::CloseUI() {

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

	changeCount = 0;
}

// 玩家点击选中
void USkyLevelPanelUserWidget::SetSelect(int newSelectIndex, bool doRealJob) {

	// doRealJob:是否执行UI后面的真实任务 true:执行  false:只修改UI

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (oldSelectIndex == newSelectIndex && oldSelectIndex != 0)return;

	//1. 设置老选项
	if (oldSelectIndex < itemList.Num()) {
		itemList[oldSelectIndex]->SetSelect(false, true);
	}

	//2. 设置新选项
	if (newSelectIndex < itemList.Num()) {
		itemList[newSelectIndex]->SetSelect(false, false);
		oldSelectIndex = newSelectIndex;

		//3. 加载关卡
		if (doRealJob) {
			baseGameMode->levelManage->ChangeNewSkyLevel(newSelectIndex, false);
		}
	}
}