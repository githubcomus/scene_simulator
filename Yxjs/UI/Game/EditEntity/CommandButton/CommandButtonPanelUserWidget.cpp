// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandButtonPanelUserWidget.h"

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

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/
//
void UCommandButtonPanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UCommandButtonPanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UCommandButtonPanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UCommandButtonPanelUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
FReply UCommandButtonPanelUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
FReply UCommandButtonPanelUserWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	return FReply::Handled();
}

//
void UCommandButtonPanelUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UCommandButtonPanelUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UCommandButtonPanelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// ???????????? ?????? ????????????view
TWeakObjectPtr<AEditEntityManage> UCommandButtonPanelUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// ???????????? ????????????
TWeakObjectPtr<AEditEntityActor> UCommandButtonPanelUserWidget::GetEntityAcotr(int entityId)
{
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// ?????????????????????actor
TWeakObjectPtr<AEditEntityActor> UCommandButtonPanelUserWidget::GetSelectEntityActor()
{

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

// bp to ui ??????
void UCommandButtonPanelUserWidget::ProtobufToBPInfo(Gamedata::PlayerEntityButtonInfo* pbInfo, FPlayerEntityButtonInfo* uiInfo)
{


	auto tempFunction = [](TArray<FString>& ui_command_list, google::protobuf::RepeatedPtrField< ::std::string>* pb_command_list) {

		ui_command_list.Reset();
		for (auto it = pb_command_list->begin(); it != pb_command_list->end(); ++it)
		{
			auto& pb_command_str = *it;
			FString command = UTF8_TO_TCHAR(pb_command_str.c_str());
			ui_command_list.Add(command);
		}
		return true;
	};

	uiInfo->keyList.Reset();

	for (auto node = pbInfo->mutable_key_list()->begin(); node != pbInfo->mutable_key_list()->end(); ++node)
	{
		FPlayerButtonKeyInfo keyInfo;
		tempFunction(keyInfo.commandList, node->mutable_command_list());
		uiInfo->keyList.Add(keyInfo);
	}
}

// ui to bp ??????
void UCommandButtonPanelUserWidget::BPInfoToProtobuf(Gamedata::PlayerEntityButtonInfo* pbInfo, FPlayerEntityButtonInfo* uiInfo)
{

	auto tempFunction = [](TArray<FString>& ui_command_list, google::protobuf::RepeatedPtrField< ::std::string>* pb_command_list) {
		for (auto node : ui_command_list)
		{
			std::string str(TCHAR_TO_UTF8(*node));
			pb_command_list->Add(str.c_str());
		}
		return true;
	};

	pbInfo->clear_key_list();
	for (auto node : uiInfo->keyList)
	{
		auto keyInfo = pbInfo->add_key_list();
		tempFunction(node.commandList, keyInfo->mutable_command_list());
	}
}

// ?????? ??????????????????

// ????????????
Gamedata::PlayerInfo* UCommandButtonPanelUserWidget::GetPlayerSyncInfo()
{
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return nullptr;

	auto syncInfo = character->myData->GetSyncInfo();
	return syncInfo;

}


/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

// UI??????????????????,??????????????????
void UCommandButtonPanelUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

}

// UI?????? ??????????????????
void UCommandButtonPanelUserWidget::OnExpandCollapse(bool isShow)
{

	auto syncInfo = GetPlayerSyncInfo();
	if (syncInfo == nullptr)return;

	// ???????????????
	{
		if (syncInfo->has_entity_button_info() == false || syncInfo->mutable_entity_button_info()->key_list_size() != 10)
		{
			auto pbInfo = syncInfo->mutable_entity_button_info();
			pbInfo->clear_key_list();
			for (auto i = 0; i < 10; i++)
			{
				pbInfo->add_key_list();
			}
			ProtobufToBPInfo(pbInfo, &syncInfoCommand);
		}
	}

	if (isShow)
	{
		OpenUI();
	}
	else
	{
		CloseUI();
	}

	FString str = isShow ? TEXT("??????") : TEXT("??????");
	UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [OnExpandCollapse] [%s]"), this, *str);
}

// UI?????? ??????
void UCommandButtonPanelUserWidget::OpenUI()
{

	changeCount = 0;

	auto syncInfo = GetPlayerSyncInfo();
	if (syncInfo == nullptr)return;

	ProtobufToBPInfo(syncInfo->mutable_entity_button_info(), &syncInfoCommand);

	if (UFunction* func = FindFunction("Event_OpenUI"))
	{
		struct TempParam
		{
		};
		TempParam tempParam;

		ProcessEvent(func, &tempParam);
	}
}

// UI?????? ??????
void UCommandButtonPanelUserWidget::CloseUI()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	if (character->commandComponent == nullptr)return;

	BPInfoToProtobuf(character->myData->GetSyncInfo()->mutable_entity_button_info(), &syncInfoCommand);

	// ??????????????????
	if (changeCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [SaveData] ????????????"), this);
		character->commandComponent->SavePlayerDate();
	}
	changeCount = 0;
}

// UI?????? ????????????
void UCommandButtonPanelUserWidget::AddLineData(int keyIndex, FString commandStr)
{
	changeCount++;
	if (syncInfoCommand.keyList.Num() > keyIndex)
	{
		syncInfoCommand.keyList[keyIndex].commandList.Add(commandStr);
	}
}

// UI?????? ????????????
void UCommandButtonPanelUserWidget::UpdateLineData(int keyIndex, int commandIndex, FString commandStr)
{
	changeCount++;
	if (syncInfoCommand.keyList.Num() > keyIndex)
	{
		auto& commandList = syncInfoCommand.keyList[keyIndex].commandList;
		if (commandList.Num() > commandIndex)
		{
			commandList[commandIndex] = commandStr;
		}
	}
}

// UI?????? ????????????
void UCommandButtonPanelUserWidget::RemoveLineData(int keyIndex, int commandIndex)
{
	changeCount++;
	if (syncInfoCommand.keyList.Num() > keyIndex)
	{
		auto& commandList = syncInfoCommand.keyList[keyIndex].commandList;
		if (commandList.Num() > commandIndex)
		{
			commandList.RemoveAt(commandIndex);
		}
	}
}


// 