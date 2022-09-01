// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include <Types/SlateEnums.h>
#include "EntityBackpack_LeftItem.generated.h"


class UEntityBackpackPanelUserWidget;

class UTextBlock;
class UImage;
class UEditableText;
class UOverlay;
class UButton;


UCLASS()
class YXJS_API UEntityBackpack_LeftItem : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UEntityBackpackPanelUserWidget> owner;

	// 
	xlsdata::MenuBarTableDefine* xlsInfo;

	//
	FString name = TEXT("");

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UButton> Button_tag;

	//
	TWeakObjectPtr<UImage> Image_background;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_str;

	/*------------------------------------------------------------------*/

	void Init();
	void Start();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	void SetSelect(bool in);
	UFUNCTION(BlueprintCallable)
		void MyClicked();
};
