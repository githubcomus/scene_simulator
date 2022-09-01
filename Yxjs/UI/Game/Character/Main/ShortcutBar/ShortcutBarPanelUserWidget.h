// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "ShortcutBarInfo.h"
#include "ShortcutBarPanelUserWidget.generated.h"


class AEditEntityManage;
class AEditEntityActor;
class UShortcutBarItemUserWidget;

class UTextBlock;
class UImage;


UCLASS()
class YXJS_API UShortcutBarPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	// 数据更新标记
	int changeCount = 0;

	// 按钮指针列表
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<UShortcutBarItemUserWidget*> itemList;

	/*------------------------------------------------------------------*/

	void Init();
	Gamedata::PlayerInfo* GetPlayerSyncInfo();
	void OpenUI();
	void OnExpandCollapse(bool isShow, UUserWidget* owner_);
	void CloseUI();
	void Input_Number(int keyIndex, bool enter);
	void UICallback_ItemButtonEnter(int keyIndex, int valueIndex);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();

	TWeakObjectPtr<AEditEntityManage> GetEditEntityManage();
	TWeakObjectPtr<AEditEntityActor> GetEntityAcotr(int entityId);
	TWeakObjectPtr<AEditEntityActor> GetSelectEntityActor();

};
