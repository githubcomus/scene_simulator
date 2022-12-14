// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Core/BaseGameInstance.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshComponent.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshView.h"
#include "UI/BaseHud.h"

#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
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
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Tools/CommonlyObject.h"
#include "../OverallSituation/OverallSituationView.h"

//#include<vector>

/*
data
*/

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// UEditEntityManageData
UEditEntityManageData::UEditEntityManageData()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// BeginPlay
void UEditEntityManageData::Start(TWeakObjectPtr<UEditEntityManageController> controller_, TWeakObjectPtr<AEditEntityManage> view_)
{
	controller = controller_;
	view = view_;
	CheckDefaultData();
	LoadSelectMaterial();
}

// EndPlay
void UEditEntityManageData::End()
{
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// ?????????????????????
bool UEditEntityManageData::CheckDefaultData()
{
	bool checkTwice = false;
	if (gameInstance.IsValid() == false || gameMode.IsValid() == false
		|| playerController.IsValid() == false || hud.IsValid() == false)
	{
		gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController.IsValid())hud = Cast<ABaseHUD>(playerController->GetHUD());

		checkTwice = true;
	}

	bool flag = true;
	if (checkTwice)
	{
		if (gameInstance.IsValid() == false || gameMode.IsValid() == false
			|| playerController.IsValid() == false || hud.IsValid() == false)
		{
			flag = false;
		}
	}

	return flag;
}

// ?????? ??????????????????
bool UEditEntityManageData::IsExistData(int32 entityId)
{
	auto actor = GetEntityActor(entityId);
	if (actor.IsValid())
	{
		return true;
	}
	return false;
}

// ?????? ??????????????????
Gamedata::EntityInfo* UEditEntityManageData::GetSyncInfo(int32 entityId)
{
	auto actor = GetEntityActor(entityId);
	if (actor.IsValid())
	{
		return &actor->syncInfo;
	}
	return nullptr;
}

// ?????? ????????????????????????
bool UEditEntityManageData::IsExistXlsInfo(int32 xlsId)
{
	// ??????:?????????????????????string???????????????,??????????????????,???????????????

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());

	if (gameInstance_->zwXlsDataActorComponent->placeMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->placeMap.end())
	{
		return true;
	}
	return false;
}

// ?????? ??????????????????
xlsdata::PlaceTableDefine* UEditEntityManageData::GetXlsInfo(int32 entityId)
{
	// ??????:?????????????????????string???????????????,??????????????????,???????????????

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());

	auto actor = GetEntityActor(entityId);
	if (actor.IsValid())
	{
		return actor->xlsInfo;
	}
	return nullptr;
}

// ?????? ??????????????????
xlsdata::PlaceTableDefine* UEditEntityManageData::GetXlsInfoWithXlsId(int xlsId)
{
	// ??????:?????????????????????string???????????????,??????????????????,???????????????

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance_->zwXlsDataActorComponent->placeMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->placeMap.end())
	{
		return &gameInstance_->zwXlsDataActorComponent->placeMap[xlsId];
	}
	return nullptr;
}

// ?????? ?????????????????????????????????
bool UEditEntityManageData::GetEntityIdWithBox(UBoxComponent* box, int32& entityId, int32& index)
{
	if (collisionBoxMap.Find(box))
	{
		entityId = collisionBoxMap[box].entityId;
		index = collisionBoxMap[box].index;
		return true;
	}
	return false;
}

// ?????? ??????????????????
xlsdata::MaterialTableDefine* UEditEntityManageData::GetMaterialInfoWithId(int32 xlsId)
{
	// ??????:?????????????????????string???????????????,??????????????????,???????????????

	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance_->zwXlsDataActorComponent->materialMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->materialMap.end())
	{
		return &gameInstance_->zwXlsDataActorComponent->materialMap[xlsId];
	}
	return nullptr;
}

// ?????? ????????????Id
int UEditEntityManageData::GetLocalPlayerId()
{
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(view->GetWorld(), 0)->GetPawn());
	if (baseCharacter)
	{
		auto localPlayerId = baseCharacter->myData->playerId;
		return localPlayerId;
	}
	else
	{
		return 0;
	}
}

// ?????? ?????????????????????
TWeakObjectPtr<USceneStaticMeshComponent> UEditEntityManageData::GetEntityMeshComponent(int actorMapId)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (baseGameMode && baseGameMode->sceneStaticMeshView)
	{
		return baseGameMode->sceneStaticMeshView->GetEntityMeshComponent(actorMapId);
	}

	return TWeakObjectPtr<USceneStaticMeshComponent>();
}

