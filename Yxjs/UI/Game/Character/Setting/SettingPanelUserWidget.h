// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "SettingPanelUserWidget.generated.h"


class AEditEntityManage;
class AEditEntityActor;
//class USkyLevelItemUserWidget;

class UTextBlock;
class UImage;
class UScrollBox;


UCLASS()
class YXJS_API USettingPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	// 数据更新标记
	int changeCount = 0;

	// 是否初始化过
	bool isInitData = false;

	// 之前选中的
	int oldSelectIndex = 0;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_SkyLevel_Item_C;

	//
	TWeakObjectPtr<UScrollBox> ScrollBox_line;

	//
	//TArray<TWeakObjectPtr<USkyLevelItemUserWidget>>itemList;

	/*------------------------------------------------------------------*/

	void Init();
	Gamedata::PlayerInfo* GetPlayerSyncInfo();
	void OpenUI();
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow);
	void CloseUI();
	void InitData();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	TWeakObjectPtr<AEditEntityManage> GetEditEntityManage();
	TWeakObjectPtr<AEditEntityActor> GetEntityAcotr(int entityId);
	TWeakObjectPtr<AEditEntityActor> GetSelectEntityActor();

};
