// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterControllerComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerPlaceComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerEntityBackpackComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Net/NetMainComponent.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "FunctionalModule/Selected/SelectedDataComponent.h"
#include "FunctionalModule/Selected/SelectedControllerComponent.h"
#include "PlayerCharacterMovementComponent.h"

#include "GameMode/BaseGameMode.h"
#include "Core/BaseGameInstance.h"
#include "UI/BaseHud.h"
#include "GameFramework/PlayerController.h"

#include "Json/Public/Serialization/JsonSerializer.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Tools/CommonlyObject.h"
#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include <AIModule/Classes/AIController.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerTimelineComponent.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManage.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/ControllerComponent/EditEntityTimelineComponent.h"
#include "PlayerEditEntityComponent.h"
#include "TimerManager.h"
#include "Yxjs/UI/HUDStaticFunc.h"

/*----------------------------------------------------*/

// ?????? ????????????view
TWeakObjectPtr<AEditEntityManage> UCharacterControllerComponent::GetEditEntityManage()
{
	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetOwner()->GetGameInstance());
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	return baseGameMode->editEntityManage;
}

/*----------------------------------------------------*/

// Sets default values for this component's properties
UCharacterControllerComponent::UCharacterControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UCharacterControllerComponent]  "),this);
}

// Called when the game starts
void UCharacterControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

//
void UCharacterControllerComponent::Start(UCharacterDataComponent* data_, ABaseCharacter* view_)
{
	view = view_;
	data = data_;

	data->CheckDefaultData();

	if (data->isLocal)
	{
	}
	else
	{
	}
}

//
void UCharacterControllerComponent::End()
{
}

//
void UCharacterControllerComponent::PossessedBy()
{
	if (data->CheckDefaultData() == false)return;

	if (data->isLocal)
	{
		//auto data->playerController = Cast<APlayerController>(view->GetController());
		if (data->playerController.IsValid())
		{
			//auto data->hud = Cast<ABaseHUD>(data->playerController->GetHUD());
			if (data->hud.IsValid())
			{
				// data->hud->ShowUI(UIName::ui_name_mainui);
				//data->hud->Call_ChangeMainUI(EGameMainOperationMode::game_main_operation_mode_base_character);
			}
		}
	}
	else
	{
	}
}

// Called every frame
void UCharacterControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (data->isLocal == false)return;

	auto nowTime = GetWorld()->TimeSeconds;
	if (nowTime >= (cvControllerInfo.lastTime + 0.3))
	{
		cvControllerInfo.lastTime = nowTime;
		FrameGetSyncTransform();
	}
}

/*----------------------------------------------------*/

// ?????? ????????????
void UCharacterControllerComponent::Function_LeaveRoom()
{
	//view->myData->GetNet()->UICall_LeavelRoom();
}

// ???????????? ??????????????????
void UCharacterControllerComponent::Input_ChangePawnType()
{
	if (view->myData->CheckDefaultData() == false)return;

	auto localPlayerId = view->myData->gameInstance->netMainComponent->cvPlayerInfo.playerId;
	auto localPlayerSyncInfo = view->myData->gameMode->GetPlayerSyncInfo(localPlayerId);
	if (localPlayerSyncInfo == nullptr)return;

	if (localPlayerSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Character)
	{
		Gamedata::PlayerInfo newSyncInfo;
		newSyncInfo.CopyFrom(*view->myData->GetSyncInfo());
		newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_Racing);
		view->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
	}
	else if (localPlayerSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Racing)
	{
		Gamedata::PlayerInfo newSyncInfo;
		newSyncInfo.CopyFrom(*view->myData->GetSyncInfo());
		newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_Character);
		view->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
	}
}

/*----------------------------------------------------*/

