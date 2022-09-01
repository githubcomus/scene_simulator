// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityBackpackMainUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EntityBackpackPanelUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UEntityBackpackMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityBackpackMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityBackpackMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEntityBackpackMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UEntityBackpackMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UEntityBackpackMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEntityBackpackMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UEntityBackpackMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UEntityBackpackMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	panelUserWidget = Cast<UEntityBackpackPanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_EntityBackpack_Panel")));

}

//
void UEntityBackpackMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation)
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
