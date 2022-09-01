// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "EntityBackpackPanelUserWidget.generated.h"


class AEditEntityManage;
class AEditEntityActor;
class UEntityBackpack_Item;
class UEntityBackpackDropUserWidget;
class UEntityBackpack_LeftItem;

class UTextBlock;
class UImage;
class UUniformGridPanel;
class UVerticalBox;
class UScrollBox;


UCLASS()
class YXJS_API UEntityBackpackPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	// 数据初始化
	bool isInitData = false;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_EntityBackpack_Item;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_EntityBackpack_LeftItem;

	//
	TWeakObjectPtr<UUniformGridPanel> Uniform_Grid_Panel_right_list;

	//
	TArray<TWeakObjectPtr<UEntityBackpack_Item>>rightItemList;

	//
	TWeakObjectPtr<UEntityBackpackDropUserWidget> BP_UI_Bacackpack_Drop;

	//
	TWeakObjectPtr<UScrollBox> ScrollBox_left_list;

	//
	TArray<TWeakObjectPtr<UEntityBackpack_LeftItem>> leftItemList;

	/*------------------------------------------------------------------*/

	void Init();
	Gamedata::PlayerInfo* GetPlayerSyncInfo();
	void OpenUI();
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow);
	void CloseUI();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	TWeakObjectPtr<AEditEntityManage> GetEditEntityManage();
	TWeakObjectPtr<AEditEntityActor> GetEntityAcotr(int entityId);
	TWeakObjectPtr<AEditEntityActor> GetSelectEntityActor();

	void InitBackpackData();
	FString GetIconPath(int xlsId);
	void SetItemInfo(int firstType, int secondType);
	xlsdata::PlaceTableDefine* GetPlaceXlsInfoWithXlsId(int32 xlsId);
	xlsdata::MaterialTableDefine* GetMaterialXlsInfoWithXlsId(int32 xlsId);
	void UI_InitUI(int firstType);
	void OpenSecondType();
	void OpenThirdType(xlsdata::SecondMenuType secondType);
	void ChangeEditEntityOrPreview(bool isEdit);
	void UI_SetSecondItem(int firstType);
	void UI_SetThirdItem(int firstType, int secondType);
	void UI_InitUI();
	xlsdata::BackpackTableDefine* GetBackpackXlsInfoWithXlsId(int32 xlsId);

};
