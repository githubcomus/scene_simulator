// Fill out your copyright notice in the Description page of Project Settings.

#include "Tips_Main.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Components/Overlay.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Components/CanvasPanelSlot.h>
#include "Tips_Message.h"

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
void UTips_Main::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UTips_Main::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTips_Main::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UTips_Main::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UTips_Main::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UTips_Main::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UTips_Main::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UTips_Main::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
void UTips_Main::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	{
		FString bpPath = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Permanent/Tips/BP_UI_Tips_Message.BP_UI_Tips_Message_C'");
		class_BP_UI_Tips_Message = LoadClass<UTips_Message>(this, *bpPath);
		if (class_BP_UI_Tips_Message == nullptr)
		{
			return;
		}
	}

	Overlay_show = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_show")));
	if (Overlay_show.IsValid())
	{
		UCanvasPanelSlot* canvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Overlay_show.Get());
		if (canvasPanelSlot)
		{
			canvasPanelSlot->SetZOrder(20);
		}
	}

}

//
void UTips_Main::OnExpandCollapse(bool isShow, bool useAnimation)
{

	if (isShow)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}

	//if (auto Event_ChangeChildUI = FindFunction("Event_ChangeChildUI")) {

	//	struct FUIInfo_ChangeChildUIBroadcast {
	//		bool isShow;
	//		bool useAnimation = false;
	//	};
	//	FUIInfo_ChangeChildUIBroadcast info;
	//	info.isShow = isShow;
	//	info.useAnimation = useAnimation;
	//	ProcessEvent(Event_ChangeChildUI, &info);
	//};
	//panelUserWidget->OnExpandCollapse(isShow);
}

// 发送一条消息
void UTips_Main::PushMessage(FString str)
{

	UE_LOG(LogTemp, Log, TEXT("[%x] [推送消息] [PushMessage] [%s]"), this, *str);

	if (class_BP_UI_Tips_Message == nullptr)
	{
		return;
	}
	UTips_Message* pItem = Cast<UTips_Message>(
		CreateWidget(GetWorld(), class_BP_UI_Tips_Message));
	if (pItem == nullptr)
	{
		return;
	}
	Overlay_show->AddChild(pItem);
	pItem->Start(this, str);

	int jobId = tempJobId++;
	FMyMessageInfo myMessageInfo;
	myMessageInfo.ui = pItem;
	messsageMap.Add(jobId, myMessageInfo);
	Message_DelayRun(true, jobId);
}

// 外部消息管理
void UTips_Main::Message_DelayRun(bool isStart, int jobId)
{
	float relayTime = 2.0;
	auto it = messsageMap.Find(jobId);
	if (it == nullptr)return;

	// 预备执行
	if (isStart)
	{
		if (it->timerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(it->timerHandle);
		}
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &UTips_Main::Message_DelayRun, false, jobId);
		GetWorld()->GetTimerManager().SetTimer(it->timerHandle, RespawnDelegate, relayTime, false);
	}
	// 真实执行
	else
	{
		if (it->timerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(it->timerHandle);
		}
		UTips_Message* ui = it->ui.Get();
		ui->End();
		messsageMap.Remove(jobId);
		Overlay_show->RemoveChild(ui);
	}
}