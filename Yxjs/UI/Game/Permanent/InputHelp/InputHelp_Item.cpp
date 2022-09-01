// Fill out your copyright notice in the Description page of Project Settings.

#include "InputHelp_Item.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>
#include <Components/EditableText.h>
#include <Components/Overlay.h>
#include "../../../../GameMode/BaseGameMode.h"
#include "../../../../FunctionalModule/Selected/SelectedViewActor.h"
#include "../../../../FunctionalModule/Selected/SelectedControllerComponent.h"
#include "../../../../Pawn/Character/BaseCharacter.h"
#include "../../../../Core/GameInstanceStaticFunc.h"
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
void UInputHelp_Item::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UInputHelp_Item::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UInputHelp_Item::NativeDestruct()
{
	Super::NativeDestruct();


}

//
void UInputHelp_Item::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (Image_background.IsValid())
	{
		Image_background->SetColorAndOpacity(FLinearColor(0.621257, 0.630753, 1.0, 0.5));
	}
	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI_确认);
}

//
void UInputHelp_Item::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (Image_background.IsValid())
	{
		Image_background->SetColorAndOpacity(FLinearColor(0.052083, 0.052083, 0.052083, 0.5));
	}
}

//
void UInputHelp_Item::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	Image_background = Cast<UImage>(GetWidgetFromName(TEXT("Image_background")));
	TextBlock_str = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_str")));

}

//
void UInputHelp_Item::Start(TWeakObjectPtr<UInputHelp_Panel> owner_, FString str)
{
	owner = owner_;
	if (TextBlock_str.IsValid())
	{
		TextBlock_str->SetText(FText::FromString(*str));
	}
}
