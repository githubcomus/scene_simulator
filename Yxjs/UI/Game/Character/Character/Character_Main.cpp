// Fill out your copyright notice in the Description page of Project Settings.

#include "Character_Main.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Character_Panel.h"

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
void UCharacter_Main::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UCharacter_Main::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UCharacter_Main::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UCharacter_Main::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UCharacter_Main::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UCharacter_Main::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UCharacter_Main::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UCharacter_Main::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
void UCharacter_Main::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	panelUserWidget = Cast<UCharacter_Panel>(GetWidgetFromName(TEXT("BP_UI_Character_Panel")));

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UCharacter_Main::Init] "), this);
}

//
void UCharacter_Main::OnExpandCollapse(bool isShow, bool useAnimation) {

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