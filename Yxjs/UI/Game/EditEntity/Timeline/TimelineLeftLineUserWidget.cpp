// Fill out your copyright notice in the Description page of Project Settings.


#include "TimelineLeftLineUserWidget.h"

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
void UTimelineLeftLineUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UTimelineLeftLineUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UTimelineLeftLineUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTimelineLeftLineUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UTimelineLeftLineUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UTimelineLeftLineUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UTimelineLeftLineUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);


}

//
void UTimelineLeftLineUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);


}

//
FReply UTimelineLeftLineUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// data
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/


// // 获取 点数据
// FTimelineEnterPointInfo* UTimelineLeftLineUserWidget::GetTimelinePointInfo()
// {
// 	if (panel == nullptr)return nullptr;
//
// 	return panel->GetTimelinePointInfo(pointIndexInfo.entityId, pointIndexInfo.pointInfoListIndex);
// }

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// controller
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTimelineLeftLineUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	textBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_70")));

	// image = Cast<UImage>(GetWidgetFromName(TEXT("Image_92")));

}

//
void UTimelineLeftLineUserWidget::SetStr(FString str)
{
	if (textBlock.IsValid() == false)return;


	textBlock->SetText(FText::FromString(str));
}

// 
void UTimelineLeftLineUserWidget::SetSelect(bool isSelect_)
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
void UTimelineLeftLineUserWidget::OnMouseEnterLeave(bool enter)
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