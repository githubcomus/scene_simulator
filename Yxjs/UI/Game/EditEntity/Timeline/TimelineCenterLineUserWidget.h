// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Yxjs/Protobuf/hall.pb.h"
#include "Yxjs/Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "TimelineInfo.h"
#include "TimelineCenterLineUserWidget.generated.h"

class UTimelinePanelUserWidget;

class UTextBlock;
class UImage;

UCLASS()
class YXJS_API UTimelineCenterLineUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr < UImage> image;

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UTimelinePanelUserWidget> panel;

	//
	int32 entityId = 0;

	/*------------------------------------------------------------------*/

	void Init();
	virtual void NativeOnInitialized();
	//virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();

	void SetStr(FString str);
	void SetSelect(bool isSelect);
	void OnMouseEnterLeave(bool enter);
};
