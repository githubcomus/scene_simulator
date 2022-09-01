// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn/Character/CharacterComponent/PlayerCommandComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "FunctionalModule/OverallSituation/OverallSituationView.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManage.h"
#include "Yxjs/FunctionalModule/EditEntity/MyPython/MyPythonActor.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "../BaseCharacter.h"
#include "Yxjs/UI/Game/EditEntity/Timeline/TimelineMainUserWidget.h"
#include "Yxjs/UI/BaseHUD.h"
#include "UI/Game/EditEntity/Timeline/TimelineMainUserWidget.h"
#include "UI/Game/EditEntity/Timeline/TimelinePanelUserWidget.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManageController.h"
#include "CharacterDataComponent.h"
#include "CharacterControllerComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityCommandComponent.h"
#include "Yxjs/FunctionalModule/EditEntity/Command/EditEntityManageCommandComponent.h"
#include "Yxjs/UI/Game/Character/Main/CharacterMainUserWidget.h"
#include "Yxjs/UI/Game/Character/Main/ShortcutBar/ShortcutBarPanelUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 检查必须的数据
bool UPlayerCommandComponent::CheckTargetData()
{
	bool flag = true;
	//if (myPythonActor.IsValid() == false) {
	//	auto gameMode_ = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	if (gameMode_) {
	//		myPythonActor = gameMode_->editEntityManage->myPythonActor;
	//		flag = true;
	//	}
	//}
	selectComponent = Cast<ABaseCharacter>(GetOwner())->selectComponent;

	return flag;
}

// 检查必须的数据
bool UPlayerCommandComponent::CheckDefaultData()
{
	bool checkTwice = false;
	if (gameInstance.IsValid() == false || gameMode.IsValid() == false
		|| playerController.IsValid() == false || hud.IsValid() == false)
	{
		gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController.IsValid())hud = Cast<ABaseHUD>(playerController->GetHUD());

		checkTwice = true;
	}

	bool flag = true;
	if (checkTwice)
	{
		if (gameInstance.IsValid() == false || gameMode.IsValid() == false
			|| playerController.IsValid() == false || hud.IsValid() == false)
		{
			flag = false;
		}
	}

	return flag;
}

// 能否展开UI
bool UPlayerCommandComponent::CanOpenUI()
{
	if (selectComponent.IsValid())
	{
		return selectComponent->IsSelectEntity();
	}
	return false;
}

// 获取 动态获取view
AEditEntityManage* UPlayerCommandComponent::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

// Sets default values for this component's properties
UPlayerCommandComponent::UPlayerCommandComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPlayerCommandComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// START
void UPlayerCommandComponent::Start()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerCommandComponent::Start] "), this);
	CheckDefaultData();
	CheckTargetData();
	view = Cast<ABaseCharacter>(GetOwner());
}

// END
void UPlayerCommandComponent::End()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerCommandComponent::End]   "), this);
}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

// 没有使用的函数

//// game选中实例 -> 展开时间轴ui -> 设置时间轴数据
//void UPlayerCommandComponent::Call_SetUIData(Gamedata::EntityInfo* entityInfo) {
//	//auto timelineMainUserWidget = Cast<UTimelineMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_timeline));
//	//if (timelineMainUserWidget == nullptr || timelineMainUserWidget->timelinePanel.IsValid() == false)return;
//	//auto& timelinePanel = timelineMainUserWidget->timelinePanel;
//
//	//timelinePanel->Call_InitData(false, entityInfo);
//}
//
//// 输入 切换空状态与编辑状态
//bool UPlayerCommandComponent::SwitchEditEntityStaus()
//{
//	auto localPlayerSyncInfo = view->myData->GetSyncInfo();
//	// -> 进入编辑
//	if (localPlayerSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Character) {
//		if (timelineInfo.isTimeline) {
//			view->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity, ECharacterStatusType::CharacterStatusType_EditTimeline);
//		}
//		else {
//			view->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity, ECharacterStatusType::CharacterStatusType_EditEntity);
//		}
//	}
//	// -> 进入游览
//	else if (localPlayerSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity) {
//		SetChildUI_TimelineData(true);
//		view->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_Character, ECharacterStatusType::CharacterStatusType_None);
//	}
//
//	return true;
//}
//
//// 设置子页面记录
//void UPlayerCommandComponent::SetChildUI_TimelineData(bool write) {
//	FString str = write ? TEXT("写入") : TEXT("擦除");
//	auto localPlayerSyncInfo = view->myData->GetSyncInfo();
//	if (write) {
//		// 保存子页面记录
//		if (view->selectComponent->IsSelectEntity() && view->myData->GetInputMode() == ECharacterStatusType::CharacterStatusType_EditTimeline) {
//			timelineInfo.entityId = view->selectComponent->GetSelectMasterInstance();
//			timelineInfo.isTimeline = true;
//		}
//		else {
//			timelineInfo.entityId = 0;
//			timelineInfo.isTimeline = false;
//		}
//		UE_LOG(LogTemp, Log, TEXT("[%x] [时间轴] [SwitchEditEntityStaus] [自动选中%s] [entityId:%d] [isTimeline:%d] "), this, *str, timelineInfo.entityId, timelineInfo.isTimeline);
//	}
//	else {
//		UE_LOG(LogTemp, Log, TEXT("[%x] [时间轴] [SwitchEditEntityStaus] [自动选中%s] [entityId:%d] [isTimeline:%d] "), this, *str, timelineInfo.entityId, timelineInfo.isTimeline);
//		timelineInfo.entityId = 0;
//		timelineInfo.isTimeline = false;
//	}
//}
//
//// 网络回调 使用子页面记录
//void UPlayerCommandComponent::NetCallback_UseTimelineData() {
//	if (timelineInfo.isTimeline) {
//		view->selectComponent->Call_SelectEntity(timelineInfo.entityId);
//	}
//}

