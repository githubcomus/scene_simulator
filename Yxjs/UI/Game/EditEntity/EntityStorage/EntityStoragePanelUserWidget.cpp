// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityStoragePanelUserWidget.h"

#include "Tools/CommonlyObject.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Tools/CalculationObject.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerCommandComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerInputComponent.h"

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
#include "EntityStorageItemUserWidget.h"
#include <Components/ScrollBox.h>
#include <Components/Button.h>
#include <Components/CheckBox.h>
#include "EntityStorageDropUserWidget.h"
#include <Components/Image.h>
#include "../../../HUDStaticFunc.h"
#include "../../../../Core/GameInstanceStaticFunc.h"

/*
拖拽流程:
1.开启拖拽 UEntityStorageItemUserWidget::NativeOnDragDetected
2.进入拖拽领域,UI结束本次拖拽 UEntityStorageDropUserWidget::NativeOnDragEnter
3.修改输入模式 UEntityStoragePanelUserWidget::ChangeEditEntityOrPreview
4.开启tick预览 UPlayerPreviewComponent::OpenClosePerview
5.捕获鼠标左键松开 UPlayerInputComponent::MouseLeft_Released
6.结束拖拽 UPlayerPreviewComponent::OpenClosePerview
*/


/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 外部数据 获取 动态获取view
TWeakObjectPtr<AEditEntityManage> UEntityStoragePanelUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> UEntityStoragePanelUserWidget::GetEntityAcotr(int entityId)
{
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// 获取当前选中的actor
TWeakObjectPtr<AEditEntityActor> UEntityStoragePanelUserWidget::GetSelectEntityActor()
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
Gamedata::PlayerInfo* UEntityStoragePanelUserWidget::GetPlayerSyncInfo()
{
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return nullptr;

	auto syncInfo = character->myData->GetSyncInfo();
	return syncInfo;

}

// 改名/拖拽
bool UEntityStoragePanelUserWidget::IsChangeName()
{
	if (CheckBox_change.IsValid() == false)return true;
	bool flag = CheckBox_change->IsChecked();
	return flag;

}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/
//
void UEntityStoragePanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityStoragePanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityStoragePanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEntityStoragePanelUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
FReply UEntityStoragePanelUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
FReply UEntityStoragePanelUserWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
void UEntityStoragePanelUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEntityStoragePanelUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

// UI创建的初始化,各种组件变量
void UEntityStoragePanelUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	FString bpPath = TEXT(
		"WidgetBlueprint'/Game/Yxjs/UI/EditEntity/EntityStorage/BP_UI_EntityStorage_Item.BP_UI_EntityStorage_Item_C'");
	class_BP_UI_EntityStorage_Item = LoadClass<UEntityStorageItemUserWidget>(this, *bpPath);
	if (class_BP_UI_EntityStorage_Item == nullptr)
	{
		return;
	}
	ScrollBox_line = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_line")));
	Button_add = Cast<UButton>(GetWidgetFromName(TEXT("Button_add")));
	Button_remove = Cast<UButton>(GetWidgetFromName(TEXT("Button_remove")));
	CheckBox_change = Cast<UCheckBox>(GetWidgetFromName(TEXT("CheckBox_change")));
	BP_UI_EntityStorage_Drop = Cast<UEntityStorageDropUserWidget>(GetWidgetFromName(TEXT("BP_UI_EntityStorage_Drop")));
	BP_UI_EntityStorage_Drop->owner = this;

	Image_stop = Cast<UImage>(GetWidgetFromName(TEXT("Image_stop")));
	TextBlock_checkStr = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_checkStr")));


	if (Button_add.IsValid())
	{
		FScriptDelegate func;
		func.BindUFunction(this, TEXT("AddLineData"));
		Button_add->OnClicked.AddUnique(func);
	}
	if (Button_remove.IsValid())
	{
		FScriptDelegate func;
		func.BindUFunction(this, TEXT("RemoveLineData"));
		Button_remove->OnClicked.AddUnique(func);
	}
	if (CheckBox_change.IsValid())
	{
		FScriptDelegate func;
		func.BindUFunction(this, TEXT("MyOnCheckStateChanged"));
		CheckBox_change->OnCheckStateChanged.AddUnique(func);
	}
}

// UI回调 展开收起事件
void UEntityStoragePanelUserWidget::OnExpandCollapse(bool isShow)
{

	auto syncInfo = GetPlayerSyncInfo();
	if (syncInfo == nullptr)return;

	FString str = isShow ? TEXT("展开") : TEXT("折叠");
	UE_LOG(LogTemp, Log, TEXT("[%x] [命令控制] [OnExpandCollapse] [%s]"), this, *str);


	if (isShow)
	{
		OpenUI();
	}
	else
	{
		CloseUI();
	}

}

