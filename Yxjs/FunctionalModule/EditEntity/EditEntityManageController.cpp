// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityManageController.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewImageComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewMeshComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewCollisionComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "Tools/CalculationObject.h"
#include "Core/BaseGameInstance.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Core/BaseGameInstance.h"
#include "Net/NetMainComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewLaserBeamComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerMoveTimelineObject.h"
#include "Tools/CommonlyObject.h"
#include "Components/Image.h"
#include "Net/NetRingBuffer.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerScenesTimelineObject.h"

#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Actor/EditEntityActor.h"
#include "Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "Actor/ControllerComponent/EditEntityEditComponent.h"
#include "Actor/ControllerComponent/EditEntityCommandComponent.h"

/*
controller
*/


/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

// ??????????????????????????????
bool UEditEntityManageController::IsStaticColorChange(Gamedata::EntityInfo* oldSyncInfo, Gamedata::EntityInfo* newSyncInfo)
{

	EMyMeshMaterialType newType = data->GetChildMaterialType(newSyncInfo, 0);
	if (
		newType != EMyMeshMaterialType::EMyMeshMaterialType_StaticNormalColor
		&& newType != EMyMeshMaterialType::EMyMeshMaterialType_StaticHighhight
		)
	{
		return false;
	}

	bool old_highlight = oldSyncInfo->mutable_child_list(0)->mutable_mesh()->highlight();
	Gamedata::FVector old_color = oldSyncInfo->mutable_child_list(0)->mutable_mesh()->color();
	bool new_highlight = newSyncInfo->mutable_child_list(0)->mutable_mesh()->highlight();
	Gamedata::FVector new_color = newSyncInfo->mutable_child_list(0)->mutable_mesh()->color();
	if (old_highlight != new_highlight)
	{
		return true;
	}
	else if (old_color.x() != new_color.x()
		|| old_color.y() != new_color.y()
		|| old_color.z() != new_color.z()
		)
	{

		return true;
	}

	return false;
}

// ??????/??????????????????
bool UEditEntityManageController::IsMaterialTypeChange(Gamedata::EntityInfo* oldSyncInfo, Gamedata::EntityInfo* newSyncInfo)
{

	EMyMeshMaterialType oldType = data->GetChildMaterialType(oldSyncInfo, 0);
	EMyMeshMaterialType newType = data->GetChildMaterialType(newSyncInfo, 0);

	if (oldType != newType)
	{
		return true;
	}

	return false;
}

// ??????/??????????????????
bool UEditEntityManageController::IsMaterialIdChange(Gamedata::EntityInfo* oldSyncInfo, Gamedata::EntityInfo* newSyncInfo)
{

	EMyMeshMaterialType oldType = data->GetChildMaterialType(oldSyncInfo, 0);
	EMyMeshMaterialType newType = data->GetChildMaterialType(newSyncInfo, 0);

	if (
		(oldType == EMyMeshMaterialType::EMyMeshMaterialType_WhiteMold
			|| oldType == EMyMeshMaterialType::EMyMeshMaterialType_OrdinaryMaterial)
		&& (newType == EMyMeshMaterialType::EMyMeshMaterialType_WhiteMold
			|| newType == EMyMeshMaterialType::EMyMeshMaterialType_OrdinaryMaterial)
		)
	{
		if (oldSyncInfo->child_list(0).mesh().material_id() != newSyncInfo->child_list(0).mesh().material_id())
		{
			return true;
		}
	}

	return false;
}

/*/////////////////////////////////////////////////////////////*/
// ??????
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// ?????????
/*/////////////////////////////////////////////////////////////*/

//
UEditEntityManageController::UEditEntityManageController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// START
void UEditEntityManageController::Start(TWeakObjectPtr<UEditEntityManageData> data_, TWeakObjectPtr<AEditEntityManage> view_)
{
	data = data_;
	view = view_;

	moveTimelineController = NewObject<UPlaceControllerMoveTimelineObject>();
	moveTimelineController->Start(this);
	scenesTimelineController = NewObject<UPlaceControllerScenesTimelineObject>();
	scenesTimelineController->Start(this);
}

// END
void UEditEntityManageController::End()
{
	moveTimelineController->End();
	moveTimelineController = nullptr;
	scenesTimelineController->End();
	scenesTimelineController = nullptr;
}

