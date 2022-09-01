// Fill out your copyright notice in the Description page of Project Settings.


#include "TimelineColorUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TimelinePanelUserWidget.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
#include <Components/MenuAnchor.h>
#include <Blueprint/SlateBlueprintLibrary.h>
#include "Yxjs/UI/BaseHUD.h"
#include "TimelineMainUserWidget.h"
#include "TimelinePanelUserWidget.h"
#include <Components/Border.h>
#include <Blueprint/WidgetLayoutLibrary.h>

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// view
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTimelineColorUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UTimelineColorUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTimelineColorUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// data
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// controller
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTimelineColorUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	// textBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_70")));

	Sign = Cast<UImage>(GetWidgetFromName(TEXT("Sign")));
	Border_91 = Cast<UBorder>(GetWidgetFromName(TEXT("Border_91")));


	auto hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud == nullptr)return;
	auto timelinePanel = Cast<UTimelineMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_timeline))->timelinePanel;
	if (timelinePanel == nullptr)return;

	FVector color = timelinePanel->GetEditColor();
	Border_91->SetBrushColor(FLinearColor(color.X, color.Y, color.Z, 1));
}

// 鼠标 移动
FReply UTimelineColorUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnMouseMove(true, InGeometry, InMouseEvent);

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

// 鼠标 NativeOnMouseMove()->移动
void UTimelineColorUserWidget::OnMouseMove(bool isPanel, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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
				if (tempVar.X < 0)tempVar.X = 0;
				else if (tempVar.X > 400)tempVar.X = 400;
				if (tempVar.Y < 0)tempVar.Y = 0;
				else if (tempVar.Y > 400)tempVar.Y = 400;
				canvasPanelSlot->SetPosition(tempVar);
			}
		}


	}
}

// 按钮点击应用
void UTimelineColorUserWidget::OnEnterColor(FVector color)
{
	auto hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud == nullptr)return;
	auto timelinePanel = Cast<UTimelineMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_timeline))->timelinePanel;
	if (timelinePanel == nullptr)return;
	if (timelinePanel->CanSave())
	{
		timelinePanel->OnEnterColor(color);
	}
	else
	{

	}


}

// 测试坐标系
void UTimelineColorUserWidget::Test(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	{
		FGeometry gemotry = GetPaintSpaceGeometry();
		FVector2D mousePosion;
		auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerController->GetMousePosition(mousePosion.X, mousePosion.Y);
		mousePosion = mousePosion / gemotry.Scale;
		UE_LOG(LogTemp, Log, TEXT("[%x] [OnMouseMove] 子类左键按下A [time:%f] [mousePosion:%f %f]"), this,
			GetWorld()->TimeSeconds, mousePosion.X, mousePosion.Y);
	}

	{
		FVector2D LocalWidgetMousePos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, InMouseEvent.GetScreenSpacePosition());
		FVector2D LocalWidgetMousePos2 = USlateBlueprintLibrary::LocalToAbsolute(InGeometry, FVector2D(0, 0));

		UE_LOG(LogTemp, Log, TEXT("[%x] [OnMouseMove] 子类左键按下B [time:%f]   [tempVar:%s] [tempVar:%s] "), this,
			GetWorld()->TimeSeconds, *LocalWidgetMousePos.ToString(), *InMouseEvent.GetScreenSpacePosition().ToString());
	}


	{
		auto tempVar = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		UE_LOG(LogTemp, Log, TEXT("[%x] [OnMouseMove] 子类左键按下C [time:%f] [tempVar:%s]"), this,
			GetWorld()->TimeSeconds, *tempVar.ToString());
	}

	{
		FGeometry gemotry = GetPaintSpaceGeometry();
		auto tempVar1 = gemotry.GetAbsolutePosition();
		auto tempVar2 = InGeometry.GetAbsolutePosition();
		FVector2D LocalWidgetMousePos1 = USlateBlueprintLibrary::AbsoluteToLocal(gemotry, tempVar1);
		FVector2D LocalWidgetMousePos2 = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, tempVar2);
		UE_LOG(LogTemp, Log, TEXT("[%x] [OnMouseMove] 子类左键按下D [time:%f] [tempVar:%s] [tempVar:%s] [tempVar:%s] [tempVar:%s]"), this,
			GetWorld()->TimeSeconds, *tempVar1.ToString(), *tempVar2.ToString(), *LocalWidgetMousePos1.ToString(), *LocalWidgetMousePos2.ToString());
	}
}