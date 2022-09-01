// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityMaterialMainUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EntityMaterialPanelUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UEntityMaterialMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UEntityMaterialMainUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UEntityMaterialMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityMaterialMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEntityMaterialMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UEntityMaterialMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UEntityMaterialMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEntityMaterialMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UEntityMaterialMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// // 获取 点数据
// FTimelineEnterPointInfo* UEntityMaterialMainUserWidget::GetTimelinePointInfo()
// {
// 	if (panel == nullptr)return nullptr;
//
// 	return panel->GetTimelinePointInfo(pointIndexInfo.entityId, pointIndexInfo.pointInfoListIndex);
// }

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UEntityMaterialMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	panelUserWidget = Cast<UEntityMaterialPanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_EntityMaterial_Panel")));

}

//
void UEntityMaterialMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation)
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