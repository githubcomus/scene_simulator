// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityBackpackPanelUserWidget.h"

#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Tools/CalculationObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerCommandComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Yxjs/Core/BaseGameInstance.h"
#include "Yxjs/Core/Component/ZwXlsDataActorComponent.h"
#include "Yxjs/Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/PlayerPlaceComponent.h"
#include "EntityBackpackDropUserWidget.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include <Components/EditableText.h>
#include <Components/TextBlock.h>
#include "EntityBackpackMainUserWidget.h"
#include <Components/UniformGridPanel.h>
#include <Components/VerticalBox.h>
#include <Components/ScrollBox.h>
#include "EntityBackpack_LeftItem.h"
#include "Yxjs/Pawn/Character/CharacterComponent/PlayerInputComponent.h"


/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 外部数据 获取 动态获取view
TWeakObjectPtr<AEditEntityManage> UEntityBackpackPanelUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> UEntityBackpackPanelUserWidget::GetEntityAcotr(int entityId)
{
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// 获取当前选中的actor
TWeakObjectPtr<AEditEntityActor> UEntityBackpackPanelUserWidget::GetSelectEntityActor()
{

	auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto character = Cast<ABaseCharacter>(player->GetPawn());
	if (character == nullptr)return nullptr;
	if (character->selectComponent == nullptr)return nullptr;
	auto entityList = &character->selectComponent->cvMultipleInstanceInfo.selectedList;

	auto& entityList_ = *entityList;
	if (entityList_.Num() == 0)return nullptr;
	int masterEntityId = entityList_[entityList_.Num() - 1].entityId;

	auto actor = GetEntityAcotr(masterEntityId);
	return actor;
}

// 玩家数据
Gamedata::PlayerInfo* UEntityBackpackPanelUserWidget::GetPlayerSyncInfo()
{
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return nullptr;

	auto syncInfo = character->myData->GetSyncInfo();
	return syncInfo;

}

// 读取指定资源名字,并且拼凑成iocn路径
FString UEntityBackpackPanelUserWidget::GetIconPath(int xlsId)
{

	auto xlsInfo = GetPlaceXlsInfoWithXlsId(xlsId);
	if (xlsInfo == nullptr)return TEXT("");

	FString tempPath = UTF8_TO_TCHAR(xlsInfo->MeshPath_.c_str());
	FString name = FPaths::GetBaseFilename(tempPath, true);
	FString path = TEXT("Texture2D'/Game/Resources/Icon/Mesh/") + name + TEXT(".") + name + TEXT("'");
	UE_LOG(LogTemp, Log, TEXT("[%x] [资源文件夹] [GetIconPath] [path:%s]  [name:%s]"), this, *path, *name);
	return path;
}

// 获取 获取背包表格数据
xlsdata::BackpackTableDefine* UEntityBackpackPanelUserWidget::GetBackpackXlsInfoWithXlsId(int32 xlsId)
{
	// 注意:传递指针会导致string内容被清楚,只能传递引用,访问源数据

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance_->zwXlsDataActorComponent->backpacktMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->backpacktMap.end())
	{
		return &gameInstance_->zwXlsDataActorComponent->backpacktMap[xlsId];
	}
	return nullptr;
}

// 获取 获取表格数据
xlsdata::PlaceTableDefine* UEntityBackpackPanelUserWidget::GetPlaceXlsInfoWithXlsId(int32 xlsId)
{
	// 注意:传递指针会导致string内容被清楚,只能传递引用,访问源数据

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance_->zwXlsDataActorComponent->placeMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->placeMap.end())
	{
		return &gameInstance_->zwXlsDataActorComponent->placeMap[xlsId];
	}
	return nullptr;
}

// 获取 获取表格数据
xlsdata::MaterialTableDefine* UEntityBackpackPanelUserWidget::GetMaterialXlsInfoWithXlsId(int32 xlsId)
{
	// 注意:传递指针会导致string内容被清楚,只能传递引用,访问源数据

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance_->zwXlsDataActorComponent->materialMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->materialMap.end())
	{
		return &gameInstance_->zwXlsDataActorComponent->materialMap[xlsId];
	}
	return nullptr;
}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/
//
void UEntityBackpackPanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityBackpackPanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityBackpackPanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

}