// Called every frame
void UEditEntityManageController::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if (moveTimelineController)
	//{
	//	moveTimelineController->TickComponent(DeltaTime);
	//}
	//if (scenesTimelineController)
	//{
	//	scenesTimelineController->TickComponent(DeltaTime);
	//}

	////
	//auto nowTime = GetWorld()->TimeSeconds;

	//// ??????30???
	//// ??????60???
	////auto frame = 1.0/ DeltaTime;
	////(float)frame/30.0;
	//auto count = 0;
	//if (lastTimeTick + waitTime < nowTime) {
	//	lastTimeTick = nowTime;
	//	for (auto node : timelineTickActorMap) {
	//		auto& actor = node.Value;
	//		if (actor.IsValid()) {
	//			actor->myTimelineComponent->FunctionTick(nowTime);
	//			count++;

	//			if (count > 2000) {
	//				break;
	//			}
	//		}
	//	}
	//}
}

/*----------------------------------------------------*/

// ?????? ?????? ??????
void UEditEntityManageController::NetCall_NewData(Gamedata::EntityInfo syncInfo)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	auto gameInstance = baseGameMode->gameInstance;


	Gamedata::EntityAddRequest request;
	request.set_playerid(data->GetLocalPlayerId());
	Gamedata::EntityInfo* entityInfo = request.add_entityinfolist();
	entityInfo->CopyFrom(syncInfo);

	std::string sendStr = request.SerializePartialAsString();
	char* sendData = (char*)sendStr.c_str();
	int sendSize = sendStr.length();
	int type = (int)Gamedata::MsgType::PB_EntityAddRequest;
	gameInstance->netMainComponent->NetCall_GameData(type, sendData, sendSize);

}

// ?????? ?????? ??????
void UEditEntityManageController::NetCall_NewData(TArray<int32> entityIdList)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	auto gameInstance = baseGameMode->gameInstance;

	Gamedata::EntityAddRequest request;
	request.set_playerid(data->GetLocalPlayerId());
	request.set_playerid(data->GetLocalPlayerId());

	for (auto node : entityIdList)
	{
		auto syncInfo = data->GetSyncInfo(node);
		Gamedata::EntityInfo* entityInfo = request.add_entityinfolist();
		entityInfo->CopyFrom(*syncInfo);
	}


	std::string sendStr = request.SerializePartialAsString();
	char* sendData = (char*)sendStr.c_str();
	int sendSize = sendStr.length();
	int type = (int)Gamedata::MsgType::PB_EntityAddRequest;
	gameInstance->netMainComponent->NetCall_GameData(type, sendData, sendSize);
}

// ?????? ?????? 
void UEditEntityManageController::NetCallback_EntityAdd(Gamedata::EntityInfo* syncInfo_)
{
	if (data->IsExistXlsInfo(syncInfo_->child_list(0).mesh().xls_id()) == false)
	{
		return;
	}

	// ??????????????????
	if (data->IsExistData(syncInfo_->entityid()))
	{
		NetCallback_EntityRemove(syncInfo_->entityid());
	}

	// ????????????
	view->EntityAdd(syncInfo_);
}

// ?????? ?????? 
void UEditEntityManageController::NetCall_UpdateData(TArray<int32> entityIdList)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	auto gameInstance = baseGameMode->gameInstance;

	if (entityIdList.Num() == 0)return;

	Gamedata::EntityUpdateRequest request;

	request.set_playerid(data->GetLocalPlayerId());

	for (auto node : entityIdList)
	{
		if (data->IsExistData(node) == false)
		{
			continue;;
		}
		auto syncInfo = data->GetSyncInfo(node);
		Gamedata::EntityInfo* entityInfo = request.add_entityinfolist();
		entityInfo->CopyFrom(*syncInfo);
	}

	std::string sendStr = request.SerializePartialAsString();
	char* sendData = (char*)sendStr.c_str();
	int sendSize = sendStr.length();
	int type = (int)Gamedata::MsgType::PB_EntityUpdateRequest;
	gameInstance->netMainComponent->NetCall_GameData(type, sendData, sendSize);
}

