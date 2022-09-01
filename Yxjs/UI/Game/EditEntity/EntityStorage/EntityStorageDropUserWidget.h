// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include <Types/SlateEnums.h>
#include "EntityStorageDropUserWidget.generated.h"


class UEntityStoragePanelUserWidget;

class UTextBlock;
class UImage;
class UEditableText;
class UOverlay;


UCLASS()
class YXJS_API UEntityStorageDropUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	//
	TWeakObjectPtr<UEntityStoragePanelUserWidget> owner;

	// 是否初始化
	bool isInit = false;

	// 启动预览
	//bool isPreview = false;

	/*------------------------------------------------------------------*/

	void Init();
	void CreatePreview(Gamedata::PlayerStorageEntity* syncInfo);
	void Start();
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
};
