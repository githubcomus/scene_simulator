// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandTriggerMainUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CommandTriggerPanelUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UCommandTriggerMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UCommandTriggerMainUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UCommandTriggerMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UCommandTriggerMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UCommandTriggerMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UCommandTriggerMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UCommandTriggerMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UCommandTriggerMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UCommandTriggerMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// // 获取 点数据
// FTimelineEnterPointInfo* UCommandTriggerMainUserWidget::GetTimelinePointInfo()
// {
// 	if (panel == nullptr)return nullptr;
//
// 	return panel->GetTimelinePointInfo(pointIndexInfo.entityId, pointIndexInfo.pointInfoListIndex);
// }

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UCommandTriggerMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	panelUserWidget = Cast<UCommandTriggerPanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_Command_Panel")));

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UCommandTriggerMainUserWidget::Init] "), this);
}

//
void UCommandTriggerMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation)
{
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
	panelUserWidget->OnExpandCollapse(isShow);
}