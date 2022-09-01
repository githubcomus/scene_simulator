// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "CommandTriggerInfo.h"
#include "CommandTriggerPanelUserWidget.generated.h"


class AEditEntityManage;
class AEditEntityActor;

class UTextBlock;
class UImage;


UCLASS()
class YXJS_API UCommandTriggerPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	// 数据UI版本
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FEntityCommandInfo syncInfoCommand;

	// 本地数据 localInfo;

	// 数据更新标记
	int changeCount = 0;

	/*------------------------------------------------------------------*/

	void Init();
	void OpenUI();
	void OnExpandCollapse(bool isShow);
	void CloseUI();
	UFUNCTION(BlueprintCallable)
		void DateUpdateChangePage(EEntityCommandType type);
	UFUNCTION(BlueprintCallable)
		void DateAddCommand(FEntityCommandCommand command);
	UFUNCTION(BlueprintCallable)
		void DateRemoveCommand(int valueIndex);
	UFUNCTION(BlueprintCallable)
		void DateUpdateCommand(int valueIndex, FEntityCommandCommand command);
	virtual void NativeOnInitialized();
	//virtual void NativePreConstruct();
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
	void ProtobufToBPInfo(Gamedata::EntityCommandInfo* pbInfo, FEntityCommandInfo* uiInfo);
	void BPInfoToProtobuf(Gamedata::EntityCommandInfo* pbInfo, FEntityCommandInfo* uiInfo);

	//UFUNCTION(BlueprintImplementableEvent)
	//	void Event_Update();
};
