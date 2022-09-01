// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerEntityStorageComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerPlaceComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerMaterialComponent.h"
#include "Core/BaseGameInstance.h"
#include "Tools/CalculationObject.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "Net/NetMainComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "UI/BaseHud.h"
#include "OnlineResources/OnlineResourcesComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Yxjs/FunctionalModule/OverallSituation/OverallSituationView.h"

#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include <math.h>

/*----------------------------------------------------*/
/*----------------------------------------------------*/

//
UCharacterDataComponent* UPlayerEntityStorageComponent::GetDataComponent()
{
	if (view.IsValid() == false)return nullptr;
	return view->myData;
}

//
UPlayerSelectedComponent* UPlayerEntityStorageComponent::GetSelectedComponent()
{
	if (view.IsValid() == false)return nullptr;
	return view->selectComponent;
}

// 获取 动态获取view
AEditEntityManage* UPlayerEntityStorageComponent::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// 获取 动态获取view
AOverallSituationView* UPlayerEntityStorageComponent::GetOverallSituationView()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->overallSituationView;
}

// 获取 获取表格数据
xlsdata::MaterialTableDefine* UPlayerEntityStorageComponent::GetXlsInfoWithXlsId(int32 xlsId)
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
void UPlayerEntityStorageComponent::GetSelectInstanceList(std::vector<int>& selectList)
{
	auto baseCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (baseCharacter == nullptr)return;
	baseCharacter->selectComponent->GetSelectInstanceList(selectList);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
UPlayerEntityStorageComponent::UPlayerEntityStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPlayerEntityStorageComponent::BeginPlay()
{
	Super::BeginPlay();
}

// EndPlay
void UPlayerEntityStorageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// START
void UPlayerEntityStorageComponent::Start()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEntityStorageComponent::Start] "), this);
	view = Cast<ABaseCharacter>(GetOwner());
	//InitBackpackData();

	// 从数据缓存中心重新初始化数据到UI
	//auto& overallSituationView = view->myData->gameMode->overallSituationView;
	//auto actor_list = overallSituationView->syncInfo.mutable_data()->mutable_actor_list();
	//for (auto it = actor_list->begin(); it != actor_list->end(); ++it) {
	//	UI_MergeActor_AddLine(&it->second);
	//}
}

// END
void UPlayerEntityStorageComponent::End()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEntityStorageComponent::End]   "), this);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// UI调用 打开UI初始化
void UPlayerEntityStorageComponent::UI_InitUI()
{
}

// UI回调 合并实例
void UPlayerEntityStorageComponent::UICallback_EntityMerge()
{
	auto placeView = GetEditEntityManage();
	if (placeView == nullptr)return;
	auto overallSituationView = GetOverallSituationView();
	if (overallSituationView == nullptr)return;

	// 所有选中的都是实例验证
	std::vector<int> selectList;
	GetSelectInstanceList(selectList);

	if (selectList.size() <= 1)
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

	// 发送合并请求
	overallSituationView->NetCall_Place_Merge_AddActor(syncInfoList);

	// 编辑模式下删除选中数据
	{
		auto dataComp = GetDataComponent();
		if (dataComp->GetInputMode() == ECharacterStatusType::CharacterStatusType_EditEntity)
		{
			GetSelectedComponent()->OperationResultsSwitch(UPlayerSelectedComponent::Operation_Type::Operation_Type_Delete);
		}
	}
}

// UI调用 新增数据
void UPlayerEntityStorageComponent::UI_MergeActor_AddLine(Gamedata::EntityMeshMasterInfo* info)
{
	//CheckDefaultData();

	auto ui = view->myData->hud->GetChildUI(EUIChildName::ui_name_entity_merge);
	if (ui == nullptr)return;

	FString name = TEXT("未命名");
	if (info->name().length() != 0)
	{
		name = UTF8_TO_TCHAR(info->name().c_str());
	}

	FString cmd = FString::Printf(TEXT("Event_MergeActor_AddLine %d %s"),
		info->xls_id(), *name);
	FOutputDeviceDebug device;
	ui->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/