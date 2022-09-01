// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "TimelineInfo.h"
#include "TimelineTimeAxisUserWidget.generated.h"


class UTimelinePanelUserWidget;

class UTextBlock;
class UImage;


UCLASS()
class YXJS_API UTimelineTimeAxisUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	// TWeakObjectPtr<UTextBlock> textBlock;


	TWeakObjectPtr < UImage> image1;

	TWeakObjectPtr < UImage> image2;

	TWeakObjectPtr < UImage> image3;

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UTimelinePanelUserWidget> panel;

	// 
	int32 entityId = 0;

	// 鼠标是否进入范围
	bool mouseEnter = false;

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
