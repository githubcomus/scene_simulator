// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityMainButtonUserWidget.h"
//#include "EntityMaterialPanelUserWidget.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
//#include "EntityMaterialPanelUserWidget.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>
#include "Yxjs/FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "../../../../Core/BaseGameInstance.h"
#include "Yxjs/Pawn/Character/BaseCharacter.h"
#include "Components/Button.h"
#include "Yxjs/Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/
//

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> UEditEntityMainButtonUserWidget::GetEntityAcotr(int entityId)
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto view = baseGameMode->editEntityManage;
	if (view == nullptr)return nullptr;
	return view->data->GetEntityActor(entityId);
}

//
UPlayerSelectedComponent* UEditEntityMainButtonUserWidget::GetSelectedComponent()
{
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	return baseCharacter->selectComponent;
}

// 获取 动态获取view
AEditEntityManage* UEditEntityMainButtonUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 获取 获取表格数据
xlsdata::MaterialTableDefine* UEditEntityMainButtonUserWidget::GetXlsInfoWithXlsId(int32 xlsId)
{
	// 注意:传递指针会导致string内容被清楚,只能传递引用,访问源数据

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance_->zwXlsDataActorComponent->materialMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->materialMap.end())
	{
		return &gameInstance_->zwXlsDataActorComponent->materialMap[xlsId];
	}
	return nullptr;
}

// 获取 所有选中实例
void UEditEntityMainButtonUserWidget::GetSelectInstanceList(std::vector<int>& selectList)
{
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (baseCharacter == nullptr)return;
	baseCharacter->selectComponent->GetSelectInstanceList(selectList);
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UEditEntityMainButtonUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEditEntityMainButtonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEditEntityMainButtonUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEditEntityMainButtonUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UEditEntityMainButtonUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UEditEntityMainButtonUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEditEntityMainButtonUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
void UEditEntityMainButtonUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	Image_stop = Cast<UImage>(GetWidgetFromName(TEXT("Image_stop")));
	Button_0 = Cast<UButton>(GetWidgetFromName(TEXT("Button_0")));

	FScriptDelegate func;
	func.BindUFunction(this, TEXT("MyButtonEnter"));
	Button_0->OnClicked.AddUnique(func);

}

// 设置禁止点击
void UEditEntityMainButtonUserWidget::SetStopEnter(bool isShow)
{
	if (isShow)
	{
		Image_stop->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_stop->SetVisibility(ESlateVisibility::Hidden);
	}
}

// 按钮点击
void UEditEntityMainButtonUserWidget::MyButtonEnter()
{

	if (Image_stop->GetVisibility() != ESlateVisibility::Hidden)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [编辑实例主页] [MyButtonEnter] [按钮按下] err"), this);
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [编辑实例主页] [MyButtonEnter] [按钮按下]"), this);

	if (type == EEditEntityMainButtonType::EEditEntityMainButtonType_merge)
	{
		EntityMerge();
	}
	else if (type == EEditEntityMainButtonType::EEditEntityMainButtonType_break_down)
	{
		EntityBreakDown();
	}

}

// 合并实例
void UEditEntityMainButtonUserWidget::EntityMerge()
{
	auto placeView = GetEditEntityManage();
	if (placeView == nullptr)return;

	// 所有选中的都是实例验证
	std::vector<int> selectList;
	GetSelectInstanceList(selectList);

	if (selectList.size() <= 1)
	{
		return;
	}

	// 收集合并数据
	std::vector<Gamedata::EntityInfo*> syncInfoList;
	for (int i = selectList.size() - 1; i >= 0; i--)
	{
		int node = selectList[i];
		Gamedata::EntityInfo* syncInfo = placeView->data->GetSyncInfo(node);
		syncInfoList.push_back(syncInfo);
	}

	placeView->controller->NetCall_MergeEntity(syncInfoList);// 发送合并请求

	//GetSelectedComponent()->OperationResultsSwitch(UPlayerSelectedComponent::Operation_Type::Operation_Type_Delete);// 编辑模式下删除选中数据

}

// 分解实例
void UEditEntityMainButtonUserWidget::EntityBreakDown()
{
	auto placeView = GetEditEntityManage();
	if (placeView == nullptr)return;

	std::vector<int> selectList;
	GetSelectInstanceList(selectList);
	if (selectList.size() != 1)
	{
		return;
	}
	// 收集合并数据
	std::vector<Gamedata::EntityInfo*> syncInfoList;
	for (auto node : selectList)
	{
		auto syncInfo = placeView->data->GetSyncInfo(node);
		syncInfoList.push_back(syncInfo);
	}
	placeView->controller->NetCall_BreakDownEntity(syncInfoList);
}