// ?????? ?????? 
void UEditEntityManageController::NetCallback_EntityUpdate(Gamedata::EntityInfo* syncInfo_)
{
	if (data->IsExistData(syncInfo_->entityid()) == false)
	{
		return;
	}
	auto actor = data->GetEntityActor(syncInfo_->entityid());
	auto syncInfo = data->GetSyncInfo(syncInfo_->entityid());
	auto observerInfo = &actor->observerSyncInfo;
	UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [EntityUpdate] ??????  [entityId:%d]  [materialId:%d---%d] [collision:%d]"),
		this, syncInfo->entityid(), syncInfo->child_list(0).mesh().xls_id(), syncInfo_->child_list(0).mesh().material_id(), syncInfo_->collision_type());

	// ?????? ????????????
	if (syncInfo_->collision_type() != observerInfo->collision_type())
	{
		{
			actor->EntitySetNewSyncInfo(syncInfo_);
			actor->ReplaceMesh();
		}
	}
	// ?????? ????????????????????????
	else if (IsMaterialTypeChange(observerInfo, syncInfo_))
	{
		actor->EntitySetNewSyncInfo(syncInfo_);
		actor->ReplaceMesh();
	}
	// ?????? ????????????????????????
	else if (IsMaterialIdChange(observerInfo, syncInfo_))
	{
		actor->EntitySetNewSyncInfo(syncInfo_);
		actor->ReplaceMesh();
	}
	// ?????? ???????????? ????????????
	else if (IsStaticColorChange(observerInfo, syncInfo_))
	{
		actor->EntitySetNewSyncInfo(syncInfo_);
		actor->ReplaceMesh();
	}
	// ?????? ????????????
	else
	{
		// ??????????????????
		bool isTimelineUpdate = actor->myTimelineComponent->IsTimelineUpdate(syncInfo_);
		actor->EntitySetNewSyncInfo(syncInfo_);

		// ???????????????
		{
			actor->UpdateChildMesh();		// 
		}

		// ???????????????
		{
			if (isTimelineUpdate)
			{
				actor->myTimelineComponent->UpdateTimeline(); // ?????????
			}
		}

		// ???????????? 
		{

			if (actor->IsTimelineAuto())
			{
				actor->myTimelineComponent->RegisterTick(true);
			}
			else
			{
				actor->myTimelineComponent->RegisterTick(false);
			}

			actor->commandComponent->UpdateToMaster(); // 
		}

	}
}

// ?????? ?????? 
void UEditEntityManageController::NetCall_RemoveData(TArray<int32> entityIdList)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	auto gameInstance = baseGameMode->gameInstance;

	if (entityIdList.Num() == 0)return;

	Gamedata::EntityRemoveRequest request;

	request.set_playerid(data->GetLocalPlayerId());

	for (auto node : entityIdList)
	{
		request.add_entityidlist(node);
	}

	std::string sendStr = request.SerializePartialAsString();
	char* sendData = (char*)sendStr.c_str();
	int sendSize = sendStr.length();
	int type = (int)Gamedata::MsgType::PB_EntityRemoveRequest;
	gameInstance->netMainComponent->NetCall_GameData(type, sendData, sendSize);
}

// ?????? ?????? 
void UEditEntityManageController::NetCallback_EntityRemove(int32 entityId)
{
	if (data->IsExistData(entityId) == false)
	{
		return;
	}
	// ??????
	//auto syncInfo = data->GetSyncInfo(entityId);

	// ????????????????????????????????????,?????????,?????????
	RemoveChooseAxis(entityId);

	view->EntityRemove(entityId);
}

// ?????? ?????? ?????????????????????
void UEditEntityManageController::RemoveChooseAxis(int32 entityId)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (baseCharacter == nullptr)return;

	baseCharacter->selectComponent->RemoveSelected(entityId);
}

// ?????? ?????? 
void UEditEntityManageController::NetCall_MergeEntity(std::vector<Gamedata::EntityInfo*>nodeEntityIdList)
{

	ABaseGameMode* gameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	auto localPlayerId = gameMode->GetLocalPlayerId();

	if (nodeEntityIdList.size() <= 1)return;

	// ??????
	{
		Gamedata::EntityInfo syncInfo;
		auto masterSyncInfo = nodeEntityIdList[0];

		syncInfo.mutable_transform()->mutable_location()->CopyFrom(*masterSyncInfo->mutable_transform()->mutable_location());
		syncInfo.mutable_transform()->mutable_rotator()->CopyFrom(*masterSyncInfo->mutable_transform()->mutable_rotator());
		syncInfo.mutable_transform()->mutable_scale()->set_x(1);
		syncInfo.mutable_transform()->mutable_scale()->set_y(1);
		syncInfo.mutable_transform()->mutable_scale()->set_z(1);
		FTransform masterWTransform = UCommonlyObject::PBToUe4Transform(syncInfo.mutable_transform());

		// actor
		for (auto i = 0; i < nodeEntityIdList.size(); i++)
		{
			auto tempSyncInfo = nodeEntityIdList[i];
			auto entityWTransform = UCommonlyObject::PBToUe4Transform(tempSyncInfo->mutable_transform());

			for (auto h = 0; h < tempSyncInfo->child_list_size(); h++)
			{
				//int h = 0;
				auto childInfo = syncInfo.add_child_list();
				//childInfo->mutable_mesh()->set_xls_id(tempSyncInfo->child_list(h).mesh().xls_id());
				//childInfo->mutable_mesh()->set_material_id(tempSyncInfo->child_list(h).mesh().material_id());
				childInfo->mutable_mesh()->CopyFrom(tempSyncInfo->child_list(h).mesh());

				FTransform childRTransform = UCommonlyObject::PBToUe4Transform(tempSyncInfo->mutable_child_list(h)->mutable_transform());
				FVector scale = childRTransform.GetScale3D();
				childRTransform.SetScale3D(FVector::OneVector);
				FTransform childWTransform = UCalculationObject::RelativeToWorldTransform(entityWTransform, childRTransform);
				childWTransform.SetScale3D(scale * entityWTransform.GetScale3D());

				auto childRTransform2 = childWTransform.GetRelativeTransform(masterWTransform);

				UCommonlyObject::Ue4ToPbTransform(childRTransform2, childInfo->mutable_transform());
				UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [NetCall_MergeEntity] node [i:%d--%d]  [xlsId:%d] [masterTransform:%s] [childRTransform:%s] [childWTransform:%s]"),
					this, i, h, tempSyncInfo->child_list(h).mesh().xls_id(),
					*masterWTransform.ToString(),
					*childRTransform2.ToString(),
					*entityWTransform.ToString()
				);
			}


		}

		NetCall_NewData(syncInfo);
	}


	// ?????????????????????
	{
		TArray<int32> entityIdList;
		for (auto i = 0; i < nodeEntityIdList.size(); i++)
		{
			auto tempSyncInfo = nodeEntityIdList[i];
			entityIdList.Add(tempSyncInfo->entityid());
		}
		NetCall_RemoveData(entityIdList);
	}
}

