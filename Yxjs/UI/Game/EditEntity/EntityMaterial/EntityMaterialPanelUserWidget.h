// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "EntityMaterialPanelUserWidget.generated.h"


class AEditEntityManage;
class AEditEntityActor;
class UEntityMaterialItemUserWidget;
class UEntityMaterial_Color;

class UTextBlock;
class UImage;
class UUniformGridPanel;


UCLASS()
class YXJS_API UEntityMaterialPanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 是否初始化
	bool isInit = false;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_EntityMaterial_Item;

	//
	TWeakObjectPtr<UUniformGridPanel> Uniform_Grid_Panel_;

	//
	TArray<TWeakObjectPtr<UEntityMaterialItemUserWidget>>itemList;

	//
	TWeakObjectPtr<UEntityMaterial_Color> BP_UI_EntityMaterial_Color;

	// 选中
	int oldSelectIndex = -1;

	//
	bool isInitData = false;

	/*------------------------------------------------------------------*/

	void Init();
	void OpenUI();
	void OnExpandCollapse(bool isShow);
	void CloseUI();

	std::map<int32, xlsdata::MaterialTableDefine>& GetXlsData();
	FString GetIconPath(std::string materialPath);
	void InitData();
	void OpenUISetSelect();
	void SetSelect(int index, bool isInit_ = false);
	void SetChangeColor(FVector color, bool highlight);
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

};