// ?????? ????????? ??????????????????
void UCharacterControllerComponent::FrameGetSyncTransform()
{
	if (data->CheckDefaultData() == false)return;

	auto moveComponent = Cast<UPlayerCharacterMovementComponent>(view->GetMovementComponent());
	if (moveComponent == nullptr) return;

	auto localPlayerId = data->GetNet()->cvPlayerInfo.playerId;
	// ??????????????????
	auto characterLocation = GetOwner()->GetActorLocation();
	auto characterRotator = GetOwner()->GetActorRotation();

	if ((
		(characterLocation - cvControllerInfo.lastLocation).Size() > 100 || cvControllerInfo.lastVelocity.Equals(moveComponent->Velocity) == false)
		||
		(cvControllerInfo.lastLocation.Equals(characterLocation) == false && moveComponent->Velocity.Equals(FVector::ZeroVector))
		)
	{
		cvControllerInfo.lastLocation = characterLocation;
		cvControllerInfo.lastRotator = characterRotator;
		cvControllerInfo.lastVelocity = moveComponent->Velocity;
		NetCall_Move();
	}

	//// ?????????????????????
	//auto nowTime = GetWorld()->TimeSeconds;
	//if (nowTime >= (cvControllerInfo.lastTimeJiugongge + 2))
	//{
	//	cvControllerInfo.lastTimeJiugongge = nowTime;
	//	//FString jiugongeKey = data->GetJiugongge(characterLocation.X, characterLocation.Y);
	//	FString jiugongeKey = UCommonlyObject::GetJiugongge(characterLocation.X, characterLocation.Y,
	//	                                                    baseGameMode->syncSize);
	//	if (jiugongeKey.Equals(data->jiugonggeKey) == false)
	//	{
	//		UE_LOG(LogTemp, Log,
	//		       TEXT("[%x] [FrameGetSyncTransform] [localPlayerId:%d] ??????????????? [playerId:%d] [%s]-->[%s]"),
	//		       this, localPlayerId, data->playerId, *data->jiugonggeKey, *jiugongeKey);
	//		cvControllerInfo.lastLocation = characterLocation;
	//		cvControllerInfo.lastRotator = characterRotator;
	//		data->jiugonggeKey = jiugongeKey;
	//		NetCall_ChangeJiugongge();
	//	}
	//}
}

// ?????? ??????
void UCharacterControllerComponent::NetCall_Move()
{
	auto localPlayerId = data->GetNet()->cvPlayerInfo.playerId;
	Gamedata::PlayerInfo* localSyncInfo = data->GetSyncInfo();

	FVector characterLocation = view->GetActorLocation();
	FRotator characterRotator = view->GetActorRotation();
	FVector characterVelocity = view->GetVelocity();
	Gamedata::FVector* oldLocationPtr = localSyncInfo->mutable_playerlocaioninfo()->mutable_transform_s()->mutable_location();
	FVector oldLocation = UCommonlyObject::PBToUe4Vector(oldLocationPtr);

	//UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [NetCall_Move]  [GetVelocity:%s] [size:%f] [??????:%s]"),
	//	this, *characterVelocity.ToString(), characterVelocity.Size(), *characterVelocity.Rotation().ToString());

	// ???????????????????????????
	float moveSize = (oldLocation - characterLocation).Size();
	if (localSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity
		&& moveSize > 0)
	{
		characterVelocity = (oldLocation - view->GetActorLocation()).Rotation().Vector() * data->editMoveSpeed;
	}

	Gamedata::UpdateLocationRequest info;
	info.set_playerid(localPlayerId);
	Gamedata::FVector* tempLocation = info.mutable_playerlocaioninfo()->mutable_transform_s()->mutable_location();
	tempLocation->set_x(characterLocation.X);
	tempLocation->set_y(characterLocation.Y);
	tempLocation->set_z(characterLocation.Z);
	info.mutable_playerlocaioninfo()->mutable_transform_s()->mutable_rotator()->set_yaw(characterRotator.Yaw);
	info.mutable_playerlocaioninfo()->set_charactermovementmodetype(Gamedata::CharacterMovementModeType::CharacterMovementModeType_Walk);
	Gamedata::FVector* velocity2 = info.mutable_playerlocaioninfo()->mutable_velocity();
	velocity2->set_x(characterVelocity.X);
	velocity2->set_y(characterVelocity.Y);
	velocity2->set_z(characterVelocity.Z);
	info.mutable_playerlocaioninfo()->set_isracing(false);

	std::string sendStr = info.SerializePartialAsString();
	char* sendData = (char*)sendStr.c_str();
	int sendSize = sendStr.length();
	int type = (int)Gamedata::MsgType::PB_PlayerMoveRequest;
	data->GetNet()->NetCall_GameData(type, sendData, sendSize);

}

