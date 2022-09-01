// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTimelineColorUserWidget.h"

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

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// view
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTestTimelineColorUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UTestTimelineColorUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTestTimelineColorUserWidget::NativeDestruct()
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
void UTestTimelineColorUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	// textBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_70")));

	//image = Cast<UImage>(GetWidgetFromName(TEXT("Image_backpack")));

}

// 鼠标 移动
FReply UTestTimelineColorUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnMouseMove(true, InGeometry, InMouseEvent);

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
bool UTestTimelineColorUserWidget::GetParentOffset(UWidget* ui, FVector2D& postion)
{
	bool flag = false;
	postion = FVector2D::ZeroVector;
	if (ui == nullptr)return flag;
	UCanvasPanelSlot* tempSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ui);
	if (tempSlot)
	{
		postion = tempSlot->GetPosition();
		flag = true;;
	}
	return flag;
}

//
FVector2D UTestTimelineColorUserWidget::GetUIOffset(UWidget* ui)
{
	FVector2D postion = FVector2D::ZeroVector;
	UWidget* tempUI = ui;

	FVector2D tempPostion;
	if (GetParentOffset(ui, tempPostion))
	{
		postion += tempPostion;
		UE_LOG(LogTemp, Log, TEXT("[%x] [GetUIOffset]  [name:%s] [tempUIParent:%s] "), this,
			*ui->GetName(), *tempPostion.ToString());
	}

	// 最多100蹭
	for (int i = 0; i < 100; i++)
	{
		auto tempUIParent = tempUI->GetParent();
		if (tempUIParent)
		{
			if (GetParentOffset(tempUIParent, tempPostion))
			{
				postion += tempPostion;
				tempUI = tempUIParent;
				UE_LOG(LogTemp, Log, TEXT("[%x] [GetUIOffset]  [name:%s] [tempUIParent:%s] "), this,
					*tempUIParent->GetName(), *tempPostion.ToString());
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return postion;
}

//
FVector2D UTestTimelineColorUserWidget::GetWidgetCenterLocation(UWidget* Widget)
{
	auto ParentWidget = Widget->GetParent();
	FGeometry Geometry = ParentWidget->GetCachedGeometry();
	FVector2D Position = Geometry.AbsoluteToLocal(Widget->GetCachedGeometry().GetAbsolutePosition()) + Widget->GetCachedGeometry().GetLocalSize() / 2.0f;
	return Position;
}

// 鼠标 NativeOnMouseMove()->移动
void UTestTimelineColorUserWidget::OnMouseMove(bool isPanel, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	// 鼠标左键按下
	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(
		InMouseEvent, EKeys::LeftMouseButton))
	{

		{
			FGeometry gemotry = GetPaintSpaceGeometry();
			float scale = gemotry.Scale;

			FVector2D mousePosion;
			auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			playerController->GetMousePosition(mousePosion.X, mousePosion.Y);
			mousePosion = mousePosion / scale;

			FVector2D ScreenPos;
			USlateBlueprintLibrary::ScreenToWidgetLocal(this, InGeometry, InMouseEvent.GetScreenSpacePosition(), ScreenPos);
			FVector2D LocalWidgetMousePos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, InMouseEvent.GetScreenSpacePosition());

			UE_LOG(LogTemp, Log, TEXT("[%x] [OnMouseMove] 父类左键按下 [time:%f] [mousePosion:%s] [GetScreenSpacePosition:%s] [scale:%f] [ScreenPos:%s]  [tempVar:%s] "), this,
				GetWorld()->TimeSeconds, *mousePosion.ToString(), *InMouseEvent.GetScreenSpacePosition().ToString(), scale, *ScreenPos.ToString(), *LocalWidgetMousePos.ToString());
		}

		{

			FGeometry gemotry = GetPaintSpaceGeometry();
			float scale = gemotry.Scale;
			auto MenuAnchor_0 = GetWidgetFromName(TEXT("MenuAnchor_0"));
			auto Image_357 = GetWidgetFromName(TEXT("Image_116"));
			if (MenuAnchor_0 && Image_357)
			{
				auto var1 = MenuAnchor_0->GetCachedGeometry().GetAbsolutePosition();
				auto var2 = Image_357->GetCachedGeometry().GetAbsolutePosition();
				auto var3 = (var2 / scale - var1 / scale);

				UE_LOG(LogTemp, Log, TEXT("[%x] [OnMouseMove] 父类左键按下 [image:%s] [MenuAnchor:%s] [c:%s] [d:%s]"), this,
					*var2.ToString(), *var1.ToString(), *var3.ToString(), *InMouseEvent.GetScreenSpacePosition().ToString());
			}
		}
	}
}