//---------------------------------------------------------------//

/*快捷栏执行命令事件*/

// 输入 按下快捷键
void UPlayerCommandComponent::Input_EnterNumber(bool enter, int number)
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto selfPlayerId = view->myData->playerId;
	auto placeView = GetEditEntityManage();

	auto syncInfo = view->myData->GetSyncInfo();

	auto mainui = Cast<UCharacterMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_mainui));
	if (mainui == nullptr || mainui->shortcutBar.IsValid() == false)return;
	auto& shortcutBar = mainui->shortcutBar;
	shortcutBar->Input_Number(number, enter);
}

// 输入 按下快捷键
void UPlayerCommandComponent::UICallback_EnterNumber(std::string commondStr)
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto selfPlayerId = view->myData->playerId;
	auto placeView = GetEditEntityManage();

	auto syncInfo = view->myData->GetSyncInfo();

	// 找到快捷键命令
	std::list<int>playerIdList;
	std::list<int>entityIdList;

	// 找到视野内所有玩家
	for (auto node : baseGameMode->playerMap)
	{
		if (node.Key != selfPlayerId)
		{
			playerIdList.push_back(node.Key);
		}
	}

	// 找到支持该命令的所有entity
	auto tempList = placeView->command->GetCommandEntityList(commondStr);
	if (tempList != nullptr)
	{
		auto& tempList2 = *tempList;
		std::copy(tempList2.begin(), tempList2.end(), std::back_inserter(entityIdList));
	}

	// 发送事件
	Gamedata::EventInfo eventInfo;
	eventInfo.set_player_id(view->myData->playerId);
	eventInfo.set_event_type(Gamedata::EventType_command);
	eventInfo.set_broadcast_range_type(Gamedata::EventBroadcastRangeType_specify);

	for (auto it = playerIdList.begin(); it != playerIdList.end(); ++it)
	{
		eventInfo.add_broadcast_player_list(*it);
	}
	auto command_info = eventInfo.mutable_command_info();
	command_info->set_command(commondStr);
	for (auto it = entityIdList.begin(); it != entityIdList.end(); ++it)
	{
		command_info->add_entity_list(*it);
	}

	FString str2 = UTF8_TO_TCHAR(commondStr.c_str());
	UE_LOG(LogTemp, Log, TEXT("[%x] [命令控制] [Input_EnterNumber] [playerId:%d] [命令:%s]"), this, selfPlayerId, *str2);

	NetCall_Command(eventInfo);
	NetCallback_Command(eventInfo);
}

// 网络请求 命令事件
void UPlayerCommandComponent::NetCall_Command(Gamedata::EventInfo& eventInfo)
{

	std::string sendStr = eventInfo.SerializePartialAsString();
	char* sendData = (char*)sendStr.c_str();
	int sendSize = sendStr.length();
	int type = (int)Gamedata::MsgType::PB_EventInfo;
	gameInstance->netMainComponent->NetCall_GameData(type, sendData, sendSize);
}

// 网络回调 命令事件
void UPlayerCommandComponent::NetCallback_Command(Gamedata::EventInfo& eventInfo)
{

	auto placeView = GetEditEntityManage();

	// 找到所有实例
	if (eventInfo.event_type() == Gamedata::EventType_command && eventInfo.has_command_info())
	{
		auto command = eventInfo.command_info().command();
		auto& entity_list = eventInfo.command_info().entity_list();
		for (auto it = entity_list.begin(); it != entity_list.end(); ++it)
		{
			auto actor = placeView->data->GetEntityActor(*it);
			if (actor.IsValid())
			{
				actor->commandComponent->PlayAnim(command);
			}
		}
	}
}

//---------------------------------------------------------------//

// 命令触发ui关闭 -> 保存更变数据
void UPlayerCommandComponent::SaveEntityDate()
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UEditEntityManageController* placeController = baseGameMode->editEntityManage->controller;
	if (placeController == nullptr)return;

	TArray<int32> entityIdList;
	int entityId = view->selectComponent->GetSelectMasterInstance();
	entityIdList.Add(entityId);
	placeController->NetCall_UpdateData(entityIdList);
}

// 命令按钮ui关闭 -> 保存更变数据
void UPlayerCommandComponent::SavePlayerDate()
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UEditEntityManageController* placeController = baseGameMode->editEntityManage->controller;
	if (placeController == nullptr)return;
	//view->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);

	Gamedata::PlayerInfo newSyncInfo;
	newSyncInfo.CopyFrom(*view->myData->GetSyncInfo());
	newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);
	view->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
}
