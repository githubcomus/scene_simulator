// Fill out your copyright notice in the Description page of Project Settings.

#include "NewTimelineTimeUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
//#include "TimelinePanelUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

//
void UNewTimelineTimeUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UNewTimelineTimeUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UNewTimelineTimeUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UNewTimelineTimeUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UNewTimelineTimeUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UNewTimelineTimeUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UNewTimelineTimeUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UNewTimelineTimeUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UNewTimelineTimeUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

// // 获取 点数据
// FTimelineEnterPointInfo* UNewTimelineTimeUserWidget::GetTimelinePointInfo()
// {
// 	if (panel == nullptr)return nullptr;
//
// 	return panel->GetTimelinePointInfo(pointIndexInfo.entityId, pointIndexInfo.pointInfoListIndex);
// }

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
void UNewTimelineTimeUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	//timelinePanel = Cast<UTimelinePanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_TimelinePanel")));
}

//
void UNewTimelineTimeUserWidget::SetStr(FString str)
{
	// if (textBlock.IsValid() == false)return;
	//
	//
	// textBlock->SetText(FText::FromString(str));
}

//
void UNewTimelineTimeUserWidget::SetSelect(bool isSelect_)
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
void UNewTimelineTimeUserWidget::OnMouseEnterLeave(bool enter)
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

// 操作 新增一个点
void UNewTimelineTimeUserWidget::AddPoint()
{
}

// 操作 移除一个点
void UNewTimelineTimeUserWidget::RemovePoint()
{
}

// 操作 拖动一个点
void UNewTimelineTimeUserWidget::MovePoint()
{
}

// 操作 跟新点内数据
void UNewTimelineTimeUserWidget::UpdatePoint()
{
}
