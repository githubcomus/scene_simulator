// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityMainUserWidget.h"
#include "Pawn/Character/CharacterComponent/PlayerEditEntityComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerTimelineComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Yxjs/UI/BaseHUD.h"
#include "Yxjs/Pawn/Character/BaseCharacter.h"
#include "EditEntityMainButtonUserWidget.h"
#include "Yxjs/GameMode/BaseGameMode.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Components/Slider.h>
#include "../../../../FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "EditEntityMainBaseSyncInfoUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> UEditEntityMainUserWidget::GetEntityAcotr(int entityId)
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto view = baseGameMode->editEntityManage;
	if (view == nullptr)return nullptr;
	return view->data->GetEntityActor(entityId);
}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UEditEntityMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEditEntityMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEditEntityMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEditEntityMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UEditEntityMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UEditEntityMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEditEntityMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UEditEntityMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
FReply UEditEntityMainUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return FReply::Handled();
}

//
FReply UEditEntityMainUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return FReply::Handled();
}

//
void UEditEntityMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	BP_UI_EditEntity_Button_merge = Cast<UEditEntityMainButtonUserWidget>(GetWidgetFromName(TEXT("BP_UI_EditEntity_Button_merge")));
	BP_UI_EditEntity_Button_break_down = Cast<UEditEntityMainButtonUserWidget>(GetWidgetFromName(TEXT("BP_UI_EditEntity_Button_break_down")));
	BP_UI_EditEntity_BaseSyncInfo = Cast<UEditEntityMainBaseSyncInfoUserWidget>(GetWidgetFromName(TEXT("BP_UI_EditEntity_BaseSyncInfo")));
	TextBlock_ctrl = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_ctrl")));

	Slider_speed = Cast<USlider>(GetWidgetFromName(TEXT("Slider_speed")));

	if (Slider_speed.IsValid())
	{
		FScriptDelegate func;
		func.BindUFunction(this, TEXT("MyOnValueChanged"));
		Slider_speed->OnValueChanged.AddUnique(func);
	}

	Slider_speed->SetValue(0.5);
	MyOnValueChanged(0.5);
}

// 展开收起
void UEditEntityMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation)
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

	if (auto Event_ChangeChildUI = FindFunction("Event_ChangeChildUI"))
	{

		struct FUIInfo_ChangeChildUIBroadcast
		{
			bool isShow;
			bool useAnimation = false;
		};
		FUIInfo_ChangeChildUIBroadcast info;
		info.isShow = isShow;
		info.useAnimation = useAnimation;
		ProcessEvent(Event_ChangeChildUI, &info);
	};

	if (isShow)
	{
		float var = character->myData->editMoveSpeed / 30.0f;
		Slider_speed->SetValue(var);
	}
	else
	{
	}

}

// 切换主状态保存零时变量 
void UEditEntityMainUserWidget::SaveTempDate(EUITempDateType type)
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	auto hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud == nullptr)return;

	tempUIInfo.type = EUITempDateType::UITempDateType_none;

	if (type == EUITempDateType::UITempDateType_EditEntity)
	{
		if (character->selectComponent->IsSelectEntity() && hud->childUI == EUIChildName::ui_name_edit_entity)
		{
			tempUIInfo.editEntity.entityId = character->selectComponent->GetSelectMasterInstance();
			tempUIInfo.type = type;
			//UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [编辑实例] [SwitchEditEntityStaus] [预处理保存] EditEntityTempDateType_EditEntity 开始 [entityId:%d] "), this, tempUIInfo.timelineInfo.entityId);
		}
	}
	else if (type == EUITempDateType::UITempDateType_Timeline && hud->childUI == EUIChildName::ui_name_timeline)
	{
		if (character->selectComponent->IsSelectEntity())
		{
			tempUIInfo.timelineInfo.entityId = character->selectComponent->GetSelectMasterInstance();
			tempUIInfo.type = type;
			//UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [编辑实例] [SwitchEditEntityStaus] [预处理保存] EditEntityTempDateType_Timeline 开始 [entityId:%d] "), this, tempUIInfo.timelineInfo.entityId);
		}
	}
	else if (type == EUITempDateType::UITempDateType_CommandTrigger && hud->childUI == EUIChildName::ui_name_command_trigger)
	{
		if (character->selectComponent->IsSelectEntity())
		{
			tempUIInfo.commandTrigger.entityId = character->selectComponent->GetSelectMasterInstance();
			tempUIInfo.type = type;
			//UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [编辑实例] [SwitchEditEntityStaus] [预处理保存] EditEntityTempDateType_Timeline 开始 [entityId:%d] "), this, tempUIInfo.timelineInfo.entityId);
		}
	}
}

