// Fill out your copyright notice in the Description page of Project Settings.

#include "InputHelp_Main.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "InputHelp_Panel.h"

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UInputHelp_Main::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UInputHelp_Main::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UInputHelp_Main::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UInputHelp_Main::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UInputHelp_Main::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UInputHelp_Main::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UInputHelp_Main::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UInputHelp_Main::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
void UInputHelp_Main::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;


	panelUserWidget = Cast<UInputHelp_Panel>(GetWidgetFromName(TEXT("BP_UI_InputHelp_Panel")));

}

//
void UInputHelp_Main::OnExpandCollapse(bool isShow, bool useAnimation)
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

