// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Game/EditEntity/Timeline/TimelineMainUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TimelinePanelUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
void UTimelineMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UTimelineMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTimelineMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UTimelineMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UTimelineMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UTimelineMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UTimelineMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UTimelineMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
void UTimelineMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	timelinePanel = Cast<UTimelinePanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_TimelinePanel")));

}

//
void UTimelineMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation)
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

	timelinePanel->OnExpandCollapse(isShow);
}

//
void UTimelineMainUserWidget::SetStr(FString str)
{
	// if (textBlock.IsValid() == false)return;
	//
	//
	// textBlock->SetText(FText::FromString(str));
}

//
void UTimelineMainUserWidget::SetSelect(bool isSelect_)
{
	// if(isSelect_)
	// {
	// 	FLinearColor inColorAndOpacity(0, 0, 0, 1);
	// 	image->SetColorAndOpacity(inColorAndOpacity);
	// }
	// else
	// {
	// 	FLinearColor inColorAndOpacity(255, 255, 255, 1);
	// 	image->SetColorAndOpacity(inColorAndOpacity);
	// }
}

//
void UTimelineMainUserWidget::OnMouseEnterLeave(bool enter)
{
	// if(enter)
	// {
	// 	FLinearColor inColorAndOpacity(222, 222, 0, 1);
	// 	image->SetColorAndOpacity(inColorAndOpacity);
	//
	// 	// panel->cuoi.mouseRealTimeMode = true;
	// 	// panel->cuoi.stopInPoint_pointIndex.entityId = pointIndexInfo.entityId;
	// 	// panel->cuoi.stopInPoint_pointIndex.pointInfoListIndex = pointIndexInfo.pointInfoListIndex;
	// }
	// else
	// {
	// 	auto timelinePointInfo = GetTimelinePointInfo();
	// 	if (timelinePointInfo == nullptr)return;
	//
	// 	SetSelect(timelinePointInfo->isSelect);
	//
	// 	// panel->cuoi.mouseRealTimeMode = false;
	// 	// panel->cuoi.stopInPoint_pointIndex.entityId = 0;
	// 	// panel->cuoi.stopInPoint_pointIndex.pointInfoListIndex = 0;
	// }
}