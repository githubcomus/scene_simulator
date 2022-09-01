// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyLevelMainUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "SkyLevelPanelUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void USkyLevelMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void USkyLevelMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void USkyLevelMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply USkyLevelMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply USkyLevelMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void USkyLevelMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void USkyLevelMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply USkyLevelMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
void USkyLevelMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	panelUserWidget = Cast<USkyLevelPanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_SkyLevel_Panel")));

	//UE_LOG(LogTemp, Log, TEXT("[%x] [USkyLevelMainUserWidget::Init] "), this);
}

//
void USkyLevelMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation) {

	if (auto Event_ChangeChildUI = FindFunction("Event_ChangeChildUI")) {

		struct FUIInfo_ChangeChildUIBroadcast {
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