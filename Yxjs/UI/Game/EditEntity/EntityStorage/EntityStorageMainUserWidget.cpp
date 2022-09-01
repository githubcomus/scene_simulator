// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityStorageMainUserWidget.h"
#include "EntityStoragePanelUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EntityStorageItemUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UEntityStorageMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityStorageMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityStorageMainUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEntityStorageMainUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UEntityStorageMainUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UEntityStorageMainUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEntityStorageMainUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UEntityStorageMainUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnMouseMove] 鼠标移动 [%f] "), this, GetWorld()->TimeSeconds);
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UEntityStorageMainUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;


	panelUserWidget = Cast<UEntityStoragePanelUserWidget>(GetWidgetFromName(TEXT("BP_UI_EntityStorage_Panel")));

}

//
void UEntityStorageMainUserWidget::OnExpandCollapse(bool isShow, bool useAnimation)
{

	if (auto Event_ChangeChildUI = FindFunction("Event_ChangeChildUI"))
	{

		struct FUIInfo_ChangeChildUIBroadcast
		{
			bool isShow;
			bool useAnimation = false;
		};
		FUIInfo_ChangeChildUIBroadcast info;
		info.isShow = isShow;
		info.useAnimation = useAnimation;
		ProcessEvent(Event_ChangeChildUI, &info);
	};

	panelUserWidget->OnExpandCollapse(isShow);

}

