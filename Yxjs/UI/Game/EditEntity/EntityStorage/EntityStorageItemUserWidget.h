// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include <Types/SlateEnums.h>
#include "EntityStorageItemUserWidget.generated.h"


class UEntityStoragePanelUserWidget;

class UTextBlock;
class UImage;
class UEditableText;
class UOverlay;


UCLASS()
class YXJS_API UEntityStorageItemUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UEntityStoragePanelUserWidget> owner;

	//
	int unique_mark = 0;

	// 联机数据
	Gamedata::PlayerStorageEntity entitySyncInfo;

	//
	FString name = TEXT("");

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UOverlay> Overlay_edit;

	//
	TWeakObjectPtr<UImage> Image_background;

	//
	TWeakObjectPtr<UEditableText> EditableText_str;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_EntityStorage_Item_drag;

	/*------------------------------------------------------------------*/

	void Init();
	void Start();
	void StartDrag(FString str);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	void SetSelect(bool call, bool isOld);
	UFUNCTION(BlueprintCallable)
		void MyTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
