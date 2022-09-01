// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityStorageItemUserWidget.h"
#include "EntityStoragePanelUserWidget.h"
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

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UEntityStorageItemUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityStorageItemUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityStorageItemUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEntityStorageItemUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	if (owner->IsChangeName())
	{
		if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnMouseButtonDown] 鼠标左键按下  "), this);
			SetSelect(true, true);
		}
		return FReply::Handled();
	}
	else
	{
		auto val = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return val.NativeReply;
	}
}

//
FReply UEntityStorageItemUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
void UEntityStorageItemUserWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDragDetected] 开始拖拽  "), this);

	//1. 将坐标轴清除
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	character->selectComponent->SetSelectHidden();

	//2. 临时UI
	UEntityStorageItemUserWidget* child = Cast<UEntityStorageItemUserWidget>(
		CreateWidget(GetWorld(), owner->class_BP_UI_EntityStorage_Item));
	child->StartDrag(name);

	//3. 拖拽UI
	UDragDropOperation* DragOperation = Cast<UDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(class_BP_UI_EntityStorage_Item_drag));
	DragOperation->DefaultDragVisual = child;
	DragOperation->Pivot = EDragPivot::MouseDown;
	DragOperation->Payload = this;
	OutOperation = DragOperation;

	//4. 拖拽窗口工作
	owner->BP_UI_EntityStorage_Drop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

//
bool UEntityStorageItemUserWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	//UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDragOver] 结束拖拽  "), this);

	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

//
void UEntityStorageItemUserWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDragCancelled] 结束拖拽  "), this);
	owner->BP_UI_EntityStorage_Drop->SetVisibility(ESlateVisibility::Hidden);
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

//
void UEntityStorageItemUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (Image_background.IsValid())
	{
		Image_background->SetColorAndOpacity(FLinearColor(0.055016, 0.095039, 0.151042, 0.5));
	}
	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI_确认);
}

//
void UEntityStorageItemUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (Image_background.IsValid())
	{
		Image_background->SetColorAndOpacity(FLinearColor(0.130208, 0.130208, 0.130208, 1.000000));
	}
}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UEntityStorageItemUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;
	FString bpPath = TEXT(
		"WidgetBlueprint'/Game/Yxjs/UI/EditEntity/EntityStorage/BP_UI_EntityStorage_Item_drag.BP_UI_EntityStorage_Item_drag_C'");
	class_BP_UI_EntityStorage_Item_drag = LoadClass<UDragDropOperation>(this, *bpPath);
	if (class_BP_UI_EntityStorage_Item_drag == nullptr)
	{
		return;
	}

	Image_background = Cast<UImage>(GetWidgetFromName(TEXT("Image_background")));
	EditableText_str = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_str")));
	Overlay_edit = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_edit")));

	if (EditableText_str.IsValid())
	{
		FScriptDelegate func;
		func.BindUFunction(this, TEXT("MyTextCommitted"));
		EditableText_str->OnTextCommitted.AddUnique(func);
	}
}

//
void UEntityStorageItemUserWidget::Start()
{
	if (EditableText_str.IsValid())
	{
		EditableText_str->SetText(FText::FromString(*name));
	}
}

// 拖拽初始化
void UEntityStorageItemUserWidget::StartDrag(FString str)
{
	Init();
	if (EditableText_str.IsValid())
	{
		EditableText_str->SetText(FText::FromString(*str));
	}
}

// 点亮边框
void UEntityStorageItemUserWidget::SetSelect(bool call, bool isOld)
{
	if (call)
	{
		owner->SetSelect(unique_mark);
	}
	else
	{
		// 不可编辑
		if (isOld)
		{
			Overlay_edit->SetVisibility(ESlateVisibility::Visible);
			Image_background->SetColorAndOpacity(FLinearColor(0.130208, 0.130208, 0.130208, 1.000000));
		}
		// 可以编辑
		else
		{
			Overlay_edit->SetVisibility(ESlateVisibility::Hidden);
			Image_background->SetColorAndOpacity(FLinearColor(0.036458, 0.036458, 0.036458, 1.000000));
			EditableText_str->SetFocus();
		}
	}
}

// 文本提交时
void UEntityStorageItemUserWidget::MyTextCommitted(const FText& Text, ETextCommit::Type  CommitMethod)
{

	std::string newStr = TCHAR_TO_UTF8(*Text.ToString());
	std::string oldStr = entitySyncInfo.entity_name();
	if (newStr != oldStr)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [MyTextCommitted] [%s]"), this, *Text.ToString());
		entitySyncInfo.set_entity_name(newStr);

		owner->changeCount++;
	}
}