// ?????? ??????
void UEditEntityManageController::NetCall_BreakDownEntity(std::vector<Gamedata::EntityInfo*>nodeEntityIdList)
{

	ABaseGameMode* gameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	auto localPlayerId = gameMode->GetLocalPlayerId();

	if (nodeEntityIdList.size() != 1)return;
	auto mergeSyncInfo = *nodeEntityIdList[0];
	if (mergeSyncInfo.child_list_size() < 2) return;

	FTransform masterWTransform = UCommonlyObject::PBToUe4Transform(mergeSyncInfo.mutable_transform());

	// ????????????????????????
	for (auto i = 0; i < mergeSyncInfo.child_list_size(); i++)
	{

		Gamedata::EntityInfo syncInfo;
		auto& mergeChildInfo = *mergeSyncInfo.mutable_child_list(i);
		auto childInfo = syncInfo.add_child_list();

		// mesh info
		//childInfo->mutable_mesh()->set_xls_id(mergeChildInfo.mesh().xls_id());
		//childInfo->mutable_mesh()->set_material_id(mergeChildInfo.mesh().material_id());
		childInfo->mutable_mesh()->CopyFrom(mergeChildInfo.mesh());

		// r transform
		FTransform rTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
		UCommonlyObject::Ue4ToPbTransform(rTransform, childInfo->mutable_transform());

		// w transform
		auto childRTransform = UCommonlyObject::PBToUe4Transform(mergeChildInfo.mutable_transform());
		auto meshWTransform = UCalculationObject::RelativeToWorldTransform(masterWTransform, childRTransform);
		UCommonlyObject::Ue4ToPbTransform(meshWTransform, syncInfo.mutable_transform());

		// w transform scale
		FVector childScale = UCommonlyObject::PBToUe4Vector(mergeChildInfo.mutable_transform()->mutable_scale());
		FVector scale = masterWTransform.GetScale3D() * childScale;
		syncInfo.mutable_transform()->mutable_scale()->set_x(scale.X);
		syncInfo.mutable_transform()->mutable_scale()->set_y(scale.Y);
		syncInfo.mutable_transform()->mutable_scale()->set_z(scale.Z);

		NetCall_NewData(syncInfo);
	}

	// ??????????????????
	{
		TArray<int32> entityIdList;
		entityIdList.Add(mergeSyncInfo.entityid());
		NetCall_RemoveData(entityIdList);
	}
}

/*----------------------------------------------------*/

// ?????????????????? ????????????
void UEditEntityManageController::OnlineResources_Callback_SetImage(int32 entityId, FVector2D imageSize, UImage* image,
	TWeakObjectPtr<UTexture2D> texture2D)
{
	// ????????????
	auto syncInfo = data->GetSyncInfo(entityId);
	// ????????????
	//view->imageView->CallUIOverSetImage(syncInfo, localInfo, image, texture2D);

	// ????????????
	//view->collisionView->BoxSetImageSize(syncInfo, localInfo);
}

/*----------------------------------------------------*/

