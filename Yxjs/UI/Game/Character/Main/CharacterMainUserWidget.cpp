// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterMainUserWidget.h"
#include "ShortcutBar/ShortcutBarPanelUserWidget.h"
#include "Pawn/Character/CharacterComponent/PlayerEditEntityComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Yxjs/UI/BaseHUD.h"
#include "Yxjs/Pawn/Character/BaseCharacter.h"

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
void UCharacterMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UCharacterMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UCharacterMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	shortcutBar = Cast<UShortcutBarPanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_ShortcutBar_Panel")));

}

//
void UCharacterMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UCharacterMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UCharacterMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UCharacterMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UCharacterMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UCharacterMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
FReply UCharacterMainUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftShift) {
	}
	return FReply::Handled();
}

//
FReply UCharacterMainUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	if (InKeyEvent.GetKey() == EKeys::LeftShift) {
		// 方案1
		hud->SetUIState(false);
	}
	return FReply::Handled();
}

//
void UCharacterMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation) {


	// 粗暴无动画模式
	if (isShow) {
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		SetVisibility(ESlateVisibility::Hidden);
	}

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

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	if (isShow) {
		//character->myController->Call_EnterStaus(true);
		character->selectComponent->SetSelectHidden();
	}
	else {
		//character->myController->Call_EnterStaus(false);
	}
	if (shortcutBar.IsValid()) {
		shortcutBar->OnExpandCollapse(isShow, this);
	}

}