// UI回调 展开
void UEntityStoragePanelUserWidget::OpenUI()
{

	changeCount = 0;

	auto syncInfo = GetPlayerSyncInfo();
	if (syncInfo == nullptr)return;

	if (UFunction* func = FindFunction("Event_OpenUI"))
	{
		struct TempParam
		{
		};
		TempParam tempParam;

		ProcessEvent(func, &tempParam);
	}

	InitData();
	//BP_UI_EntityStorage_Drop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ChangeEditEntityOrPreview(true);
}

// UI回调 收起
void UEntityStoragePanelUserWidget::CloseUI()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

	// 保存到服务器
	if (changeCount > 0)
	{
		UIInfoToSyncInfo();

		//character->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);
		Gamedata::PlayerInfo newSyncInfo;
		newSyncInfo.CopyFrom(*character->myData->GetSyncInfo());
		newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);
		character->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
	}

	BP_UI_EntityStorage_Drop->SetVisibility(ESlateVisibility::Hidden);
	changeCount = 0;
}

// 初始化数据
void UEntityStoragePanelUserWidget::InitData()
{

	if (isInitData)return;
	isInitData = true;


	auto playerSyncInfo = GetPlayerSyncInfo();
	if (playerSyncInfo == nullptr)return;

	// 获得 use_unique_mark
	{
		auto entity_storage = playerSyncInfo->mutable_entity_storage();
		auto storage_entity_size = entity_storage->storage_entity_list_size();
		for (int thisListIndex = 0; thisListIndex < storage_entity_size; thisListIndex++)
		{
			auto entitySyncInfo = entity_storage->mutable_storage_entity_list(thisListIndex);
			if (use_unique_mark < entitySyncInfo->unique_mark())
			{
				use_unique_mark = entitySyncInfo->unique_mark();
			}
		}
	}


	SyncInfoToUIInfo();
}

// 新增一行UI
void UEntityStoragePanelUserWidget::AddRow(int unique_mark, Gamedata::PlayerStorageEntity* entitySyncInfo)
{

	//auto addRowFunc = [&](int itemIndex, Gamedata::PlayerStorageEntity* entitySyncInfo) {

	if (class_BP_UI_EntityStorage_Item == nullptr)
	{
		return;
	}
	UEntityStorageItemUserWidget* pItem = Cast<UEntityStorageItemUserWidget>(
		CreateWidget(GetWorld(), class_BP_UI_EntityStorage_Item));
	if (pItem == nullptr)
	{
		return;
	}
	pItem->owner = this;
	pItem->unique_mark = unique_mark;
	pItem->name = UTF8_TO_TCHAR(entitySyncInfo->entity_name().c_str());
	pItem->entitySyncInfo.CopyFrom(*entitySyncInfo);
	ScrollBox_line->AddChild(pItem);
	itemList.Add(pItem);
	pItem->Start();
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [InitData] [itemIndex:%d]"), this, unique_mark);

}

// syncInfo->ui info
void UEntityStoragePanelUserWidget::SyncInfoToUIInfo()
{

	auto playerSyncInfo = GetPlayerSyncInfo();
	if (playerSyncInfo == nullptr)return;

	auto entity_storage = playerSyncInfo->mutable_entity_storage();
	auto storage_entity_list = entity_storage->mutable_storage_entity_list();
	auto storage_entity_size = entity_storage->storage_entity_list_size();
	if (storage_entity_size == 0)return;


	// 清空之前的数据
	itemList.Reset();
	auto childList = ScrollBox_line->GetAllChildren();
	for (auto node : childList)
	{
		ScrollBox_line->RemoveChild(node);
	}

	for (int thisListIndex = 0; thisListIndex < storage_entity_size; thisListIndex++)
	{
		auto entitySyncInfo = entity_storage->mutable_storage_entity_list(thisListIndex);
		AddRow(use_unique_mark, entitySyncInfo);
		use_unique_mark++;
	}
}

// ui info->syncInfo
void UEntityStoragePanelUserWidget::UIInfoToSyncInfo()
{

	auto playerSyncInfo = GetPlayerSyncInfo();
	if (playerSyncInfo == nullptr)return;
	auto entity_storage = playerSyncInfo->mutable_entity_storage();

	entity_storage->clear_storage_entity_list();

	auto childList = ScrollBox_line->GetAllChildren();
	for (auto node : childList)
	{
		auto& entitySyncInfo = Cast<UEntityStorageItemUserWidget>(node)->entitySyncInfo;
		auto storage_entity = entity_storage->add_storage_entity_list();
		storage_entity->CopyFrom(entitySyncInfo);
	}
}

