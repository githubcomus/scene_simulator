// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Tips_Message.generated.h"


class UTips_Main;

class UTextBlock;
class UImage;
class UCanvasPanel;


UCLASS()
class YXJS_API UTips_Message : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UTips_Main> owner_;

	//
	TWeakObjectPtr<UTextBlock> TextBlock_str;
	//
	TWeakObjectPtr<UCanvasPanel> CanvasPanel_move;

	/*------------------------------------------------------------------*/

	void Init();
	void Start(TWeakObjectPtr<UTips_Main> owner, FString str);
	void End();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();

};
