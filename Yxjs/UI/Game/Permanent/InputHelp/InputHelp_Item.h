// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Types/SlateEnums.h>
#include "InputHelp_Item.generated.h"


class UInputHelp_Panel;

class UTextBlock;
class UImage;
class UEditableText;
class UOverlay;


UCLASS()
class YXJS_API UInputHelp_Item : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UInputHelp_Panel> owner;

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UImage> Image_background;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_str;


	/*------------------------------------------------------------------*/

	void Init();
	void Start(TWeakObjectPtr<UInputHelp_Panel> owner_, FString str);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
};
