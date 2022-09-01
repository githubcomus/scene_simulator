// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityMaterialPanelUserWidget.h"

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
#include <Components/UniformGridPanel.h>
#include "EntityMaterialItemUserWidget.h"
#include "../../../../Core/BaseGameInstance.h"
#include "EntityMaterial_Color.h"
#include "Yxjs/UI/HUDStaticFunc.h"
#include "Yxjs/Core/GameInstanceStaticFunc.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityChildComponent.h"

/*

*/


/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 外部数据 获取 动态获取view
TWeakObjectPtr<AEditEntityManage> UEntityMaterialPanelUserWidget::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 外部数据 获取实例
TWeakObjectPtr<AEditEntityActor> UEntityMaterialPanelUserWidget::GetEntityAcotr(int entityId)
{
	auto view = GetEditEntityManage();
	if (view == nullptr)return nullptr;

	return view->data->GetEntityActor(entityId);
}

// 获取当前选中的actor
TWeakObjectPtr<AEditEntityActor> UEntityMaterialPanelUserWidget::GetSelectEntityActor()
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

// 读取xls内的数据
std::map<int32, xlsdata::MaterialTableDefine>& UEntityMaterialPanelUserWidget::GetXlsData()
{

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	return gameInstance_->zwXlsDataActorComponent->materialMap;
}

// 读取指定资源名字,并且拼凑成iocn路径
FString UEntityMaterialPanelUserWidget::GetIconPath(std::string materialPath)
{

	FString tempPath = UTF8_TO_TCHAR(materialPath.c_str());
	FString name = FPaths::GetBaseFilename(tempPath, true);
	FString path = TEXT("Texture2D'/Game/Resources/Icon/Material/") + name + TEXT(".") + name + TEXT("'");
	UE_LOG(LogTemp, Log, TEXT("[%x] [ReadMeshData] [path:%s]  [name:%s]"), this, *path, *name);
	return path;
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UEntityMaterialPanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityMaterialPanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityMaterialPanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();


}

//
FReply UEntityMaterialPanelUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
FReply UEntityMaterialPanelUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UEntityMaterialPanelUserWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

//
void UEntityMaterialPanelUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEntityMaterialPanelUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UEntityMaterialPanelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
void UEntityMaterialPanelUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	FString bpPath = TEXT(
		"WidgetBlueprint'/Game/Yxjs/UI/EditEntity/EntityMaterial/BP_UI_EntityMaterial_Item.BP_UI_EntityMaterial_Item_C'");
	class_BP_UI_EntityMaterial_Item = LoadClass<UEntityMaterialItemUserWidget>(this, *bpPath);
	if (class_BP_UI_EntityMaterial_Item == nullptr)
	{
		return;
	}

	Uniform_Grid_Panel_ = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("Uniform_Grid_Panel_")));
	BP_UI_EntityMaterial_Color = Cast<UEntityMaterial_Color>(GetWidgetFromName(TEXT("BP_UI_EntityMaterial_Color")));

}

// UI调用 展开收起事件
void UEntityMaterialPanelUserWidget::OnExpandCollapse(bool isShow)
{


	FString str = isShow ? TEXT("展开") : TEXT("折叠");
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例材质] [OnExpandCollapse] [%s]"), this, *str);


	if (isShow)
	{
		OpenUI();
	}
	else
	{
		CloseUI();
	}

}

// UI调用 展开
void UEntityMaterialPanelUserWidget::OpenUI()
{


	if (UFunction* func = FindFunction("Event_OpenUI"))
	{
		struct TempParam
		{
		};
		TempParam tempParam;

		ProcessEvent(func, &tempParam);
	}

	InitData();

	OpenUISetSelect();
}

// UI回调 收起
void UEntityMaterialPanelUserWidget::CloseUI()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	if (character->commandComponent == nullptr)return;

	auto actor = GetSelectEntityActor();
	if (actor == nullptr)return;

}