// 进入主状态读取零时变量
void UEditEntityMainUserWidget::UseTempDate()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	auto hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud == nullptr)return;

	if (tempUIInfo.type == EUITempDateType::UITempDateType_none)return;


	if (tempUIInfo.type == EUITempDateType::UITempDateType_EditEntity)
	{
		//UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [编辑实例] [SwitchEditEntityStaus] [预处理执行] EditEntityTempDateType_EditEntity  结束 [entityId:%d] "), this, tempUIInfo.timelineInfo.entityId);
		character->selectComponent->Call_SelectEntity(tempUIInfo.editEntity.entityId);// 选中实例
		tempUIInfo.editEntity.entityId = 0;
	}
	else if (tempUIInfo.type == EUITempDateType::UITempDateType_Timeline)
	{
		//UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [编辑实例] [SwitchEditEntityStaus] [预处理执行] EditEntityTempDateType_Timeline 结束 [entityId:%d] "), this, tempUIInfo.timelineInfo.entityId);
		character->selectComponent->Call_SelectEntity(tempUIInfo.timelineInfo.entityId);// 选中实例
		hud->UICallback_OpenChildUI(EUIChildName::ui_name_timeline, true);
		tempUIInfo.timelineInfo.entityId = 0;
	}
	else if (tempUIInfo.type == EUITempDateType::UITempDateType_CommandTrigger)
	{
		character->selectComponent->Call_SelectEntity(tempUIInfo.commandTrigger.entityId);// 选中实例
		hud->UICallback_OpenChildUI(EUIChildName::ui_name_command_trigger, true);
		tempUIInfo.commandTrigger.entityId = 0;
	}
	tempUIInfo.type = EUITempDateType::UITempDateType_none;
}

// 鼠标选中实例
void UEditEntityMainUserWidget::SetButtonStopEnter(std::vector<int>& selectList)
{

	// 合并按钮
	if (selectList.size() <= 1)
	{
		BP_UI_EditEntity_Button_merge->SetStopEnter(true);
	}
	else
	{
		BP_UI_EditEntity_Button_merge->SetStopEnter(false);
	}

	// 分解按钮
	bool flag = false;
	if (selectList.size() == 1)
	{
		auto entityActor = GetEntityAcotr(selectList[0]);
		if (entityActor->syncInfo.child_list_size() >= 2)
		{
			flag = true;
			BP_UI_EditEntity_Button_break_down->SetStopEnter(false);
		}
	}
	if (flag == false)
	{
		BP_UI_EditEntity_Button_break_down->SetStopEnter(true);
	}
}

// 按住ctrl
void UEditEntityMainUserWidget::EnterCtrl(bool enter)
{
	if (enter)
	{
		TextBlock_ctrl->SetText(FText::FromString(TEXT("多选")));
	}
	else
	{
		TextBlock_ctrl->SetText(FText::FromString(TEXT("单选")));
	}
}

// 速度值改变
void UEditEntityMainUserWidget::MyOnValueChanged(float value)
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	character->myData->editMoveSpeed = value * 30;
	UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [编辑实例] [SwitchEditEntityStaus] [速度]  [value:%f] [speed:%f]"), this, value, character->myData->editMoveSpeed);
}