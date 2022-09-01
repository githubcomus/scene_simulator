// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BP_UI_Timeline_node_block_data.generated.h"


class UScrollBox;
class UTextBlock;

/**
 * 时间轴滚动条内的区块节点
 */
UCLASS()
class YXJS_API UBP_UI_Timeline_node_block_data : public UUserWidget
{
	GENERATED_BODY()
public:

	/*------------------------------------------------------------------*/

	// 纵向滚动条
	TWeakObjectPtr<UScrollBox> scrollBox = nullptr;

	// 文本
	TArray< TWeakObjectPtr<UTextBlock>> textList;

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	/*------------------------------------------------------------------*/

	virtual void NativeOnInitialized();
	//virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();

	void Init();
	void SetTimeString(float start, float end);
};
