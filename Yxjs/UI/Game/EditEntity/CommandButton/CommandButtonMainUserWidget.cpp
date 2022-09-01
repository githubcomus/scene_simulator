// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandButtonMainUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CommandButtonPanelUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UCommandButtonMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UCommandButtonMainUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UCommandButtonMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UCommandButtonMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UCommandButtonMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UCommandButtonMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UCommandButtonMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UCommandButtonMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UCommandButtonMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// // 获取 点数据
// FTimelineEnterPointInfo* UCommandButtonMainUserWidget::GetTimelinePointInfo()
// {
// 	if (panel == nullptr)return nullptr;
//
// 	return panel->GetTimelinePointInfo(pointIndexInfo.entityId, pointIndexInfo.pointInfoListIndex);
// }

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UCommandButtonMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	panelUserWidget = Cast<UCommandButtonPanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_CommandButton_Panel")));

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UCommandButtonMainUserWidget::Init] "), this);
}

//
void UCommandButtonMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation)
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