// ?????? ??????
void UCharacterControllerComponent::NetCallback_Move(Gamedata::PlayerUpdateLocation& playerUpdateLocation)
{
	auto localPlayerId = data->GetNet()->cvPlayerInfo.playerId;
	auto moveComponent = Cast<UPlayerCharacterMovementComponent>(view->GetMovementComponent());

	Gamedata::PlayerInfo* syncInfo = data->GetSyncInfo();
	syncInfo->mutable_playerlocaioninfo()->mutable_transform_s()->CopyFrom(playerUpdateLocation.playerlocaioninfo().transform_s());
	syncInfo->mutable_playerlocaioninfo()->mutable_velocity()->CopyFrom(playerUpdateLocation.playerlocaioninfo().velocity());
	syncInfo->mutable_playerlocaioninfo()->set_charactermovementmodetype(playerUpdateLocation.playerlocaioninfo().charactermovementmodetype());

	// ???????????????
	if (data->isLocal)
	{
	}
	// ???????????????
	else
	{
		auto velocity = playerUpdateLocation.playerlocaioninfo().velocity();
		FVector velocity2(velocity.x(), velocity.y(), velocity.z());
		auto location = playerUpdateLocation.playerlocaioninfo().transform_s().location();
		FVector location2(location.x(), location.y(), location.z());
		auto rotator = playerUpdateLocation.playerlocaioninfo().transform_s().rotator();

		// ???????????????????????????
		FRotator rotator2(rotator.pitch(), rotator.yaw(), rotator.roll());

		EMovementMode movementMode = EMovementMode(playerUpdateLocation.playerlocaioninfo().charactermovementmodetype());

		auto moveSize = (location2 - view->GetActorLocation()).Size();


		//UE_LOG(LogTemp, Log, TEXT("[%x] [??????] [NetCallback_Move] [??????:%s]"),
		//	this, *((location2 - view->GetActorLocation())).Rotation().ToString());

		// ???????????????????????????
		//if (syncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity
		//	&& moveSize > 0) {

		//}


		{
			bool typeFlg = false;

			velocity2 = velocity2 * (3.0 / 4.0);

			auto replicatedmovement = view->GetReplicatedMovement();
			replicatedmovement.Location = view->GetActorLocation();// location2;
			replicatedmovement.Rotation = view->GetActorRotation(); // rotator2;
			replicatedmovement.LinearVelocity = velocity2;
			replicatedmovement.AngularVelocity = FVector::ZeroVector;
			view->SetReplicatedMovement(replicatedmovement);

			FRotator rotator3 = rotator2;

			moveComponent->MovementMode = movementMode;
			moveComponent->Velocity = velocity2;
			moveComponent->SmoothCorrection(
				view->GetActorLocation(), view->GetActorRotation().Quaternion(),
				location2, rotator3.Quaternion()
			);
		}
	}
}

// ???????????? ??????
void UCharacterControllerComponent::NetCall_UpdatePlayerInfo(Gamedata::PlayerInfo& newSyncInfo)
{
	auto characterLocation = view->GetActorLocation();
	auto characterRotator = view->GetActorRotation();

	//Gamedata::GameMainOperationMode gameMainOperationMode_
	//Gamedata::PlayerInfo newSyncInfo;
	//newSyncInfo.CopyFrom(*data->GetSyncInfo());
	//newSyncInfo.set_character_pawn_type(gameMainOperationMode_);

	Gamedata::FTransform_s* transform_s = newSyncInfo.mutable_playerlocaioninfo()->mutable_transform_s();;
	Gamedata::FVector* location = transform_s->mutable_location();
	Gamedata::FRotator* rotator = transform_s->mutable_rotator();

	location->set_x(characterLocation.X);
	location->set_y(characterLocation.Y);
	location->set_z(characterLocation.Z);

	rotator->set_pitch(characterRotator.Pitch);
	rotator->set_yaw(characterRotator.Yaw);
	rotator->set_roll(characterRotator.Roll);


	//NetCallback_UpdatePlayerInfo(newSyncInfo);
	// ???????????????,????????????????????????syncinfo?????????????????????NetCallback_UpdatePlayerInfo??????
	//data->gameMode->controller->NetCallback_PlayerUpdateView(newSyncInfo, false);

	std::shared_ptr<Gamedata::UpdatePlayerDataRequest> info = std::make_shared<Gamedata::UpdatePlayerDataRequest>();
	info->set_playerid(data->playerId);
	info->mutable_playerinfo()->CopyFrom(newSyncInfo);

	std::string sendStr = info->SerializePartialAsString();
	char* sendData = (char*)sendStr.c_str();
	int sendSize = sendStr.length();
	int type = (int)Gamedata::MsgType::PB_PlayerUpdateRequest;
	data->GetNet()->NetCall_GameData(type, sendData, sendSize);
}

