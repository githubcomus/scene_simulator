﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyLevelItemUserWidget.h"
#include "SkyLevelPanelUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>

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
void USkyLevelItemUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void USkyLevelItemUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void USkyLevelItemUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply USkyLevelItemUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
	{
		SetSelect(true, true);
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply USkyLevelItemUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void USkyLevelItemUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void USkyLevelItemUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
void USkyLevelItemUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	Image_icon = Cast<UImage>(GetWidgetFromName(TEXT("Image_icon")));
	Image_select = Cast<UImage>(GetWidgetFromName(TEXT("Image_select")));
	TextBlock_name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_name")));
	Button_ = Cast<UButton>(GetWidgetFromName(TEXT("Button_")));

	if (Button_.Get()) {
		FScriptDelegate func;
		func.BindUFunction(this, TEXT("MyClickedEvent"));
		Button_->OnClicked.AddUnique(func);
	}
}

//
void USkyLevelItemUserWidget::LoadImage(bool start, TWeakObjectPtr<UObject> component, FString path)
{
	if (component.IsValid() == false)return;
	if (this->IsValidLowLevel() == false)return;// 回调时actor被销毁则不继续执行

	if (start)
	{
		if (handleMap.Find(component.Get())) {
			handleMap.Remove(component.Get());
		}
		TSharedPtr<FStreamableHandle> handle;
		handleMap.Add(component.Get(), handle);

		FSoftObjectPath softObjectPath;
		softObjectPath.SetPath(path);
		handleMap[component.Get()] = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObjectPath,
			FStreamableDelegate::CreateUObject(
				this, &USkyLevelItemUserWidget::LoadImage,
				false, component, path),
			FStreamableManager::AsyncLoadHighPriority,
			false);
	}
	else
	{
		if (handleMap.Find(component.Get()) == nullptr) {
			return;
		}

		bool flag = false;
		auto& handle_ = handleMap[component.Get()];
		if (handle_ && handle_.IsValid() && component.IsValid()) {
			if (handle_->IsActive() && handle_->HasLoadCompleted()) {
				//----------------------------------------------------//
				UTexture2D* texture2D = Cast<UTexture2D>(handle_->GetLoadedAsset());
				UImage* image = Cast<UImage>(component.Get());
				SetImage(texture2D, image);
				//----------------------------------------------------//
			}
		}
		handleMap.Remove(component.Get());
	}
}

//
void USkyLevelItemUserWidget::SetImage(UTexture2D* texture2D, UImage* image) {

	if (texture2D && image)
	{
		image->SetBrushFromTexture(texture2D);
	}
}

//
void USkyLevelItemUserWidget::Start()
{
	LoadImage(true, Image_icon, iconPath);
	Image_select->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_name->SetText(FText::FromString(*name));
}

// 按钮点击事件
void USkyLevelItemUserWidget::MyClickedEvent() {
	SetSelect(true, false);
}

// 选中
void USkyLevelItemUserWidget::SetSelect(bool call, bool isOld) {
	if (call) {
		owner->SetSelect(itemIndex);
	}
	else {
		if (isOld) {
			Image_select->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			Image_select->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
