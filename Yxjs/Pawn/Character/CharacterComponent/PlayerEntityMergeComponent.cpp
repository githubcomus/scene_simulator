// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerEntityMergeComponent.h"
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
#include "UI/BaseHUDInfo.h"

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
UCharacterDataComponent* UPlayerEntityMergeComponent::GetDataComponent()
{
	if (view.IsValid() == false)return nullptr;
	return view->myData;
}

//
UPlayerSelectedComponent* UPlayerEntityMergeComponent::GetSelectedComponent()
{
	if (view.IsValid() == false)return nullptr;
	return view->selectComponent;
}

// ?????? ????????????view
AEditEntityManage* UPlayerEntityMergeComponent::GetEditEntityManage()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

// ?????? ????????????view
AOverallSituationView* UPlayerEntityMergeComponent::GetOverallSituationView()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->overallSituationView;
}

// ?????? ??????????????????
xlsdata::MaterialTableDefine* UPlayerEntityMergeComponent::GetXlsInfoWithXlsId(int32 xlsId)
{
	// ??????:?????????????????????string???????????????,??????????????????,???????????????

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance_->zwXlsDataActorComponent->materialMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->materialMap.end())
	{
		return &gameInstance_->zwXlsDataActorComponent->materialMap[xlsId];
	}
	return nullptr;
}

// ?????? ??????????????????
void UPlayerEntityMergeComponent::GetSelectInstanceList(std::vector<int>& selectList)
{
	auto baseCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (baseCharacter == nullptr)return;
	baseCharacter->selectComponent->GetSelectInstanceList(selectList);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
UPlayerEntityMergeComponent::UPlayerEntityMergeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPlayerEntityMergeComponent::BeginPlay()
{
	Super::BeginPlay();
}

// EndPlay
void UPlayerEntityMergeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// START
void UPlayerEntityMergeComponent::Start()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEntityMergeComponent::Start] "), this);
	view = Cast<ABaseCharacter>(GetOwner());
	//InitBackpackData();

	// ?????????????????????????????????????????????UI
	auto& overallSituationView = view->myData->gameMode->overallSituationView;
	auto actor_list = overallSituationView->syncInfo.mutable_data()->mutable_actor_list();
	for (auto it = actor_list->begin(); it != actor_list->end(); ++it)
	{
		UI_MergeActor_AddLine(&it->second);
	}
}

// END
void UPlayerEntityMergeComponent::End()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEntityMergeComponent::End]   "), this);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// UI?????? ??????UI?????????
void UPlayerEntityMergeComponent::UI_InitUI()
{
}

// UI?????? ????????????
void UPlayerEntityMergeComponent::UICallback_EntityMerge()
{
	auto placeView = GetEditEntityManage();
	if (placeView == nullptr)return;
	auto overallSituationView = GetOverallSituationView();
	if (overallSituationView == nullptr)return;

	// ?????????????????????????????????
	std::vector<int> selectList;
	GetSelectInstanceList(selectList);

	if (selectList.size() <= 1)
	{
		return;
	}

	// ??????????????????
	std::vector<Gamedata::EntityInfo*> syncInfoList;
	for (auto node : selectList)
	{
		auto syncInfo = placeView->data->GetSyncInfo(node);
		syncInfoList.push_back(syncInfo);
	}

	// ??????????????????
	overallSituationView->NetCall_Place_Merge_AddActor(syncInfoList);

	// ?????????????????????????????????
	{
		auto dataComp = GetDataComponent();
		if (dataComp->GetInputMode() == ECharacterStatusType::CharacterStatusType_EditEntity)
		{
			GetSelectedComponent()->OperationResultsSwitch(UPlayerSelectedComponent::Operation_Type::Operation_Type_Delete);
		}
	}
}

// UI?????? ????????????
void UPlayerEntityMergeComponent::UI_MergeActor_AddLine(Gamedata::EntityMeshMasterInfo* info)
{
	//CheckDefaultData();

	auto ui = view->myData->hud->GetChildUI(EUIChildName::ui_name_entity_merge);
	if (ui == nullptr)return;

	FString name = TEXT("?????????");
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