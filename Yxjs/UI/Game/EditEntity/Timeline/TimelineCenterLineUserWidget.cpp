// Fill out your copyright notice in the Description page of Project Settings.


#include "TimelineCenterLineUserWidget.h"

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
void UTimelineCenterLineUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UTimelineCenterLineUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UTimelineCenterLineUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTimelineCenterLineUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// data
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

// // 获取 点数据
// FTimelineEnterPointInfo* UTimelineCenterLineUserWidget::GetTimelinePointInfo()
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
void UTimelineCenterLineUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	// textBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_70")));

	image = Cast<UImage>(GetWidgetFromName(TEXT("Image_backpack")));

}

//
void UTimelineCenterLineUserWidget::SetStr(FString str)
{
	// if (textBlock.IsValid() == false)return;
	//
	//
	// textBlock->SetText(FText::FromString(str));
}

// 
void UTimelineCenterLineUserWidget::SetSelect(bool isSelect_)
{
	if (image.IsValid() == false)return;

	if (isSelect_)
	{
		FLinearColor inColorAndOpacity(111, 22, 111, 0.6);
		image->SetColorAndOpacity(inColorAndOpacity);
	}
	else
	{
		FLinearColor inColorAndOpacity(111, 111, 255, 0.3);
		image->SetColorAndOpacity(inColorAndOpacity);
	}
}

//
void UTimelineCenterLineUserWidget::OnMouseEnterLeave(bool enter)
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