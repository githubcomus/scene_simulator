// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
//#include "CommandButtonInfo.h"
#include "EntityStoragePanelUserWidget.generated.h"


class AEditEntityManage;
class AEditEntityActor;
class UEntityStorageItemUserWidget;
class UEntityStorageDropUserWidget;

class UTextBlock;
class UImage;
class UScrollBox;
class UButton;
class UCheckBox;


UCLASS()
class YXJS_API UEntityStoragePanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UScrollBox> ScrollBox_line;

	//
	TWeakObjectPtr<UButton> Button_add;

	//
	TWeakObjectPtr<UButton> Button_remove;

	//
	TWeakObjectPtr< UCheckBox> CheckBox_change;

	//
	TWeakObjectPtr< UImage> Image_stop;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_checkStr;

	//
	TWeakObjectPtr< UEntityStorageDropUserWidget> BP_UI_EntityStorage_Drop;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_EntityStorage_Item;

	//
	TArray<TWeakObjectPtr<UEntityStorageItemUserWidget>>itemList;

	//
	bool isInitData = false;

	// 唯一标识,不断递增
	int use_unique_mark = 100;

	// 当前选中标识
	int old_unique_mark = 0;

	// 修改标记
	int changeCount = 0;

	/*------------------------------------------------------------------*/

	void Init();
	Gamedata::PlayerInfo* GetPlayerSyncInfo();
	bool IsChangeName();
	void OpenUI();
	UFUNCTION(BlueprintCallable)
		void AddLineData();
	UFUNCTION(BlueprintCallable)
		void RemoveLineData();
	UFUNCTION(BlueprintCallable)
		void MyOnCheckStateChanged(bool bIsChecked);
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow);

	void ChangeEditEntityOrPreview(bool isEdit);

	void CloseUI();
	void InitData();
	void AddRow(int unique_mark, Gamedata::PlayerStorageEntity* entitySyncInfo);
	void SyncInfoToUIInfo();
	void UIInfoToSyncInfo();
	void OpenUISetSelect();
	UEntityStorageItemUserWidget* GetItem(int new_unique_mark);
	void SetSelect(int new_unique_mark);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

	TWeakObjectPtr<AEditEntityManage> GetEditEntityManage();
	TWeakObjectPtr<AEditEntityActor> GetEntityAcotr(int entityId);
	TWeakObjectPtr<AEditEntityActor> GetSelectEntityActor();

};