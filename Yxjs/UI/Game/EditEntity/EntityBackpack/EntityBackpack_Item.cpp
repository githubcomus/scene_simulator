// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityBackpack_Item.h"
#include "EntityBackpackPanelUserWidget.h"
#include "Core/BaseGameInstance.h"
#include "Core/SoundEffect/MySoundEffect.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>
#include <Components/CanvasPanelSlot.h>

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
void UEntityBackpack_Item::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityBackpack_Item::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityBackpack_Item::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEntityBackpack_Item::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
	{

	}
	FEventReply val = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return val.NativeReply;
}

//
FReply UEntityBackpack_Item::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	return FReply::Handled();
}

//
void UEntityBackpack_Item::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (Image_select.IsValid())
	{
		Image_select->SetColorAndOpacity(FLinearColor(1.0, 0.7, 0.0, 1.0));
	}

	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI_??????);
}

//
void UEntityBackpack_Item::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (Image_select.IsValid())
	{
		Image_select->SetColorAndOpacity(FLinearColor(0.378744, 0.427988, 1.0, 0.3));
	}
}

// ??????UI ????????????
void UEntityBackpack_Item::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [NativeOnDragDetected] ????????????  "), this);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("????????????"));


	//1. ??????????????????
	ABaseCharacter* character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	character->selectComponent->SetSelectHidden();

	//2. ????????????UI
	UEntityBackpack_Item* child = Cast<UEntityBackpack_Item>(
		CreateWidget(GetWorld(), owner->class_BP_UI_EntityBackpack_Item));
	UTexture2D* texture2D = Cast<UTexture2D>(Image_icon->Brush.GetResourceObject());
	child->StartDrag(name, texture2D);

	//3. ????????????
	UDragDropOperation* DragOperation = Cast<UDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(class_BP_UI_EntityStorage_Item_drag));
	DragOperation->DefaultDragVisual = child;
	DragOperation->Pivot = EDragPivot::MouseDown;
	DragOperation->Payload = this;
	OutOperation = DragOperation;

	//4. ??????drop??????
	owner->BP_UI_Bacackpack_Drop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

// ??????UI ??????????????????
bool UEntityBackpack_Item::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

// ???????????????UI ???????????????UI??????????????????
bool UEntityBackpack_Item::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [NativeOnDragOver] ???????????????UI???  "), this);
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

// ??????UI ????????????
void UEntityBackpack_Item::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [NativeOnDragCancelled] ????????????  "), this);
	owner->BP_UI_Bacackpack_Drop->SetVisibility(ESlateVisibility::Hidden);
}

//
void UEntityBackpack_Item::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	FString bpPath = TEXT(
		"Blueprint'/Game/Yxjs/UI/EditEntity/EntityBackpack/BP_UI_EntityBackpack_Item_Drop.BP_UI_EntityBackpack_Item_Drop_C'");
	class_BP_UI_EntityStorage_Item_drag = LoadClass<UDragDropOperation>(this, *bpPath);
	if (class_BP_UI_EntityStorage_Item_drag == nullptr)
	{
		return;
	}

	Image_icon = Cast<UImage>(GetWidgetFromName(TEXT("Image_icon")));
	Image_select = Cast<UImage>(GetWidgetFromName(TEXT("Image_select")));
	TextBlock_name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_name")));

}

//
void UEntityBackpack_Item::LoadImage(bool start, TWeakObjectPtr<UObject> component, FString path)
{
	if (component.IsValid() == false)return;
	if (this->IsValidLowLevel() == false)return;// ?????????actor???????????????????????????

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
				this, &UEntityBackpack_Item::LoadImage,
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
void UEntityBackpack_Item::SetImage(UTexture2D* texture2D, UImage* image)
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
void UEntityBackpack_Item::Start()
{
	LoadImage(true, Image_icon, iconPath);
	//Image_select->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_name->SetText(FText::FromString(*name));
}

// ???????????????UI?????????
void UEntityBackpack_Item::StartDrag(FString str, UTexture2D* texture2D)
{
	Init();
	TextBlock_name->SetText(FText::FromString(*str));
	SetImage(texture2D, Image_icon.Get());
}

// ????????????
void UEntityBackpack_Item::SetSelect(bool call, bool isOld)
{
	//if (call) {
	//	owner->SetSelect(itemIndex);
	//}
	//else {
	//	if (isOld) {
	//		Image_select->SetVisibility(ESlateVisibility::Hidden);
	//	}
	//	else {
	//		Image_select->SetVisibility(ESlateVisibility::Visible);
	//	}
	//}
}