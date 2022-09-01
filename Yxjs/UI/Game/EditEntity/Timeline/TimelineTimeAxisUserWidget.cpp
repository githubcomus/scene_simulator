// Fill out your copyright notice in the Description page of Project Settings.


#include "TimelineTimeAxisUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TimelinePanelUserWidget.h"

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// view
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UTimelineTimeAxisUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UTimelineTimeAxisUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UTimelineTimeAxisUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UTimelineTimeAxisUserWidget::NativeDestruct()
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
void UTimelineTimeAxisUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;


	image1 = Cast<UImage>(GetWidgetFromName(TEXT("Image_1")));
	image2 = Cast<UImage>(GetWidgetFromName(TEXT("Image_2")));
	image3 = Cast<UImage>(GetWidgetFromName(TEXT("Image_3")));

}

//
void UTimelineTimeAxisUserWidget::SetStr(FString str)
{
}

// 
void UTimelineTimeAxisUserWidget::SetSelect(bool isSelect_)
{
}

//
void UTimelineTimeAxisUserWidget::OnMouseEnterLeave(bool enter)
{
	if (mouseEnter == enter)return;
	mouseEnter = enter;
	if (enter)
	{
		FLinearColor inColorAndOpacity(0, 0, 0, 1);
		image1->SetColorAndOpacity(inColorAndOpacity);
		image2->SetColorAndOpacity(inColorAndOpacity);
		image3->SetColorAndOpacity(inColorAndOpacity);
	}
	else
	{
		FLinearColor inColorAndOpacity(255, 255, 255, 1);
		image1->SetColorAndOpacity(inColorAndOpacity);
		image2->SetColorAndOpacity(inColorAndOpacity);
		image3->SetColorAndOpacity(inColorAndOpacity);
	}
}