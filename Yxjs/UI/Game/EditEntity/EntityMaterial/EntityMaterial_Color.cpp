// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityMaterial_Color.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
//#include "TimelinePanelUserWidget.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include <Components/MenuAnchor.h>
#include <Blueprint/SlateBlueprintLibrary.h>
#include "Yxjs/UI/BaseHUD.h"
//#include "TimelineMainUserWidget.h"
//#include "TimelinePanelUserWidget.h"
#include <Components/Border.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include "../../../../FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include <Components/Button.h>
#include <Components/CheckBox.h>
#include "Yxjs/UI/HUDStaticFunc.h"
#include "Yxjs/Core/GameInstanceStaticFunc.h"

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
void UEntityMaterial_Color::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityMaterial_Color::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityMaterial_Color::NativeDestruct()
{
	Super::NativeDestruct();

}

//
FReply UEntityMaterial_Color::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
FReply UEntityMaterial_Color::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

// 鼠标 移动
FReply UEntityMaterial_Color::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnMouseMove(true, InGeometry, InMouseEvent);

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

// 鼠标 NativeOnMouseMove()->移动
void UEntityMaterial_Color::OnMouseMove(bool isPanel, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	// 鼠标左键按下
	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(
		InMouseEvent, EKeys::LeftMouseButton))
	{
		if (Sign.IsValid())
		{
			auto tempVar = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
			UCanvasPanelSlot* canvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Sign.Get());
			if (canvasPanelSlot)
			{
				if ((tempVar - FVector2D(200, 200)).Size() <= 200)
				{
					canvasPanelSlot->SetPosition(tempVar);
					SetBoxImageColor();
				}
			}
		}
	}
}

//
void UEntityMaterial_Color::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	CheckBox_ = Cast<UCheckBox>(GetWidgetFromName(TEXT("CheckBox_")));
	TextBlock_str = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_str")));
	EditableText_r = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_r")));
	EditableText_g = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_g")));
	EditableText_b = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_b")));

	Sign = Cast<UImage>(GetWidgetFromName(TEXT("Sign")));
	Border_ = Cast<UBorder>(GetWidgetFromName(TEXT("Border_")));

	Button_application = Cast<UButton>(GetWidgetFromName(TEXT("Button_application")));
	if (Button_application.IsValid())
	{


		FScriptDelegate func;
		func.BindUFunction(this, TEXT("MyButton_application"));
		Button_application->OnClicked.AddUnique(func);
	}
}

// 
void UEntityMaterial_Color::Reset(TWeakObjectPtr<UEntityMaterialPanelUserWidget> owner, TWeakObjectPtr<AEditEntityActor>  entityActor)
{

	owner_ = owner;

	bool highlight = entityActor->syncInfo.mutable_child_list(0)->mutable_mesh()->highlight();
	Gamedata::FVector color = entityActor->syncInfo.mutable_child_list(0)->mutable_mesh()->color();

	SetUIData(highlight, FVector(color.x(), color.y(), color.z()));
}

// 设置UI中的颜色
void UEntityMaterial_Color::SetBoxImageColor()
{

	FVector color = GetColorValue();
	bool highlight = CheckBox_->IsChecked();

	SetUIData(highlight, color);
}

// 设置UI数据
void UEntityMaterial_Color::SetUIData(bool highlight, FVector color)
{

	EditableText_r->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), color.X)));
	EditableText_g->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), color.Y)));
	EditableText_b->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), color.Z)));

	Border_->SetBrushColor(FLinearColor(color.X, color.Y, color.Z, 1));

	CheckBox_->SetCheckedState(highlight ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	oldColor = color;
}

// 点击应用
void UEntityMaterial_Color::MyButton_application()
{

	//1. 检测指针
	if (CheckBox_.IsValid() == false)return;
	if (EditableText_r.IsValid() == false)return;
	if (EditableText_g.IsValid() == false)return;
	if (EditableText_b.IsValid() == false)return;

	bool highlight = CheckBox_->IsChecked();

	//2. 获取数据
	std::string r(TCHAR_TO_UTF8(*EditableText_r->GetText().ToString()));
	std::string g(TCHAR_TO_UTF8(*EditableText_g->GetText().ToString()));
	std::string b(TCHAR_TO_UTF8(*EditableText_b->GetText().ToString()));

	//3. 检测输入非法字符串
	{
		bool flag1 = UCalculationObject::StrIsNumber(r);
		bool flag2 = UCalculationObject::StrIsNumber(g);
		bool flag3 = UCalculationObject::StrIsNumber(b);
		if (
			flag1 == false ||
			flag2 == false ||
			flag3 == false
			)
		{
			UHUDStaticFunc::PushMessage(this, TEXT("输入的字符为非法数字"));
			UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI叮咚音效);

			SetUIData(highlight, oldColor);
			return;
		}
	}

	//4. 转换成存储值
	FVector color(atof(r.c_str()), atof(g.c_str()), atof(b.c_str()));

	//5. 检测数字范围
	{
		if (
			(0 <= color.X && color.X <= 1)
			&& (0 <= color.Y && color.Y <= 1)
			&& (0 <= color.Z && color.Z <= 1)
			)
		{
		}
		else
		{
			UHUDStaticFunc::PushMessage(this, TEXT("输入超出范围"));
			UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI叮咚音效);
			SetUIData(highlight, oldColor);
			return;
		}
	}

	//6. 广播
	owner_->SetChangeColor(color, highlight);

	//7. 反向重新设置UI,防止是从text输入的.
	SetUIData(highlight, color);
}