// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "EditEntityMainButtonUserWidget.generated.h"


class UEntityMaterialPanelUserWidget;
class AEditEntityActor;
class UPlayerSelectedComponent;
class AEditEntityManage;

class UTextBlock;
class UImage;
class UButton;

UENUM(BlueprintType)
enum class EEditEntityMainButtonType : uint8
{
	EEditEntityMainButtonType_merge = 0   UMETA(DisplayName = "合并"),
	EEditEntityMainButtonType_break_down = 1   UMETA(DisplayName = "分解"),
};


UCLASS()
class YXJS_API UEditEntityMainButtonUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 按键类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
		EEditEntityMainButtonType type = EEditEntityMainButtonType::EEditEntityMainButtonType_merge;

	//
	//TWeakObjectPtr<UEntityMaterialPanelUserWidget> owner;

	// 是否初始化
	bool isInit = false;

	//
	TWeakObjectPtr<UImage> Image_stop;

	//
	TWeakObjectPtr<UButton> Button_0;

	/*------------------------------------------------------------------*/

	void Init();
	void SetStopEnter(bool isShow);
	UFUNCTION(BlueprintCallable)
		void MyButtonEnter();
	void EntityMerge();
	void EntityBreakDown();
	TWeakObjectPtr<AEditEntityActor> GetEntityAcotr(int entityId);
	UPlayerSelectedComponent* GetSelectedComponent();
	AEditEntityManage* GetEditEntityManage();
	xlsdata::MaterialTableDefine* GetXlsInfoWithXlsId(int32 xlsId);
	void GetSelectInstanceList(std::vector<int>& selectList);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
};
