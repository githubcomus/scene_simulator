// Fill out your copyright notice in the Description page of Project Settings.

#include "NewTimelineMainUserWidget.h"

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
void UNewTimelineMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UNewTimelineMainUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UNewTimelineMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UNewTimelineMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UNewTimelineMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UNewTimelineMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UNewTimelineMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UNewTimelineMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UNewTimelineMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

// // 获取 点数据
// FTimelineEnterPointInfo* UNewTimelineMainUserWidget::GetTimelinePointInfo()
// {
// 	if (panel == nullptr)return nullptr;
//
// 	return panel->GetTimelinePointInfo(pointIndexInfo.entityId, pointIndexInfo.pointInfoListIndex);
// }

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
void UNewTimelineMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;
	LoadTestData();
	//timelinePanel = Cast<UTimelinePanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_TimelinePanel")));
}

//
void UNewTimelineMainUserWidget::SetStr(FString str)
{
	// if (textBlock.IsValid() == false)return;
	//
	//
	// textBlock->SetText(FText::FromString(str));
}

//
void UNewTimelineMainUserWidget::SetSelect(bool isSelect_)
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
void UNewTimelineMainUserWidget::OnMouseEnterLeave(bool enter)
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

//// 加载一行数据
//void UNewTimelineMainUserWidget::LoadOneData() {
//	// 设置初始长度
//
//	// 往上加点
//}

// 拖动

// 增加一个点

// 移除一个点

// 拖动一个点

// 加载测试数据
void UNewTimelineMainUserWidget::LoadTestData()
{
	AddOneData();
}

// 添加一行数据
void UNewTimelineMainUserWidget::AddOneData()
{
	//syncInfo.mutable_transform_timeline()->
	//	set_time_lenght(21);
	auto& point_map = *syncInfo.mutable_transform_timeline()->mutable_point_map();

	for (int i = 0; i < 21; i++)
	{
		int time = i * 1000;
		FRotator rotator(0, 0, 0);
		FVector location(0, 0, 0);
		if (i < 10)
		{
			location = FVector(0, 0, i * 50);
		}
		else
		{
			auto temp_i = 20 - i;
			location = FVector(0, 0, temp_i * 50);
		}
		Gamedata::EntityTransformTimelinePoint entityTransformTimelinePoint;
		entityTransformTimelinePoint.set_uniquemark(i);
		entityTransformTimelinePoint.mutable_location()->set_x(location.X);
		entityTransformTimelinePoint.mutable_location()->set_y(location.Y);
		entityTransformTimelinePoint.mutable_location()->set_z(location.Z);
		entityTransformTimelinePoint.mutable_rotator()->set_pitch(rotator.Pitch);
		entityTransformTimelinePoint.mutable_rotator()->set_roll(rotator.Roll);
		entityTransformTimelinePoint.mutable_rotator()->set_yaw(rotator.Yaw);
		point_map.insert({ time ,entityTransformTimelinePoint });
	}
}