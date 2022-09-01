// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Yxjs/Protobuf/hall.pb.h"
#include "Yxjs/Protobuf/game.pb.h"
//#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
//#include "TimelineInfo.h"
#include "EntityMaterial_Color.generated.h"

class UEntityMaterialPanelUserWidget;
class AEditEntityActor;

class UTextBlock;
class UImage;
class UBorder;
class UButton;
class UEditableText;
class UCheckBox;


UCLASS()
class YXJS_API UEntityMaterial_Color : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/



	//
	TWeakObjectPtr < UTextBlock> TextBlock_str;
	//
	TWeakObjectPtr < UImage> Sign;

	//
	TWeakObjectPtr < UBorder> Border_;

	//
	TWeakObjectPtr < UButton> Button_application;
	//
	TWeakObjectPtr < UEditableText> EditableText_r;
	//
	TWeakObjectPtr < UEditableText> EditableText_g;
	//
	TWeakObjectPtr < UEditableText> EditableText_b;
	//
	TWeakObjectPtr < UCheckBox> CheckBox_;

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UEntityMaterialPanelUserWidget> owner_;

	//
	FVector oldColor;


	/*------------------------------------------------------------------*/

	void Init();
	void Reset(TWeakObjectPtr<UEntityMaterialPanelUserWidget> owner, TWeakObjectPtr<AEditEntityActor>  entityActor);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void OnMouseMove(bool isPanel, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void SetBoxImageColor();

	void SetUIData(bool highlight, FVector color);

	UFUNCTION(BlueprintImplementableEvent)
		FVector GetColorValue();

	UFUNCTION(BlueprintCallable)
		void MyButton_application();
};
