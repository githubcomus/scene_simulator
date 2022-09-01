// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityBackpack_LeftItem.h"
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
#include "Components/Button.h"

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
void UEntityBackpack_LeftItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityBackpack_LeftItem::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityBackpack_LeftItem::NativeDestruct()
{
	Super::NativeDestruct();
}

//
FReply UEntityBackpack_LeftItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
FReply UEntityBackpack_LeftItem::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
void UEntityBackpack_LeftItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (Image_background.IsValid())
	{
		Image_background->SetColorAndOpacity(FLinearColor(0.09375, 0.09375, 0.09375, 0.7));
	}
	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI_确认);
}

//
void UEntityBackpack_LeftItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (Image_background.IsValid())
	{
		Image_background->SetColorAndOpacity(FLinearColor(0.621257, 0.630753, 1.0, 0.3));
	}
}

//
void UEntityBackpack_LeftItem::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	Image_background = Cast<UImage>(GetWidgetFromName(TEXT("Image_background")));
	TextBlock_str = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_str")));
	Button_tag = Cast<UButton>(GetWidgetFromName(TEXT("Button_tag")));

	if (Button_tag.IsValid())
	{
		FScriptDelegate func;
		func.BindUFunction(this, TEXT("MyClicked"));
		Button_tag->OnClicked.AddUnique(func);
	}
}

//
void UEntityBackpack_LeftItem::Start()
{
	if (TextBlock_str.IsValid())
	{
		TextBlock_str->SetText(FText::FromString(*name));
	}
}

// 点亮边框
void UEntityBackpack_LeftItem::SetSelect(bool in)
{
	if (in)
	{
		//Image_background
	}
	else
	{

	}
}

//
void UEntityBackpack_LeftItem::MyClicked()
{
	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_叮当声);
	owner->OpenThirdType(xlsInfo->SecondMenuType_);
}