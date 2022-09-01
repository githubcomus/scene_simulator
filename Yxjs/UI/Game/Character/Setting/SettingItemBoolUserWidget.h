// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "SettingItemBoolUserWidget.generated.h"


class USettingPanelUserWidget;

class UTextBlock;
class UImage;
class UButton;


UCLASS()
class YXJS_API USettingItemBoolUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<USettingPanelUserWidget> owner;

	//
	int itemIndex = 0;

	//
	FString name = TEXT("");

	//
	FString iconPath = TEXT("");

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UImage> Image_icon;

	//
	TWeakObjectPtr<UImage> Image_select;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_name;

	//
	TWeakObjectPtr<UButton> Button_;

	// 资源加载句柄
	TMap<UObject*, TSharedPtr<struct FStreamableHandle>>handleMap;

	/*------------------------------------------------------------------*/

	void Init();
	void LoadImage(bool start, TWeakObjectPtr<UObject> component, FString path);
	void SetImage(UTexture2D* texture2D, UImage* image);
	void Start();
	UFUNCTION(BlueprintCallable)
		void MyClickedEvent();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	void SetSelect(bool call, bool isOld);
};
