// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_UI_Timeline_node_block_data.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Tools/CommonlyObject.h"

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
// controller
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

//
void UBP_UI_Timeline_node_block_data::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
//void UBP_UI_Timeline_node_block_data::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}

//
void UBP_UI_Timeline_node_block_data::NativeConstruct()
{
	Super::NativeConstruct();
	scrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_116")));

	// UE_LOG(LogTemp, Log, TEXT("[%x] [UBP_UI_Timeline_node_block_data::NativeConstruct] [scrollBox:%x]"), this, scrollBox.Get());

	auto textBlock2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock")));
	textList.Add(textBlock2);
	for (int i = 1; i < 40; i++)
	{
		FString name = FString::Printf(TEXT("TextBlock_%d"), i);
		FName name2(name);
		auto textBlock = Cast<UTextBlock>(GetWidgetFromName(name2));
		textList.Add(textBlock);
	}


	Init();
}

//
void UBP_UI_Timeline_node_block_data::NativeDestruct()
{
	Super::NativeDestruct();
}

/*------------------------------------------------------------------*/

// 初始化
void UBP_UI_Timeline_node_block_data::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

}


// 设置时间现实
void UBP_UI_Timeline_node_block_data::SetTimeString(float start, float end)
{
	if (isInit == false)
	{
		return;
	}
	float len = end - start + 1;
	float node = len / 20;
	for (int i = 0; i < 40; i += 2)
	{
		float val = start + float(i) / 2.0 * node;
		// FString str = FString::Printf(TEXT("%f"),val );
		std::string str = UCommonlyObject::FloatToString(val);
		FString str2 = UTF8_TO_TCHAR(str.c_str());
		FText text = FText::FromString(str2);
		textList[i]->SetText(text);
		textList[i + 1]->SetText(text);
	}

}