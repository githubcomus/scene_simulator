// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "SettingInfo.h"
#include "SettingItemSelectUserWidget.generated.h"


class USettingPanelUserWidget;

class UTextBlock;
class UImage;
class UButton;
class UComboBoxString;


UCLASS()
class YXJS_API USettingItemSelectUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<USettingPanelUserWidget> owner;

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_name;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_selectStr;

	//
	TWeakObjectPtr<UComboBoxString> ComboBoxString_0;

	// 功能类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "aMyData")
		ESettingLineType lineType = ESettingLineType::ESettingLineType_None;

	// 下拉框列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "aMyData")
		TArray<FString> selectList;

	// 下拉框默认选中
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "aMyData")
		int32 selectIndex = 0;

	// 显示名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "aMyData")
		FString name = TEXT("test");


	/*------------------------------------------------------------------*/

	void Init();
	void Start();
	UFUNCTION(BlueprintCallable)
		void MySelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	virtual void NativePreConstruct();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
};