//
FReply UEntityBackpackPanelUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled();
}

//
FReply UEntityBackpackPanelUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	return FReply::Handled();
}

//
FReply UEntityBackpackPanelUserWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	return FReply::Handled();
}

//
void UEntityBackpackPanelUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEntityBackpackPanelUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UEntityBackpackPanelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

/*------------------------------------------------------------------*/

// UI创建的初始化,各种组件变量
void UEntityBackpackPanelUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	{
		FString bpPath = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/EntityBackpack/BP_UI_EntityBackpack_Item.BP_UI_EntityBackpack_Item_C'");
		class_BP_UI_EntityBackpack_Item = LoadClass<UEntityBackpack_Item>(this, *bpPath);
		if (class_BP_UI_EntityBackpack_Item == nullptr)
		{
			return;
		}
	}

	{
		FString bpPath = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/EntityBackpack/BP_UI_EntityBackpack_LeftItem.BP_UI_EntityBackpack_LeftItem_C'");
		class_BP_UI_EntityBackpack_LeftItem = LoadClass<UEntityBackpack_LeftItem>(this, *bpPath);
		if (class_BP_UI_EntityBackpack_LeftItem == nullptr)
		{
			return;
		}
	}
	Uniform_Grid_Panel_right_list = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("Uniform_Grid_Panel_right_list")));
	BP_UI_Bacackpack_Drop = Cast<UEntityBackpackDropUserWidget>(GetWidgetFromName(TEXT("BP_UI_Bacackpack_Drop")));
	BP_UI_Bacackpack_Drop->owner = this;
	ScrollBox_left_list = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_left_list")));

}

// UI回调 展开收起事件
void UEntityBackpackPanelUserWidget::OnExpandCollapse(bool isShow)
{

	auto syncInfo = GetPlayerSyncInfo();
	if (syncInfo == nullptr)return;

	if (isShow)
	{
		OpenUI();
	}
	else
	{
		CloseUI();
	}

	FString str = isShow ? TEXT("展开") : TEXT("折叠");
	UE_LOG(LogTemp, Log, TEXT("[%x] [资源文件夹] [OnExpandCollapse] [%s]"), this, *str);
}

// UI回调 展开
void UEntityBackpackPanelUserWidget::OpenUI()
{

	if (UFunction* func = FindFunction("Event_OpenUI"))
	{
		struct TempParam
		{
		};
		TempParam tempParam;

		ProcessEvent(func, &tempParam);
	}
	UI_InitUI();
	//BP_UI_Bacackpack_Drop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ChangeEditEntityOrPreview(true);
}

// UI回调 收起
void UEntityBackpackPanelUserWidget::CloseUI()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	if (character->commandComponent == nullptr)return;

	BP_UI_Bacackpack_Drop->SetVisibility(ESlateVisibility::Hidden);

}

// UI调用 打开UI初始化
void UEntityBackpackPanelUserWidget::UI_InitUI()
{

	if (isInitData)return;
	isInitData = true;

	OpenSecondType();
	OpenThirdType(xlsdata::SecondMenuType::SecondMenuType_Mesh);
}

// 初始第二层级
void UEntityBackpackPanelUserWidget::OpenSecondType()
{

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	auto& xlsMap = gameInstance_->zwXlsDataActorComponent->menuBarMap;

	if (ScrollBox_left_list.IsValid() == false)return;

	std::vector<xlsdata::MenuBarTableDefine*> xlsList;
	for (auto node = xlsMap.begin(); node != xlsMap.end(); ++node)
	{
		xlsList.push_back(&node->second);
	}

	auto addRowFunc = [&](int thisListIndex, xlsdata::MenuBarTableDefine* xlsInfo) {

		if (class_BP_UI_EntityBackpack_LeftItem == nullptr)
		{
			return;
		}
		UEntityBackpack_LeftItem* pItem = Cast<UEntityBackpack_LeftItem>(
			CreateWidget(GetWorld(), class_BP_UI_EntityBackpack_LeftItem));
		if (pItem == nullptr)
		{
			return;
		}
		pItem->owner = this;

		pItem->xlsInfo = xlsInfo;
		pItem->name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());

		ScrollBox_left_list->AddChild(pItem);
		leftItemList.Add(pItem);
		pItem->Start();
		UE_LOG(LogTemp, Log, TEXT("[%x] [资源文件夹] [OpenSecondType] [thisListIndex:%d]"), this, thisListIndex);
	};

	// 清空之前的数据
	leftItemList.Reset();
	auto childList = ScrollBox_left_list->GetAllChildren();
	for (auto node : childList)
	{
		ScrollBox_left_list->RemoveChild(node);
	}

	for (int thisListIndex = 0; thisListIndex < xlsList.size(); thisListIndex++)
	{
		auto xlsInfo = xlsList[thisListIndex];
		addRowFunc(thisListIndex, xlsInfo);
	}
}

