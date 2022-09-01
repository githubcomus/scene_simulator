// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandTriggerPanelUserWidget.h"

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

/*

实例的命令触发UI,自动触发和命令文本触发.
被隐藏的 交互触发和碰撞触发.

*/


/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/
//
void UCommandTriggerPanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UCommandTriggerPanelUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UCommandTriggerPanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UCommandTriggerPanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UCommandTriggerPanelUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UCommandTriggerPanelUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UCommandTriggerPanelUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UCommandTriggerPanelUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UCommandTriggerPanelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 外部数据 获取 动态获取view
TWeakObjectPtr<AEditEntityManage> UCommandTriggerPanelUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> UCommandTriggerPanelUserWidget::GetEntityAcotr(int entityId)
{
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// 获取当前选中的actor
TWeakObjectPtr<AEditEntityActor> UCommandTriggerPanelUserWidget::GetSelectEntityActor()
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

// bp to ui 数据
void UCommandTriggerPanelUserWidget::ProtobufToBPInfo(Gamedata::EntityCommandInfo* pbInfo, FEntityCommandInfo* uiInfo)
{

	uiInfo->type = (EEntityCommandType)pbInfo->type();

	{
		auto command_list = pbInfo->mutable_command_list();
		uiInfo->command_list.Reset();
		for (auto it = command_list->begin(); it != command_list->end(); ++it)
		{

			auto& pb_command = it;
			FEntityCommandCommand ui_command;
			ui_command.command_name = UTF8_TO_TCHAR(pb_command->command_name().c_str());
			ui_command.end_time = pb_command->end_time();
			ui_command.start_time = pb_command->start_time();

			uiInfo->command_list.Add(ui_command);
		}
	}

	{
		auto pb_interactive = pbInfo->mutable_interactive();
		FEntityCommandInteractive& ui_interactive = uiInfo->interactive;
		ui_interactive.end_time = pb_interactive->end_time();
		ui_interactive.start_time = pb_interactive->start_time();
		ui_interactive.is_fixed_position = pb_interactive->is_fixed_position();
		ui_interactive.is_open = pb_interactive->is_open();
	}

	{
		auto pb_trigger = pbInfo->mutable_trigger();
		FEntityCommandTrigger& ui_trigger = uiInfo->trigger;
		ui_trigger.end_time = pb_trigger->end_time();
		ui_trigger.start_time = pb_trigger->start_time();
		ui_trigger.is_open = pb_trigger->is_open();
	}
}

// ui to bp 数据
void UCommandTriggerPanelUserWidget::BPInfoToProtobuf(Gamedata::EntityCommandInfo* pbInfo, FEntityCommandInfo* uiInfo)
{
	pbInfo->set_type((Gamedata::EntityCommandType)uiInfo->type);

	{
		pbInfo->clear_command_list();

		for (auto node : uiInfo->command_list)
		{
			auto pb_command = pbInfo->add_command_list();
			auto& ui_command = node;
			std::string str(TCHAR_TO_UTF8(*ui_command.command_name));
			pb_command->set_command_name(str);
			pb_command->set_end_time(ui_command.end_time);
			pb_command->set_start_time(ui_command.start_time);
		}
	}

	{
		auto pb_interactive = pbInfo->mutable_interactive();
		FEntityCommandInteractive& ui_interactive = uiInfo->interactive;
		pb_interactive->set_end_time(ui_interactive.end_time);
		pb_interactive->set_start_time(ui_interactive.start_time);
		pb_interactive->set_is_fixed_position(ui_interactive.is_fixed_position);
		pb_interactive->set_is_open(ui_interactive.is_open);
	}

	{
		auto pb_trigger = pbInfo->mutable_trigger();
		FEntityCommandTrigger& ui_trigger = uiInfo->trigger;
		pb_trigger->set_end_time(ui_trigger.end_time);
		pb_trigger->set_start_time(ui_trigger.start_time);
		pb_trigger->set_is_open(ui_trigger.is_open);
	}
}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UCommandTriggerPanelUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

}

// UI调用 展开收起事件
void UCommandTriggerPanelUserWidget::OnExpandCollapse(bool isShow)
{

	auto actor = GetSelectEntityActor();
	if (actor == nullptr)return;

	FString str = isShow ? TEXT("展开") : TEXT("折叠");
	UE_LOG(LogTemp, Log, TEXT("[%x] [命令控制] [OnExpandCollapse] [%s]"), this, *str);

	if (actor->syncInfo.has_command() == false)
	{
		Gamedata::EntityCommandInfo pbInfo;
		ProtobufToBPInfo(&pbInfo, &syncInfoCommand);
	}

	if (isShow)
	{
		OpenUI();
	}
	else
	{
		CloseUI();
	}

}

// UI调用 展开
void UCommandTriggerPanelUserWidget::OpenUI()
{

	auto actor = GetSelectEntityActor();
	if (actor == nullptr)return;

	changeCount = 0;

	ProtobufToBPInfo(actor->syncInfo.mutable_command(), &syncInfoCommand);

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
void UCommandTriggerPanelUserWidget::CloseUI()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	if (character->commandComponent == nullptr)return;

	auto actor = GetSelectEntityActor();
	if (actor == nullptr)return;

	BPInfoToProtobuf(actor->syncInfo.mutable_command(), &syncInfoCommand);

	// 通知网络更新
	if (changeCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [命令控制] [SaveData] 保存数据 [entityId:%d]"), this, actor->entityId);
		character->commandComponent->SaveEntityDate();
	}
	changeCount = 0;
}

// UI调用 切换页签
void UCommandTriggerPanelUserWidget::DateUpdateChangePage(EEntityCommandType type)
{
	syncInfoCommand.type = type;
	changeCount++;
}

// UI调用 命令文本 增加数据
void UCommandTriggerPanelUserWidget::DateAddCommand(FEntityCommandCommand command)
{
	syncInfoCommand.command_list.Add(command);
	changeCount++;
}

// UI调用 命令文本 删除数据
void UCommandTriggerPanelUserWidget::DateRemoveCommand(int valueIndex)
{
	syncInfoCommand.command_list.RemoveAt(valueIndex);
	changeCount++;
}

// UI调用 命令文本 更新数据
void UCommandTriggerPanelUserWidget::DateUpdateCommand(int valueIndex, FEntityCommandCommand command)
{
	if (syncInfoCommand.command_list.Num() > valueIndex)
	{
		syncInfoCommand.command_list[valueIndex] = command;
	}
	changeCount++;
}

// UI调用 命令交互

// UI调用 命令碰撞