// ???????????? ?????? ??????????????????callback,???????????????callback(???????????????)
void UCharacterControllerComponent::NetCallback_UpdatePlayerInfo(Gamedata::PlayerInfo& syncPlayerInfo, bool isEnterView)
{
	auto localPlayerId = data->GetNet()->cvPlayerInfo.playerId;
	Gamedata::PlayerInfo* oldPlayerSyncInfo = data->GetSyncInfo();

	if (isEnterView)
	{
		// ?????????????????????????????????
		if (syncPlayerInfo.character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity)
		{
			view->EnterEditStaus();
			UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [NetCallback_UpdatePlayerInfo] [??????????????????] [playerid:%d] "), this, syncPlayerInfo.playerid());
		}
		// ?????????????????????????????????
		else if (syncPlayerInfo.character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Character)
		{
			view->GetMovementComponent()->Velocity = FVector::ZeroVector;
			view->EnterNoneStaus();
			UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [NetCallback_UpdatePlayerInfo] [??????????????????] [playerid:%d] "), this, syncPlayerInfo.playerid());
		}
	}
	else
	{
		// ?????????????????????????????????
		if (oldPlayerSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Character
			&& syncPlayerInfo.character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity)
		{
			view->EnterEditStaus();
			UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [NetCallback_UpdatePlayerInfo] [??????????????????] [playerid:%d] "), this, syncPlayerInfo.playerid());
		}
		// ?????????????????????????????????
		else if (oldPlayerSyncInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity
			&& syncPlayerInfo.character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Character)
		{
			view->GetMovementComponent()->Velocity = FVector::ZeroVector;
			view->EnterNoneStaus();
			UE_LOG(LogTemp, Log, TEXT("[%x] [???????????????] [NetCallback_UpdatePlayerInfo] [??????????????????] [playerid:%d] "), this, syncPlayerInfo.playerid());
		}
	}


	// ???????????????
	if (data->isLocal)
	{
	}
	// ???????????????
	else
	{
		auto location = syncPlayerInfo.playerlocaioninfo().transform_s().location();
		FVector location2(location.x(), location.y(), location.z());
		auto rotator = syncPlayerInfo.playerlocaioninfo().transform_s().rotator();
		view->SetActorLocationAndRotation(location2, FRotator::ZeroRotator);
	}
}

/*----------------------------------------------------*/

// ???????????? ??????????????????????????????????????????
void UCharacterControllerComponent::LoadToGameProtect()
{

	//1. ????????????,????????????
	Gamedata::PlayerInfo* syncInfo = data->GetSyncInfo();
	if (data->isLocal == false)
	{
		return;
	}

	if (syncInfo->character_pawn_type() != Gamedata::GameMainOperationMode::GameMainOperationMode_Character)
	{
		return;
	}

	//
	LoadToGameProtect_Run(true);
}

// ???????????? ????????????
void UCharacterControllerComponent::LoadToGameProtect_Run(bool isStart)
{
	float relayTime = 0.5;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)return;

	FTimerHandle& timerHandle_DelayRun = loadProtectInfo.timerHandle_DelayRun;
	float nowTime = view->GetWorld()->TimeSeconds;
	// ????????????
	if (isStart)
	{
		LoadToGameProtect_Work(true);

		loadProtectInfo.startTime = nowTime;
		if (timerHandle_DelayRun.IsValid())
		{
			playerController->GetWorldTimerManager().ClearTimer(timerHandle_DelayRun);
		}
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &UCharacterControllerComponent::LoadToGameProtect_Run, false);
		playerController->GetWorldTimerManager().SetTimer(timerHandle_DelayRun, RespawnDelegate, relayTime, true);
	}
	// ????????????
	else
	{
		// ????????????
		if (loadProtectInfo.startTime + 10 < nowTime || EntityIsLoadOver())
		{
			if (timerHandle_DelayRun.IsValid())
			{
				playerController->GetWorldTimerManager().ClearTimer(timerHandle_DelayRun);
			}
			LoadToGameProtect_Work(false);// ????????????
		}
	}
}

