// Fill out your copyright notice in the Description page of Project Settings.

#include "BP_UI_Timeline_node_line_data.h"
#include "Components/ScrollBox.h"
#include "Components/Overlay.h"

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// controller
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UBP_UI_Timeline_node_line_data::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UBP_UI_Timeline_node_line_data::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UBP_UI_Timeline_node_line_data::NativeConstruct()
{
	Super::NativeConstruct();
	overlay = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_64")));

	// UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_line_data::NativeConstruct] [scrollBox:%x]"), this, scrollBox.Get());

	Init();
}

//
void UBP_UI_Timeline_node_line_data::NativeDestruct()
{
	Super::NativeDestruct();
}

/*------------------------------------------------------------------*/

// 初始化
void UBP_UI_Timeline_node_line_data::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;
}
