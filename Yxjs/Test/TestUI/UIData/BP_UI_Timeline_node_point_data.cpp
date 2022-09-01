// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_UI_Timeline_node_point_data.h"
#include "Components/ScrollBox.h"

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// controller
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UBP_UI_Timeline_node_point_data::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

////
//void UBP_UI_Timeline_node_point_data::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UBP_UI_Timeline_node_point_data::NativeConstruct()
{
	Super::NativeConstruct();
	// scrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_116")));

	// UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_point_data::NativeConstruct] [scrollBox:%x]"), this, scrollBox.Get());

	Init();
}

//
void UBP_UI_Timeline_node_point_data::NativeDestruct()
{
	Super::NativeDestruct();
}

/*------------------------------------------------------------------*/

// 初始化
void UBP_UI_Timeline_node_point_data::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;



}