// ????????????????????????
void UCharacterControllerComponent::LoadToGameProtect_Work(bool isStart)
{

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)return;

	UCharacterMovementComponent* movementComponent = view->GetCharacterMovement();
	UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [LoadToGameProtect_Work] %s  [time:%f]"), this, isStart ? TEXT("??????") : TEXT("??????"), view->GetWorld()->TimeSeconds);
	if (isStart)
	{
		movementComponent->GravityScale = 0;
		movementComponent->bApplyGravityWhileJumping = false;
		view->DisableInput(playerController);

		UHUDStaticFunc::PushMessage(this, TEXT("??????????????????,????????????"));
	}
	else
	{
		movementComponent->GravityScale = 1;
		movementComponent->bApplyGravityWhileJumping = true;
		view->EnableInput(playerController);

	}
}

// ????????????
bool UCharacterControllerComponent::EntityIsLoadOver()
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (baseGameMode == nullptr)return false;
	if (baseGameMode->sceneStaticMeshView == nullptr)return false;

	if (baseGameMode->sceneStaticMeshView->IsRunJob() == false)
	{
		return true;
	}
	return false;
}

/*----------------------------------------------------*/

// ?????? ????????? ????????????????????????????????????UI??????
void UCharacterControllerComponent::FrameEditMouseIsUIArea()
{
	if (data->CheckDefaultData() == false)return;

	if (data->isLocal == false)return;
	if (view == nullptr || data->playerController == nullptr)return;

	if (data->inputKeyInfo.mouseRightEnter
		|| data->inputKeyInfo.mouseLeftEnter
		)
	{
		return;
	}

	if (data->IsEditEntityTransform() == false)
	{
		// ?????????????????????????????????????????????
		if (data->isEditAndAreaUI)
		{
			data->isEditAndAreaUI = false;
			//UE_LOG(LogTemp, Log, TEXT("[%x] [FrameEditMouseIsUIArea]????????????2"),this);
			EditModeInUIArea(false);
		}
		return;
	}

	float x, y;
	data->playerController->GetMousePosition(x, y);
	//const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->GetSizeXY());
	FVector2D viewportSize = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
	const FVector2D viewportCenter = FVector2D(viewportSize.X / 2, viewportSize.Y / 2);
	auto ui_name_edit_entity = data->hud->GetChildUI(EUIChildName::ui_name_edit_entity);

	if (cvi.mouseX != int(x) || cvi.mouseY != int(y))
	{
		cvi.mouseX = int(x);
		cvi.mouseY = int(y);

		//if(cvi.mouseX<60)
		//if (cvi.mouseY < 60)/
		float var1 = ((float)cvi.mouseY / viewportSize.Y);
		if (var1 > (9.0 / 10.0))
		{
			if (data->isEditAndAreaUI == false)
			{
				UE_LOG(LogTemp, Log, TEXT("[%x] [FrameEditMouseIsUIArea] [viewportSize:%s] [%f %f] ????????????"),
					this, *viewportSize.ToString(), x, y);
				data->isEditAndAreaUI = true;
				EditModeInUIArea(true);
			}
		}
		else
		{
			if (data->isEditAndAreaUI)
			{
				data->isEditAndAreaUI = false;
				UE_LOG(LogTemp, Log, TEXT("[%x] [FrameEditMouseIsUIArea] [viewportSize:%s] [%f %f] ????????????"),
					this, *viewportSize.ToString(), x, y);
				EditModeInUIArea(false);
			}
		}
	}
}

// ?????? ????????????????????????UI??????
void UCharacterControllerComponent::EditModeInUIArea(bool flag)
{
	if (data->CheckDefaultData() == false)return;
	//auto data->playerController = Cast<APlayerController>(view->Controller);

	auto ui_name_edit_entity = data->hud->GetChildUI(EUIChildName::ui_name_edit_entity);
	// ????????????
	if (flag)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [EditModeInUIArea] ??????UI??????"), this);
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(data->playerController.Get());
		//ui_name_edit_entity->Visibility = ESlateVisibility::SelfHitTestInvisible;
		ui_name_edit_entity->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	// ????????????
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [EditModeInUIArea] ??????UI??????"), this);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(data->playerController.Get());
		//ui_name_edit_entity->Visibility = ESlateVisibility::Hidden;
		ui_name_edit_entity->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

/*----------------------------------------------------*/