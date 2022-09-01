// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "EditEntityMainBaseSyncInfoUserWidget.generated.h"


class UEntityMaterialPanelUserWidget;

class UTextBlock;
class UImage;
class UEditableText;


UCLASS()
class YXJS_API UEditEntityMainBaseSyncInfoUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UEntityMaterialPanelUserWidget> owner;


	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UEditableText> EditableText_locationX;
	//
	TWeakObjectPtr<UEditableText> EditableText_locationY;
	//
	TWeakObjectPtr<UEditableText> EditableText_locationZ;
	//
	TWeakObjectPtr<UEditableText> EditableText_rotationX;
	//
	TWeakObjectPtr<UEditableText> EditableText_rotationY;
	//
	TWeakObjectPtr<UEditableText> EditableText_rotationZ;
	//
	TWeakObjectPtr<UEditableText> EditableText_scaleX;
	//
	TWeakObjectPtr<UEditableText> EditableText_scaleY;
	//
	TWeakObjectPtr<UEditableText> EditableText_scaleZ;

	// old
	FTransform oldTransform;

	//
	/*------------------------------------------------------------------*/

	void Init();
	UFUNCTION(BlueprintCallable)
		void MyTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	void SaveData();
	void SetData(FTransform transform);
	TWeakObjectPtr<AEditEntityActor> GetSelectEntityActor();
	//void Start();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
};
