// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
//#include "TimelineInfo.h"
#include "Tips_Main.generated.h"


class UTips_Message;

class UTextBlock;
class UImage;
class UOverlay;

//

USTRUCT()
struct FMyMessageInfo
{
	GENERATED_BODY()

public:

	// 
	UPROPERTY(VisibleAnywhere)
		FTimerHandle timerHandle;

	// 
	UPROPERTY(VisibleAnywhere)
		TWeakObjectPtr<UTips_Message> ui;

};

UCLASS()
class YXJS_API UTips_Main : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Tips_Message;

	//
	TWeakObjectPtr<UOverlay> Overlay_show;

	// 是否初始化
	bool isInit = false;

	// 临时jobid
	int tempJobId = 100;

	// 消息列表 key:jobid,value:info
	UPROPERTY(VisibleAnywhere)
		TMap<int, FMyMessageInfo> messsageMap;

	/*------------------------------------------------------------------*/

	void Init();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION(BlueprintCallable)
		void OnExpandCollapse(bool isShow, bool useAnimation);
	void PushMessage(FString str);
	void Message_DelayRun(bool isStart, int tempJobId);
};
