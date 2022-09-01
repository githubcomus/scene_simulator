// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn/Character/CharacterComponent/PlayerTimelineComponent.h"
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

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 检查必须的数据
bool UPlayerTimelineComponent::CheckTargetData()
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
bool UPlayerTimelineComponent::CheckDefaultData()
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
bool UPlayerTimelineComponent::CanOpenUI()
{
	if (selectComponent.IsValid())
	{
		return selectComponent->IsSelectEntity();
	}
	return false;
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

// Sets default values for this component's properties
UPlayerTimelineComponent::UPlayerTimelineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPlayerTimelineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// START
void UPlayerTimelineComponent::Start()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerTimelineComponent::Start] "), this);
	CheckDefaultData();
	CheckTargetData();
	view = Cast<ABaseCharacter>(GetOwner());
}

// END
void UPlayerTimelineComponent::End()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerTimelineComponent::End]   "), this);
}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

// game选中实例 -> 展开时间轴ui -> 设置时间轴数据
void UPlayerTimelineComponent::Call_SetUIData(Gamedata::EntityInfo* entityInfo)
{
	auto timelineMainUserWidget = Cast<UTimelineMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_timeline));
	if (timelineMainUserWidget == nullptr || timelineMainUserWidget->timelinePanel.IsValid() == false)return;
	auto& timelinePanel = timelineMainUserWidget->timelinePanel;

	timelinePanel->Call_InitData(false, entityInfo);
}

//// 时间轴ui关闭 -> 保存更变数据
//void UPlayerTimelineComponent::SaveChangeDate() {
//	auto timelineMainUserWidget = Cast<UTimelineMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_timeline));
//	if (timelineMainUserWidget == nullptr || timelineMainUserWidget->timelinePanel.IsValid() == false)return;
//	auto& timelinePanel = timelineMainUserWidget->timelinePanel;
//
//	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//	UEditEntityManageController* placeController = baseGameMode->editEntityManage->controller;
//	if (placeController == nullptr)return;
//
//	TArray<int32> entityIdList;
//	int entityId = view->selectComponent->GetSelectMasterInstance();
//	entityIdList.Add(entityId);
//	if (timelinePanel->IsNeedSaveData()) {
//		placeController->NetCall_UpdateData(entityIdList);
//	}
//}

//---------------------------------------------------------------

// 疑惑,究竟使用哪个函数

// 输入 切换空状态与编辑状态
bool UPlayerTimelineComponent::SwitchEditEntityStaus()
{
	//auto hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	//if (hud == nullptr)return;
	auto localPlayerSyncInfo = view->myData->GetSyncInfo();
	// -> 进入编辑
	if (localPlayerSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Character)
	{

		//view->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);

		Gamedata::PlayerInfo newSyncInfo;
		newSyncInfo.CopyFrom(*view->myData->GetSyncInfo());
		newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);
		view->myController->NetCall_UpdatePlayerInfo(newSyncInfo);

	}
	// -> 进入游览
	else if (localPlayerSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity)
	{
		//view->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_Character);
		Gamedata::PlayerInfo newSyncInfo;
		newSyncInfo.CopyFrom(*view->myData->GetSyncInfo());
		newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_Character);
		view->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
	}

	return true;
}


// 本地更变 进入编辑时间轴状态
void UPlayerTimelineComponent::Call_EnterEditTimelineStaus(bool isEnter)
{
	FString str = isEnter ? TEXT("进入") : TEXT("退出");
	UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [NetCallback_EnterEditTimelineStaus] [时间轴编辑] [%s]  "), this, *str);

	auto& placeData = view->myData->gameMode->editEntityManage->data;

	// 进入
	if (isEnter)
	{
		// vidw
		//view->EnterEditStaus();

		// 本地客户端
		if (view->myData->isLocal)
		{
			view->selectComponent->Input_SetWorkMode(ESelectedWorkMode::WorldLocation);

		}
	}
	// 退出
	else
	{
	}
}

//---------------------------------------------------------------

// 废弃

//// 设置子页面记录
//void UPlayerTimelineComponent::SetChildUI_TimelineData(bool write) {
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
//void UPlayerTimelineComponent::NetCallback_UseTimelineData() {
//	if (timelineInfo.isTimeline) {
//		view->selectComponent->Call_SelectEntity(timelineInfo.entityId);
//	}
//}

/*------------------------------------------------------------------*/
