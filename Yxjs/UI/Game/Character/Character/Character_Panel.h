// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Character_Panel.generated.h"


class AEditEntityManage;
class AEditEntityActor;
class UCharacter_Item;

class UTextBlock;
class UImage;
class UScrollBox;
class UUniformGridPanel;



UCLASS()
class YXJS_API UCharacter_Panel : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//// 角色信息
	//struct CvMyCharacterInfo {
	//	//int itemIndex = 0;
	//	FString name = TEXT("");	// 名称
	//	FString content = TEXT("");	// 内容
	//	FString bpPath = TEXT("");	// 蓝图路径
	//	FString iconPath = TEXT("");	// icon路径
	//};


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
		UClass* class_BP_UI_Character_Item_C;

	//
	TWeakObjectPtr<UScrollBox> ScrollBox_line;

	//
	TWeakObjectPtr<UUniformGridPanel> Uniform_Grid_Panel;


	//
	TArray<TWeakObjectPtr<UCharacter_Item>>itemList;

	// 角色信息
	//TArray<CvMyCharacterInfo> characterInfoList;

	/*------------------------------------------------------------------*/

	void Init();
	Gamedata::PlayerInfo* GetPlayerSyncInfo();
	void OpenUI();
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow);
	void CloseUI();
	void InitData();
	void InitCharacterData();
	void OpenUISetSelect();
	void SetSelect(int newSelectIndex, bool doRealJob = true);
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