// 初始化数据
void UEntityMaterialPanelUserWidget::InitData()
{

	if (isInitData)return;
	isInitData = true;

	auto& xlsMap = GetXlsData();
	std::vector<xlsdata::MaterialTableDefine*> xlsList;
	for (auto node = xlsMap.begin(); node != xlsMap.end(); ++node)
	{
		xlsList.push_back(&node->second);
	}

	auto addRowFunc = [&](int thisRowIndex, int thisListIndex, int itemIndex, xlsdata::MaterialTableDefine* xlsInfo) {

		if (class_BP_UI_EntityMaterial_Item == nullptr)return;
		UEntityMaterialItemUserWidget* child = Cast<UEntityMaterialItemUserWidget>(
			CreateWidget(GetWorld(), class_BP_UI_EntityMaterial_Item));
		if (child == nullptr)return;
		child->owner = this;
		child->itemIndex = itemIndex;
		child->name = UTF8_TO_TCHAR(xlsInfo->Name_.c_str());
		child->iconPath = GetIconPath(xlsInfo->MaterialPath_);  //UTF8_TO_TCHAR(xlsInfo->IconPath_.c_str());
		child->xlsId = xlsInfo->ID_;
		Uniform_Grid_Panel_->AddChildToUniformGrid(child, thisRowIndex, thisListIndex);
		itemList.Add(child);
		child->Start();
		UE_LOG(LogTemp, Log, TEXT("[%x] [实例材质] [InitData] [thisRowIndex:%d] [thisListIndex:%d] [itemIndex:%d]"), this, thisRowIndex, thisListIndex, itemIndex);
	};

	// 清空之前的数据
	itemList.Reset();
	auto childList = Uniform_Grid_Panel_->GetAllChildren();
	for (auto node : childList)
	{
		Uniform_Grid_Panel_->RemoveChild(node);
	}

	int maxListCount = 6;	// 每行最大字段数量
	int maxItemCount = xlsMap.size();	// 总共的节点数量

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

// 打开UI设置选中节点
void UEntityMaterialPanelUserWidget::OpenUISetSelect()
{

	auto entityActor = GetSelectEntityActor();
	if (entityActor == nullptr)return;
	auto material_id = entityActor->syncInfo.child_list(0).mesh().material_id();

	auto& xlsMap = GetXlsData();
	std::vector<xlsdata::MaterialTableDefine*> xlsList;
	for (auto node = xlsMap.begin(); node != xlsMap.end(); ++node)
	{
		xlsList.push_back(&node->second);
	}

	for (int i = 0; i < xlsList.size(); i++)
	{
		auto node = xlsList[i];
		if (node->ID_ == material_id)
		{
			SetSelect(i, true);
			break;
		}
	}
}

// 初始化设置选中item / 玩家点击选中切换材质
void UEntityMaterialPanelUserWidget::SetSelect(int newSelectIndex, bool isInit_)
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UEditEntityManageController* placeController = baseGameMode->editEntityManage->controller;
	if (placeController == nullptr)return;

	if (BP_UI_EntityMaterial_Color.IsValid() == false)return;

	TWeakObjectPtr<AEditEntityActor>  entityActor = GetSelectEntityActor();
	if (entityActor == nullptr)
	{

		UHUDStaticFunc::PushMessage(this, TEXT("没有选中实例"));
		UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI叮咚音效);
		return;
	}

	// 选中实例不支持材质切换
	UEditEntityChildComponent* child = entityActor->childComponentList[0];
	if (child->childMeshXlsInfo->UseMaterial_ == false)
	{

		UHUDStaticFunc::PushMessage(this, TEXT("非基础网格,不支持自定义材质"));
		UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI叮咚音效);
		return;
	}

	if (oldSelectIndex < itemList.Num() && oldSelectIndex != -1)
	{
		itemList[oldSelectIndex]->SetSelect(false, true);
	}

	if (newSelectIndex < itemList.Num())
	{
		itemList[newSelectIndex]->SetSelect(false, false);
		oldSelectIndex = newSelectIndex;

		if (itemList[newSelectIndex]->xlsId == 1000)
		{
			BP_UI_EntityMaterial_Color->SetVisibility(ESlateVisibility::Visible);
			BP_UI_EntityMaterial_Color->Reset(this, entityActor);
		}
		else
		{
			BP_UI_EntityMaterial_Color->SetVisibility(ESlateVisibility::Hidden);
		}

		if (isInit_ == false)
		{

			entityActor->syncInfo.mutable_child_list(0)->mutable_mesh()->set_material_id(itemList[newSelectIndex]->xlsId);

			TArray<int32> entityIdList;
			entityIdList.Add(entityActor->entityId);
			placeController->NetCall_UpdateData(entityIdList);

			UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_ui_买);
			UHUDStaticFunc::PushMessage(this, FString::Printf(TEXT("设置材质(%d)"), itemList[newSelectIndex]->xlsId));
		}
	}
}

// 自定义颜色点击应用
void UEntityMaterialPanelUserWidget::SetChangeColor(FVector color, bool highlight)
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UEditEntityManageController* placeController = baseGameMode->editEntityManage->controller;
	if (placeController == nullptr)return;

	TWeakObjectPtr<AEditEntityActor>  entityActor = GetSelectEntityActor();
	if (entityActor == nullptr)
	{

		UHUDStaticFunc::PushMessage(this, TEXT("没有选中实例"));
		UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_UI叮咚音效);
		return;
	}

	Gamedata::EntityChildInfo* child = entityActor->syncInfo.mutable_child_list(0);

	child->mutable_mesh()->set_highlight(highlight);
	Gamedata::FVector* mesh_color = child->mutable_mesh()->mutable_color();
	mesh_color->set_x(color.X);
	mesh_color->set_y(color.Y);
	mesh_color->set_z(color.Z);

	TArray<int32> entityIdList;
	entityIdList.Add(entityActor->entityId);
	placeController->NetCall_UpdateData(entityIdList);

	UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_ui_买);
	FString str = highlight ? TEXT("高光") : TEXT("正常");
	UHUDStaticFunc::PushMessage(this, FString::Printf(TEXT("设置%s颜色(%s)"), *str, *color.ToString()));
}
