// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "CommandButtonInfo.h"
#include "CommandButtonPanelUserWidget.generated.h"


class AEditEntityManage;
class AEditEntityActor;

class UTextBlock;
class UImage;


UCLASS()
class YXJS_API UCommandButtonPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	// 数据UI版本
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FPlayerEntityButtonInfo syncInfoCommand;

	// 数据更新标记
	int changeCount = 0;

	/*------------------------------------------------------------------*/

	void Init();
	Gamedata::PlayerInfo* GetPlayerSyncInfo();
	void OpenUI();
	UFUNCTION(BlueprintCallable)
		void AddLineData(int keyIndex, FString commandStr);
	UFUNCTION(BlueprintCallable)
		void UpdateLineData(int keyIndex, int commandIndex, FString commandStr);
	UFUNCTION(BlueprintCallable)
		void RemoveLineData(int keyIndex, int commandIndex);
	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow);
	void CloseUI();
	virtual void NativeOnInitialized();
	////virtual void NativePreConstruct();
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
	void ProtobufToBPInfo(Gamedata::PlayerEntityButtonInfo* pbInfo, FPlayerEntityButtonInfo* uiInfo);
	void BPInfoToProtobuf(Gamedata::PlayerEntityButtonInfo* pbInfo, FPlayerEntityButtonInfo* uiInfo);

};
