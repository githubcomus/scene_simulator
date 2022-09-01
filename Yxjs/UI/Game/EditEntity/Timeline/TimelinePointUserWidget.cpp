// Fill out your copyright notice in the Description page of Project Settings.


#include "TimelinePointUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TimelinePanelUserWidget.h"

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// view
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTimelinePointUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UTimelinePointUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UTimelinePointUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTimelinePointUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UTimelinePointUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UTimelinePointUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UTimelinePointUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);


}

//
void UTimelinePointUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);


}

//
FReply UTimelinePointUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// data
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/


// 获取 点数据
FTimelineEnterPointInfo* UTimelinePointUserWidget::GetTimelinePointInfo()
{
	if (panel == nullptr)return nullptr;

	return panel->GetTimelinePointInfoWithUniqueMark(pointIndexInfo.entityId, pointIndexInfo.uniqueMark);
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// controller
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTimelinePointUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	TextBlock_str = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_str")));

	image = Cast<UImage>(GetWidgetFromName(TEXT("Image_92")));

}

//
void UTimelinePointUserWidget::SetStr(FString str)
{
	if (TextBlock_str.IsValid() == false)return;


	TextBlock_str->SetText(FText::FromString(str));
}

// 
void UTimelinePointUserWidget::SetSelect(bool isSelect_)
{
	if (isSelect_)
	{
		FLinearColor inColorAndOpacity(255, 0, 0, 1);
		image->SetColorAndOpacity(inColorAndOpacity);
	}
	else
	{
		FLinearColor inColorAndOpacity(0, 205, 205, 1);
		image->SetColorAndOpacity(inColorAndOpacity);
	}
}

//
void UTimelinePointUserWidget::OnMouseEnterLeave(bool enter)
{
	if (enter)
	{
		FLinearColor inColorAndOpacity(160, 32, 240, 1);
		image->SetColorAndOpacity(inColorAndOpacity);

		// panel->cuoi.mouseRealTimeMode = true;
		// panel->cuoi.stopInPoint_pointIndex.entityId = pointIndexInfo.entityId;
		// panel->cuoi.stopInPoint_pointIndex.pointInfoListIndex = pointIndexInfo.pointInfoListIndex;
	}
	else
	{
		auto timelinePointInfo = GetTimelinePointInfo();
		if (timelinePointInfo == nullptr)return;

		SetSelect(timelinePointInfo->isSelect);

		// panel->cuoi.mouseRealTimeMode = false;
		// panel->cuoi.stopInPoint_pointIndex.entityId = 0;
		// panel->cuoi.stopInPoint_pointIndex.pointInfoListIndex = 0;
	}
}