// ???????????? ??????????????????
void UEditEntityManageData::LoadSelectMaterial()
{
	FString path = TEXT(
		"MaterialInstanceConstant'/Game/Resources/Material/Selected/Outline/MI_Outline_Inst.MI_Outline_Inst'");
	selectMaterial = LoadObject<UMaterialInstanceConstant>(NULL, *path);
}

// ?????? actor
TWeakObjectPtr<AEditEntityActor> UEditEntityManageData::GetEntityActor(int32 entityId)
{
	if (view->actorMap.Find(entityId))
	{
		return view->actorMap[entityId];
	}
	return TWeakObjectPtr<AEditEntityActor>();
}

// ?????? ????????????
Gamedata::EntityMeshMasterInfo* UEditEntityManageData::GetMergeInfoWithXlsId(int xlsId)
{
	if (gameMode.IsValid() == false)return nullptr;
	auto& syncInfo = gameMode->overallSituationView->syncInfo;

	auto it = syncInfo.mutable_data()->mutable_actor_list()->find(xlsId);
	if (it != syncInfo.mutable_data()->mutable_actor_list()->end())
	{
		return &(it->second);
	}
	return nullptr;
}

// ?????? ???????????????????????????
EMyMeshMaterialType UEditEntityManageData::GetChildMaterialType(Gamedata::EntityInfo* syncInfo, int childIndex)
{

	//Gamedata::EntityInfo* syncInfo = GetSyncInfo(entityId);
	//if (syncInfo == nullptr) {
	//	return EMyMeshMaterialType::EMyMeshMaterialType_WhiteMold;
	//}
	if (syncInfo == nullptr)
		return EMyMeshMaterialType::EMyMeshMaterialType_WhiteMold;

	if (syncInfo->child_list_size() <= childIndex)
	{
		return EMyMeshMaterialType::EMyMeshMaterialType_WhiteMold;
	}

	Gamedata::EntityChildInfo* childSyncInfo = syncInfo->mutable_child_list(childIndex);


	bool staticFlag = false;

	// ????????????
	if (syncInfo->has_transform_timeline() && syncInfo->transform_timeline().point_map_size() > 0)
	{

		// ??????????????????
		if (syncInfo->transform_timeline().timeline_highlight() == Gamedata::TimelineHighlightType::TimelineHighlightType_color)
		{
			return EMyMeshMaterialType::EMyMeshMaterialType_DynamicNormalColor;
		}
		// ??????????????????
		else if (syncInfo->transform_timeline().timeline_highlight() == Gamedata::TimelineHighlightType::TimelineHighlightType_hightlight)
		{
			return EMyMeshMaterialType::EMyMeshMaterialType_DynamicHighhight;
		}
		// ????????????
		else if (syncInfo->transform_timeline().timeline_highlight() == Gamedata::TimelineHighlightType::TimelineHighlightType_off)
		{
			staticFlag = true;
		}
	}
	// ????????????
	else
	{
		staticFlag = true;
	}


	// ????????????
	if (staticFlag)
	{
		// ??????
		if (childSyncInfo->mesh().material_id() == 0)
		{
			return EMyMeshMaterialType::EMyMeshMaterialType_WhiteMold;
		}
		// ???????????????
		else if (childSyncInfo->mesh().material_id() == 1000)
		{

			// ??????
			if (childSyncInfo->mesh().highlight())
			{
				return EMyMeshMaterialType::EMyMeshMaterialType_StaticHighhight;
			}
			// ??????
			else
			{
				return EMyMeshMaterialType::EMyMeshMaterialType_StaticNormalColor;
			}
		}
		// ????????????
		else
		{
			return EMyMeshMaterialType::EMyMeshMaterialType_OrdinaryMaterial;
		}
	}


	return EMyMeshMaterialType::EMyMeshMaterialType_WhiteMold;
}

// ?????? ??????????????????
FString UEditEntityManageData::GetHightlightMaterialPath(bool highlight)
{
	FString path = TEXT("");
	if (highlight)
	{
		path = TEXT("Material'/Game/Resources/Material/CustomColor/M_CustomColor_Highlight_Inst.M_CustomColor_Highlight_Inst'");
	}
	else
	{
		path = TEXT("Material'/Game/Resources/Material/CustomColor/M_CustomColor_Base_Inst.M_CustomColor_Base_Inst'");
	}
	return path;
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/