// 初始化第三层级别
void UEntityBackpackPanelUserWidget::OpenThirdType(xlsdata::SecondMenuType secondType)
{

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	auto& xlsMap = gameInstance_->zwXlsDataActorComponent->backpacktMap;

	//1. 过滤源数据
	std::vector<xlsdata::BackpackTableDefine*> xlsList;
	for (auto node = xlsMap.begin(); node != xlsMap.end(); ++node)
	{
		if (node->second.SecondMenuType_ == secondType)
		{
			xlsList.push_back(&node->second);
		}
	}

	//2. 新增行回调
	auto addRowFunc = [&](int thisRowIndex, int thisListIndex, int itemIndex, xlsdata::BackpackTableDefine* xlsInfo) {

		if (class_BP_UI_EntityBackpack_Item == nullptr)return;
		UEntityBackpack_Item* child = Cast<UEntityBackpack_Item>(
			CreateWidget(GetWorld(), class_BP_UI_EntityBackpack_Item));
		if (child == nullptr)return;
		child->owner = this;

		child->name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());
		child->iconPath = GetIconPath(xlsInfo->ChildID_);//UTF8_TO_TCHAR(xlsInfo->IconPath_.c_str());
		child->xlsId = xlsInfo->ChildID_;

		Uniform_Grid_Panel_right_list->AddChildToUniformGrid(child, thisRowIndex, thisListIndex);
		rightItemList.Add(child);
		child->Start();
		UE_LOG(LogTemp, Log, TEXT("[%x] [资源文件夹] [OpenThirdType] [thisRowIndex:%d] [thisListIndex:%d] [itemIndex:%d]"), this, thisRowIndex, thisListIndex, itemIndex);
	};

	//3. 清空之前的数据
	rightItemList.Reset();
	auto childList = Uniform_Grid_Panel_right_list->GetAllChildren();
	for (auto node : childList)
	{
		Uniform_Grid_Panel_right_list->RemoveChild(node);
	}

	int maxListCount = 6;	// 每行最大字段数量
	int maxItemCount = xlsList.size();	// 总共的节点数量
	int rowCount = maxItemCount / maxListCount;
	bool lastRow = (maxItemCount % maxListCount) > 0 ? true : false;
	int itemIndex = 0;

	// 前面的行
	for (int thisRowIndex = 0; thisRowIndex < rowCount; thisRowIndex++)
	{
		for (int thisListIndex = 0; thisListIndex < maxListCount; thisListIndex++)
		{
			auto xlsInfo = xlsList[itemIndex];
			addRowFunc(thisRowIndex, thisListIndex, itemIndex, xlsInfo);
			itemIndex++;
		}
	}

	// 最后一行
	if (lastRow)
	{
		for (int thisListIndex = 0; thisListIndex < (maxItemCount % maxListCount); thisListIndex++)
		{
			auto xlsInfo = xlsList[itemIndex];
			addRowFunc(rowCount, thisListIndex, itemIndex, xlsInfo);
			itemIndex++;
		}
	}
}

// 切换编辑实例和拖拽姿态
void UEntityBackpackPanelUserWidget::ChangeEditEntityOrPreview(bool isEdit)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (isEdit)
	{
		character->inputComp->inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
	}
	else
	{
		character->inputComp->inputMode = ECharacterStatusType::CharacterStatusType_Preview;
	}
}

