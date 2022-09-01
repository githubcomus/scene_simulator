// Fill out your copyright notice in the Description page of Project Settings.

#include "Character_Panel.h"

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
#include "Character_Item.h"

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
#include "Yxjs/Core/Component/ZwXlsDataActorComponent.h"
#include "Components/UniformGridPanel.h"
#include "Yxjs/Pawn/Character/CharacterComponent/CharacterControllerComponent.h"

//

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 外部数据 获取 动态获取view
TWeakObjectPtr<AEditEntityManage> UCharacter_Panel::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> UCharacter_Panel::GetEntityAcotr(int entityId) {
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// 获取当前选中的actor
TWeakObjectPtr<AEditEntityActor> UCharacter_Panel::GetSelectEntityActor() {

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
Gamedata::PlayerInfo* UCharacter_Panel::GetPlayerSyncInfo() {
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
void UCharacter_Panel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UCharacter_Panel::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UCharacter_Panel::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UCharacter_Panel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UCharacter_Panel::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UCharacter_Panel::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UCharacter_Panel::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UCharacter_Panel::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

// 初始化 
void UCharacter_Panel::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	FString bpPath = TEXT(
		"WidgetBlueprint'/Game/Yxjs/UI/Character/Character/BP_UI_Character_Item.BP_UI_Character_Item_C'");
	class_BP_UI_Character_Item_C = LoadClass<UCharacter_Item>(this, *bpPath);
	if (class_BP_UI_Character_Item_C == nullptr)
	{
		return;
	}

	ScrollBox_line = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_line")));
	Uniform_Grid_Panel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("Uniform_Grid_Panel")));
}

// 事件 展开收起
void UCharacter_Panel::OnExpandCollapse(bool isShow) {

	FString str = isShow ? TEXT("展开") : TEXT("折叠");
	UE_LOG(LogTemp, Log, TEXT("[%x] [角色切换] [OnExpandCollapse] [%s]"), this, *str);

	if (isShow) {
		OpenUI();
	}
	else {
		CloseUI();
	}

}

// 展开
void UCharacter_Panel::OpenUI() {

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
void UCharacter_Panel::InitData() {

	if (isInitData)return;
	isInitData = true;

	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance == nullptr)return;
	if (gameInstance->zwXlsDataActorComponent == nullptr)return;
	xlsdata::XlsData& xlsData = gameInstance->zwXlsDataActorComponent->xlsData;

	if (Uniform_Grid_Panel.IsValid() == false)return;

	//1. 源数据
	std::vector<xlsdata::CharacterTableDefine>& xlsList = xlsData.CharacterTable_;

	//2. 新增行回调
	auto addRowFunc = [&](int thisRowIndex, int thisListIndex, int itemIndex, xlsdata::CharacterTableDefine* xlsInfo) {

		if (class_BP_UI_Character_Item_C == nullptr)return;
		UCharacter_Item* child = Cast<UCharacter_Item>(
			CreateWidget(GetWorld(), class_BP_UI_Character_Item_C));
		if (child == nullptr)return;
		child->owner = this;
		child->itemIndex = itemIndex;

		child->name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());
		child->iconPath = UTF8_TO_TCHAR(xlsInfo->IconPath_.c_str());
		child->xlsId = xlsInfo->ID_;

		Uniform_Grid_Panel->AddChildToUniformGrid(child, thisRowIndex, thisListIndex);
		itemList.Add(child);
		child->Start();

		UE_LOG(LogTemp, Log, TEXT("[%x] [角色切换] [OpenThirdType] [thisRowIndex:%d] [thisListIndex:%d] [itemIndex:%d]"), this, thisRowIndex, thisListIndex, itemIndex);
	};

	//3. 清空之前的数据
	itemList.Reset();
	Uniform_Grid_Panel->ClearChildren();

	int maxListCount = 3;	// 每行最大字段数量
	int maxItemCount = xlsList.size();	// 总共的节点数量
	int rowCount = maxItemCount / maxListCount;
	bool lastRow = (maxItemCount % maxListCount) > 0 ? true : false;
	int itemIndex = 0;

	// 前面的行
	for (int thisRowIndex = 0; thisRowIndex < rowCount; thisRowIndex++) {
		for (int thisListIndex = 0; thisListIndex < maxListCount; thisListIndex++) {
			auto& xlsInfo = xlsList[itemIndex];
			addRowFunc(thisRowIndex, thisListIndex, itemIndex, &xlsInfo);
			itemIndex++;
		}
	}

	// 最后一行
	if (lastRow) {
		for (int thisListIndex = 0; thisListIndex < (maxItemCount % maxListCount); thisListIndex++) {
			auto& xlsInfo = xlsList[itemIndex];
			addRowFunc(rowCount, thisListIndex, itemIndex, &xlsInfo);
			itemIndex++;
		}
	}

	// 选中数据
	OpenUISetSelect();


}

// 展开 打开UI设置选中节点
void UCharacter_Panel::OpenUISetSelect() {

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	int character_index = character->myData->GetSyncInfo()->character_index();

	SetSelect(character_index, false);
}

// 收起
void UCharacter_Panel::CloseUI() {

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

	changeCount = 0;
}

// 玩家点击选中
void UCharacter_Panel::SetSelect(int newSelectIndex, bool doRealJob) {

	// doRealJob:是否执行UI后面的真实任务 true:执行  false:只修改UI

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	ABaseCharacter* character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

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
			//character->myData->GetSyncInfo()->set_character_index(newSelectIndex);
			//character->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_Character);
			Gamedata::PlayerInfo newSyncInfo;
			newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_Character);
			newSyncInfo.CopyFrom(*character->myData->GetSyncInfo());
			newSyncInfo.set_character_index(newSelectIndex);
			character->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
		}
	}
}