// 打开UI设置选中节点
void UEntityStoragePanelUserWidget::OpenUISetSelect()
{

}

// 根据唯一标记找到child
UEntityStorageItemUserWidget* UEntityStoragePanelUserWidget::GetItem(int new_unique_mark)
{
	UEntityStorageItemUserWidget* item = nullptr;

	for (auto node : itemList)
	{
		if (node->unique_mark == new_unique_mark)
		{
			item = node.Get();
			break;
		}
	}
	return item;
}

// 玩家点击选中
void UEntityStoragePanelUserWidget::SetSelect(int new_unique_mark)
{


	if (old_unique_mark == new_unique_mark && old_unique_mark != 0)return;

	auto newItem = GetItem(new_unique_mark);
	auto oldItem = GetItem(old_unique_mark);
	if (oldItem)
	{

		oldItem->SetSelect(false, true);
	}
	if (newItem)
	{
		newItem->SetSelect(false, false);
		old_unique_mark = new_unique_mark;
	}
}

// UI回调 新增数据
void UEntityStoragePanelUserWidget::AddLineData()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [AddLineData] 新增  "), this);

	auto playerSyncInfo = GetPlayerSyncInfo();
	if (playerSyncInfo == nullptr)return;

	auto entityActor = GetSelectEntityActor();
	if (entityActor == nullptr)
	{
		UHUDStaticFunc::PushMessage(this, TEXT("没有选中实例"));
		UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI叮咚音效);
		return;
	}

	// 增加一行数据
	auto entity_storage = playerSyncInfo->mutable_entity_storage();
	auto storage_entity = entity_storage->add_storage_entity_list();
	auto storage_entity_size = entity_storage->storage_entity_list_size();
	storage_entity->mutable_entity_info()->CopyFrom(entityActor->syncInfo);
	std::string str(TCHAR_TO_UTF8(*entityActor->GetMyName()));
	storage_entity->set_entity_name(str);

	// 增加一行UI
	AddRow(use_unique_mark, storage_entity);
	use_unique_mark++;


	changeCount++;

	UHUDStaticFunc::PushMessage(this, TEXT("收纳成功"));
	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_ui_买);
}

// UI回调 移除数据
void UEntityStoragePanelUserWidget::RemoveLineData()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [RemoveLineData] 移除  "), this);

	auto playerSyncInfo = GetPlayerSyncInfo();
	if (playerSyncInfo == nullptr)return;

	auto oldItem = GetItem(old_unique_mark);
	if (oldItem)
	{

		auto entity_storage = playerSyncInfo->mutable_entity_storage();
		for (auto it = entity_storage->mutable_storage_entity_list()->begin(); it != entity_storage->mutable_storage_entity_list()->end(); ++it)
		{
			if (it->unique_mark() == old_unique_mark)
			{

				entity_storage->mutable_storage_entity_list()->erase(it);
				break;
			}
		}

		itemList.Remove(TWeakObjectPtr<UEntityStorageItemUserWidget>(oldItem));
		ScrollBox_line->RemoveChild(oldItem);

		changeCount++;

		UHUDStaticFunc::PushMessage(this, TEXT("移除成功"));
		UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI_解锁);
	}
	else
	{
		UHUDStaticFunc::PushMessage(this, TEXT("没有选中行"));
		UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI叮咚音效);
	}

}

// 更变勾选
void UEntityStoragePanelUserWidget::MyOnCheckStateChanged(bool bIsChecked)
{
	if (bIsChecked)
	{
		Image_stop->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_checkStr->SetText(FText::FromString(TEXT("编辑该面板")));
	}
	else
	{
		// 移除掉老的选中状态.
		SetSelect(0);
		Image_stop->SetVisibility(ESlateVisibility::Visible);
		TextBlock_checkStr->SetText(FText::FromString(TEXT("拖拽到场景")));
	}
}

// 切换编辑实例和拖拽姿态
void UEntityStoragePanelUserWidget::ChangeEditEntityOrPreview(bool isEdit)
{
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (isEdit)
	{
		character->inputComp->inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
	}
	else
	{
		character->inputComp->inputMode = ECharacterStatusType::CharacterStatusType_Preview;
	}
}

// 