// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "EntityBackpack_Item.generated.h"


class UEntityBackpackPanelUserWidget;

class UTextBlock;
class UImage;


UCLASS()
class YXJS_API UEntityBackpack_Item : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UEntityBackpackPanelUserWidget> owner;

	//
	FString name = TEXT("");

	//
	FString iconPath = TEXT("");

	//
	int xlsId = 0;


	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UImage> Image_icon;

	//
	TWeakObjectPtr<UImage> Image_select;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_name;

	// 资源加载句柄
	TMap<UObject*, TSharedPtr<struct FStreamableHandle>>handleMap;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_EntityStorage_Item_drag;

	/*------------------------------------------------------------------*/

	void Init();
	void LoadImage(bool start, TWeakObjectPtr<UObject> component, FString path);
	void SetImage(UTexture2D* texture2D, UImage* image);
	void Start();
	void StartDrag(FString str, UTexture2D* texture2D);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	void SetSelect(bool call, bool isOld);
};
