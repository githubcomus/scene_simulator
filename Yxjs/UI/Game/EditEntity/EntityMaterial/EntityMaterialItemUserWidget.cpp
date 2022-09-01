// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityMaterialItemUserWidget.h"
#include "EntityMaterialPanelUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EntityMaterialPanelUserWidget.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>
#include <Components/CanvasPanelSlot.h>

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
void UEntityMaterialItemUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityMaterialItemUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityMaterialItemUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEntityMaterialItemUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
	{
		SetSelect(true, true);
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UEntityMaterialItemUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
	{
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UEntityMaterialItemUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (Image_background.IsValid())
	{
		Image_background->SetColorAndOpacity(FLinearColor(1.0, 0.7, 0.0, 1.0));
	}
	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI_确认);
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEntityMaterialItemUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (Image_background.IsValid())
	{
		Image_background->SetColorAndOpacity(FLinearColor(0.378744, 0.427988, 1.0, 0.3));
	}
}

//
void UEntityMaterialItemUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	Image_icon = Cast<UImage>(GetWidgetFromName(TEXT("Image_icon")));
	Image_select = Cast<UImage>(GetWidgetFromName(TEXT("Image_select")));
	Image_background = Cast<UImage>(GetWidgetFromName(TEXT("Image_background")));
	TextBlock_name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_name")));

}

//
void UEntityMaterialItemUserWidget::LoadImage(bool start, TWeakObjectPtr<UObject> component, FString path)
{
	if (component.IsValid() == false)return;
	if (this->IsValidLowLevel() == false)return;// 回调时actor被销毁则不继续执行

	if (start)
	{
		if (handleMap.Find(component.Get()))
		{
			handleMap.Remove(component.Get());
		}
		TSharedPtr<FStreamableHandle> handle;
		handleMap.Add(component.Get(), handle);

		FSoftObjectPath softObjectPath;
		softObjectPath.SetPath(path);
		handleMap[component.Get()] = UAssetManager::GetStreamableManager().RequestAsyncLoad(softObjectPath,
			FStreamableDelegate::CreateUObject(
				this, &UEntityMaterialItemUserWidget::LoadImage,
				false, component, path),
			FStreamableManager::AsyncLoadHighPriority,
			false);
	}
	else
	{
		if (handleMap.Find(component.Get()) == nullptr)
		{
			return;
		}

		bool flag = false;
		auto& handle_ = handleMap[component.Get()];
		if (handle_ && handle_.IsValid() && component.IsValid())
		{
			if (handle_->IsActive() && handle_->HasLoadCompleted())
			{
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
void UEntityMaterialItemUserWidget::SetImage(UTexture2D* texture2D, UImage* image)
{

	if (texture2D && image)
	{
		//auto brushX = image->Brush.ImageSize.X;
		//auto brushY = image->Brush.ImageSize.Y;

		image->SetBrushFromTexture(texture2D);
		//image->SetOpacity(1);

		auto imageX = texture2D->GetSizeX();
		auto imageY = texture2D->GetSizeY();

		UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(image->Slot);
		if (canvasPanelSlot)
		{
			//FMargin margin;
			//margin.Left = -(imageX / 2);
			//margin.Top = -(imageY / 2);
			//margin.Right = imageX;
			//margin.Bottom = imageY;
			//canvasPanelSlot->LayoutData.Offsets = margin;
			//auto temp = canvasPanelSlot->LayoutData;
			//temp.Offsets = margin;
			//canvasPanelSlot->SetLayout(temp);

			//auto size = canvasPanelSlot->GetSize();
			//image->SetBrushSize(size);
		}

		//image->SetBrushSize(FVector2D(90, 90));

		//image->RenderTransform.Scale.X = brushX;
		//image->RenderTransform.Scale.Y = brushY;

	}
}

//
void UEntityMaterialItemUserWidget::Start()
{
	LoadImage(true, Image_icon, iconPath);
	Image_select->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_name->SetText(FText::FromString(*name));
}

// 点亮边框
void UEntityMaterialItemUserWidget::SetSelect(bool call, bool isOld)
{
	if (call)
	{
		owner->SetSelect(itemIndex);
	}
	else
	{
		if (isOld)
		{
			Image_select->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			Image_select->SetVisibility(ESlateVisibility::Visible);
		}
	}
}