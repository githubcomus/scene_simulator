// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameModeController.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "Net/NetMainComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "Yxjs/FunctionalModule/ActorTrajectory/ActorTrajectoryView.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Pawn/Character/BaseCharacterInfo.h"
//#include "Pawn/Racing/Player/PlayerRacingPawn.h"
//#include "Pawn/Racing/PlayerComponent/RacingControllerComponent.h"
//#include "Pawn/Racing/PlayerComponent/RacingDataComponent.h"
#include "Yxjs/Pawn/Racing/Simulation/SimulationRacingPawn.h"
#include "Yxjs/Pawn/Racing/SimulationComponent/SimulationRacingControllerComponent.h"
#include "Yxjs/Pawn/Racing/SimulationComponent/SimulationRacingDataComponent.h"
#include "Yxjs/FunctionalModule/EditEntity/TempComponent/PlaceControllerScenesTimelineObject.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/Timeline/EditEntityManageTimelineComponent.h"
#include "Yxjs/FunctionalModule/OverallSituation/OverallSituationView.h"
#include "Pawn/Character/CharacterComponent/PlayerInputComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerEditEntityComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerCommandComponent.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Camera/CameraComponent.h"
#include "Yxjs/Pawn/Character/CharacterComponent/PlayerTimelineComponent.h"
#include "Yxjs/FunctionalModule/EditEntity/Command/EditEntityManageCommandComponent.h"
#include "Yxjs/Core/Component/ZwXlsDataActorComponent.h"

/*
游戏消息响应分发

结构:
主操作模式  - 角色视图
			- 主UI操作模式 - 子UI
						   - 输入模式
						   - 角色视图更变


*/

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
UBaseGameModeController::UBaseGameModeController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBaseGameModeController::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

//
void UBaseGameModeController::Start(ABaseGameMode* data_)
{
	data = data_;

	// 生成测试box
	SpawnTestBox();
	//TestFunc();

	//LoadLevelInit();
}

//
void UBaseGameModeController::End()
{
}

// Called every frame
void UBaseGameModeController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto nowTime = GetWorld()->TimeSeconds;
	int maxCount = 30;	// 每帧1个
	int doCount = 0;
	auto nowTime1 = GetWorld()->TimeSeconds;

	if (data->syncInfoList.size() > 0)
	{
		while (true)
		{
			auto it = data->syncInfoList.begin();
			auto& node = it->second;
			if (node.type == ABaseGameMode::ERealSyncType::ERealSyncType_add)
			{
				NetCallback_EntityEnterView(&node.entity);
			}
			else if (node.type == ABaseGameMode::ERealSyncType::ERealSyncType_update)
			{
				NetCallback_EntityUpdateView(&node.entity);
			}
			else if (node.type == ABaseGameMode::ERealSyncType::ERealSyncType_remove)
			{
				NetCallback_EntityLeaveView(node.entityId);
			}
			doCount++;
			data->syncInfoList.erase(it);
			if (data->syncInfoList.size() == 0 || doCount == maxCount)
			{
				break;
			}
		}
	}
	auto nowTime2 = GetWorld()->TimeSeconds;
	auto useTime = nowTime2 - nowTime1;
	if (useTime > 0.2)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [UNetMainComponent::TickComponent] [DeltaTime:%d] [useTime:%d] [count:%d]"),
			this, DeltaTime, useTime, doCount);
	}

	if (flagConnect_Callback)
	{
		Connect_Callback(false);
	}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 网络调用 消息请求
void UBaseGameModeController::NetCall_GameData(EMsgType type, void* msgInfo)
{
	int dataSize = 0;
	char* outData = nullptr;
	int tempType = 0;

	switch (type)
	{
	case EMsgType_OverallSituationDataRequest:
		tempType = Gamedata::MsgType::BP_OverallSituationData_Change;
		Gamedata::OverallSituationDataRequest* msgData = (Gamedata::OverallSituationDataRequest*)msgInfo;
		dataSize = msgData->ByteSize() + 128;
		outData = (char*)malloc(dataSize);
		dataSize = msgData->ByteSize();
		msgData->SerializeToArray(outData, dataSize);
		break;
	}

	if (outData != nullptr)
	{
		data->gameInstance->netMainComponent->NetCall_GameData(tempType, outData, dataSize);
		free(outData);
	}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 连接服务器 连接失败 回调
void UBaseGameModeController::Connect_Callback(bool isChildThread)
{

	// 子线程标记
	if (isChildThread)
	{
		flagConnect_Callback = true;
	}
	// 主线程执行
	else
	{
		flagConnect_Callback = false;
		UE_LOG(LogTemp, Log, TEXT("[%x] [Connect_Callback]  网络连接失败 "), this);
	}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 网络回调 玩家进入视野
void UBaseGameModeController::NetCallback_PlayerEnterView(Gamedata::PlayerInfo& newSyncPlayerInfo)
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;

	//1. 载入数据
	data->AddPlayerSyncInfo(newSyncPlayerInfo.playerid(), &newSyncPlayerInfo);
	auto oldSyncPlayerInfo = data->GetPlayerSyncInfo(newSyncPlayerInfo.playerid());

	//2. 生成actor
	{
		auto pawnType = data->GetPawnType(oldSyncPlayerInfo->character_pawn_type());
		if (pawnType == ABaseGameMode::CvPawnType::CvPawnType_character)
		{
			SpawnCharacter(*data->GetPlayerSyncInfo(newSyncPlayerInfo.playerid()));
			hud->Call_ChangeMainUI(EGameMainOperationMode::game_main_operation_mode_base_character);
		}
		else if (pawnType == ABaseGameMode::CvPawnType::CvPawnType_racing)
		{
			SpawnRacing(*data->GetPlayerSyncInfo(newSyncPlayerInfo.playerid()));
		}
	}

	//3. 更新信息
	NetCallback_PlayerUpdateView(newSyncPlayerInfo, true);

	//4. 本地客户端数据登录
	if (oldSyncPlayerInfo->playerid() == localPlayerId)
	{
		data->levelManage->LoadLevelInit();
	}

	//5. 登录保护
	{
		auto character = Cast<ABaseCharacter>(data->GetPlayer(oldSyncPlayerInfo->playerid()));
		if (character)
		{
			character->myController->LoadToGameProtect();
		}
	}
}

// 网络回调 玩家信息更新
void UBaseGameModeController::NetCallback_PlayerUpdateView(Gamedata::PlayerInfo& newSyncPlayerInfo, bool isEnterView)
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;

	// 进入视野
	if (isEnterView)
	{
	}
	// 变身
	else
	{
		Gamedata::PlayerInfo* oldSyncPlayerInfo = data->GetPlayerSyncInfo(newSyncPlayerInfo.playerid());
		if (oldSyncPlayerInfo == nullptr)return;
		auto newPawnType = data->GetPawnType(newSyncPlayerInfo.character_pawn_type());
		auto oldPawnType = data->GetPawnType(oldSyncPlayerInfo->character_pawn_type());

		// 主操作模式变身
		if (newPawnType != oldPawnType)
		{
			if (newPawnType == ABaseGameMode::CvPawnType::CvPawnType_character)
			{
				playerController->UnPossess();
				DestroyRacing(newSyncPlayerInfo.playerid());
				SpawnCharacter(newSyncPlayerInfo);
			}
			else if (newPawnType == ABaseGameMode::CvPawnType::CvPawnType_racing)
			{
				playerController->UnPossess();
				DestroyCharacter(newSyncPlayerInfo.playerid());
				SpawnRacing(newSyncPlayerInfo);
			}
			UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [NetCallback_PlayerUpdateView] [变身] [newPawnType:%d] "), this, newPawnType);
		}
		// 玩家变身
		else if (newPawnType == ABaseGameMode::CvPawnType::CvPawnType_character
			&& oldSyncPlayerInfo->character_index() != newSyncPlayerInfo.character_index()
			)
		{
			DestroyCharacter(newSyncPlayerInfo.playerid());
			SpawnCharacter(newSyncPlayerInfo);
		}
	}

	auto oldSyncPlayerInfo = data->GetPlayerSyncInfo(newSyncPlayerInfo.playerid());

	// 更新操作模式
	{
		if (oldSyncPlayerInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Character
			|| oldSyncPlayerInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity)
		{
			UpdateCharacter(newSyncPlayerInfo, isEnterView);
		}
		else if (oldSyncPlayerInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Racing)
		{
			UpdateRacing(newSyncPlayerInfo);
		}
	}

	// 设置主UI
	NetCallback_UpdataUI(oldSyncPlayerInfo, &newSyncPlayerInfo, isEnterView);

	oldSyncPlayerInfo->CopyFrom(newSyncPlayerInfo);
}

// 网络回调 NetCallback_PlayerUpdateView->UI更新
void UBaseGameModeController::NetCallback_UpdataUI(Gamedata::PlayerInfo* oldSyncPlayerInfo, Gamedata::PlayerInfo* newSyncPlayerInfo, bool isEnterView)
{

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;

	// 主客户端
	if (newSyncPlayerInfo->playerid() == localPlayerId)
	{
		if (newSyncPlayerInfo->character_pawn_type() != oldSyncPlayerInfo->character_pawn_type() || isEnterView)
		{

			UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [NetCallback_PlayerUpdateView] [更变主操作模式] [character_pawn_type:%s] "), this, *hud->GetMainUIName((EGameMainOperationMode)newSyncPlayerInfo->character_pawn_type()));
			switch (newSyncPlayerInfo->character_pawn_type())
			{
			case Gamedata::GameMainOperationMode::GameMainOperationMode_Character:
				hud->Call_ChangeMainUI(EGameMainOperationMode::game_main_operation_mode_base_character);//, EUIChildName::ui_name_mainui
				data->editEntityManage->timeline->SetOpenClosePlay(true);
				data->editEntityManage->command->SetOpenClosePlay(true);

				break;
			case Gamedata::GameMainOperationMode::GameMainOperationMode_Racing:
				hud->Call_ChangeMainUI(EGameMainOperationMode::game_main_operation_mode_racing);//, EUIChildName::ui_name_racing
				data->editEntityManage->timeline->SetOpenClosePlay(true);
				data->editEntityManage->command->SetOpenClosePlay(true);
				break;
			case Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity:
			{
				data->editEntityManage->timeline->SetOpenClosePlay(false);// 时间轴启动,选中物体,切换到time=0的位置
				data->editEntityManage->command->SetOpenClosePlay(false);// 命令控制器

				hud->Call_ChangeMainUI(EGameMainOperationMode::game_main_operation_mode_edit_etity); //, EUIChildName::ui_name_edit_entity

			}
			break;
			default:
				break;
			}
		}
	}
}

// 网络回调 玩家位置更新
void UBaseGameModeController::NetCallback_PlayerUpdateLocation(Gamedata::PlayerUpdateLocation& playerUpdateLocation)
{
	auto oldSyncPlayerInfo = data->GetPlayerSyncInfo(playerUpdateLocation.playerid());
	if (oldSyncPlayerInfo == nullptr)return;
	if (oldSyncPlayerInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Character
		|| oldSyncPlayerInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity)
	{
		MoveCharacter(playerUpdateLocation);
	}
	else if (oldSyncPlayerInfo->character_pawn_type() == Gamedata::GameMainOperationMode::GameMainOperationMode_Racing)
	{
		MoveRacing(playerUpdateLocation);
	}
}

// 网络回调 玩家离开视野
void UBaseGameModeController::NetCallback_PlayerLeaveView(int playerId_)
{
	auto playerSyncInfo = data->GetPlayerSyncInfo(playerId_);
	if (playerSyncInfo == nullptr)return;
	auto pawnType = data->GetPawnType(playerSyncInfo->character_pawn_type());
	if (pawnType == ABaseGameMode::CvPawnType::CvPawnType_character)
	{
		DestroyCharacter(playerId_);
	}
	else if (pawnType == ABaseGameMode::CvPawnType::CvPawnType_racing)
	{
		DestroyRacing(playerId_);
	}
	data->RemovePlayerSyncInfo(playerId_);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 网络回调 实体进入视野
void UBaseGameModeController::NetCallback_EntityEnterView(Gamedata::EntityInfo* syncEntityInfo)
{
	data->editEntityManage->controller->NetCallback_EntityAdd(syncEntityInfo);
}

// 网络回调 实体离开视野
void UBaseGameModeController::NetCallback_EntityLeaveView(int entityId_)
{
	data->editEntityManage->controller->NetCallback_EntityRemove(entityId_);
}

// 网络回调 实体更新视野
void UBaseGameModeController::NetCallback_EntityUpdateView(Gamedata::EntityInfo* syncEntityInfo)
{
	data->editEntityManage->controller->NetCallback_EntityUpdate(syncEntityInfo);
}

// 网络回调 实体操作
void UBaseGameModeController::NetCallback_EntityOperation(Gamedata::EntityOperationTotal& info)
{
	if (data->editEntityManage == nullptr)return;

	if (data->editEntityManage->controller == nullptr)return;
	auto timelineController = data->editEntityManage->controller->scenesTimelineController;
	if (timelineController == nullptr)return;

	timelineController->NetCallback_EntityOperation(info);
}

// 网络回调 全局数据更变
void UBaseGameModeController::NetCallback_BP_OverallSituationData_Change(Gamedata::OverallSituationDataResponse& info)
{
	data->overallSituationView->NetCallback_BP_OverallSituationData_Change(info);
}

// 网络回调 事件信息
void UBaseGameModeController::NetCallback_PB_EventInfo(Gamedata::EventInfo& info)
{

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;
	auto character = Cast<ABaseCharacter>(playerController->GetPawn());
	if (character == nullptr)return;

	character->commandComponent->NetCallback_Command(info);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 人物 生成
void UBaseGameModeController::SpawnCharacter(Gamedata::PlayerInfo& syncPlayerInfo)
{
	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance == nullptr)return;
	if (gameInstance->zwXlsDataActorComponent == nullptr)return;

	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;
	int playerId = syncPlayerInfo.playerid();

	//UE_LOG(LogTemp, Log, TEXT("[%x] [CharacterCreate] [localPlayerId:%d]  "),this, localPlayerId);

	if (data->playerMap.Contains(syncPlayerInfo.playerid()))
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [SpawnCharacter] [localPlayerId:%d]  角色已经存在  [playerId:%d]"),
			this, localPlayerId, syncPlayerInfo.playerid());
		return;
	}

	auto location = syncPlayerInfo.playerlocaioninfo().transform_s().location();
	auto rotator = syncPlayerInfo.playerlocaioninfo().transform_s().rotator();
	FVector location2(location.x(), location.y(), location.z());
	FRotator rotator2(rotator.pitch(), rotator.yaw(), rotator.roll());
	FTransform spawnTransform(rotator2, location2);



	//FString bpPath = TEXT("Blueprint'/Game/Yxjs/BP/Pawn/BP_BaseCharacter.BP_BaseCharacter_C'");
	FString bpPath = TEXT("Blueprint'/Game/ResPackage/LanFang/Blueprints/Combines/BP_CB_LanFang_A.BP_CB_LanFang_A_C'");
	//FString bpPath = TEXT("Blueprint'/Game/ResPackage/LP287Character_P3/Blueprints/Combines/BP_CB_LP287_B.BP_CB_LP287_B_C'");

	// 根据index来生成模型
	{
		xlsdata::XlsData& xlsData = gameInstance->zwXlsDataActorComponent->xlsData;
		std::vector<xlsdata::CharacterTableDefine>& xlsList = xlsData.CharacterTable_;
		if (syncPlayerInfo.character_index() < xlsList.size())
		{
			xlsdata::CharacterTableDefine& xlsInfo = xlsList[syncPlayerInfo.character_index()];
			bpPath = UTF8_TO_TCHAR(xlsInfo.BpPath_.c_str());
		}
	}

	UClass* loadObj = LoadClass<AActor>(this, *bpPath);
	if (loadObj == nullptr)
	{
		return;
	}

	// 创建角色
	auto character = Cast<ABaseCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, loadObj, spawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (character == nullptr)
	{
		return;
	}
	auto actor = UGameplayStatics::FinishSpawningActor(character, spawnTransform);
	if (character)
	{
		bool isLocal = false;
		if (syncPlayerInfo.playerid() == localPlayerId)
		{
			isLocal = true;
		}
		data->playerMap.Add(syncPlayerInfo.playerid(), character);
		character->Start(syncPlayerInfo.playerid(), isLocal, syncPlayerInfo);

		//{
		//	TWeakObjectPtr<ABaseCharacter> pData = character;
		//	if (pData.IsValid()) {
		//		int h = 0;
		//	}
		//	else {
		//		int h = 0;
		//	}
		//}

		// 本地客户端
		if (isLocal)
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [SpawnCharacter] [localPlayerId:%d] 本地客户端 创建角色  [playerId:%d]"),
				this, localPlayerId, syncPlayerInfo.playerid());

			data->localPlayerCameraComponet = Cast<UCameraComponent>(character->GetComponentByClass(UCameraComponent::StaticClass()));
		}
		// 模拟客户端
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [SpawnCharacter] [localPlayerId:%d] 模拟客户端 创建角色 [playerId:%d]"),
				this, localPlayerId, syncPlayerInfo.playerid());

			data->actorTrajectoryView->OnPlayerCreate(syncPlayerInfo.playerid());
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [SpawnCharacter] 创建角色 ERR 失败 [playerId:%d]"),
			this, syncPlayerInfo.playerid());
	}
}

// 人物 销毁
void UBaseGameModeController::DestroyCharacter(int playerId_)
{
	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;
	int playerId = playerId_;

	if (data->playerMap.Contains(playerId_) == false)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [DestroyCharacter] [localPlayerId:%d]  角色已经不存在  [playerId:%d]"),
			this, localPlayerId, playerId_);
		return;
	}

	if (data->playerMap.Contains(playerId_))
	{
		auto character = Cast<ABaseCharacter>(data->playerMap[playerId_]);
		bool isLocal = character->myData->isLocal;
		bool destroyFlag = false;

		character->SetRole(ENetRole::ROLE_None);
		if (character != nullptr)
		{
			character->End();

			destroyFlag = character->Destroy();
		}
		if (isLocal)
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [DestroyCharacter] [localPlayerId:%d] 本地客户端 销毁角色  [playerId:%d] [destroyFlag:%d] "),
				this, localPlayerId, playerId_, destroyFlag);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [DestroyCharacter] [localPlayerId:%d] 模拟客户端 销毁角色  [playerId:%d] [destroyFlag:%d] "),
				this, localPlayerId, playerId_, destroyFlag);

			data->actorTrajectoryView->OnPlayerDestroy(playerId_);
		}
		data->playerMap.Remove(playerId_);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [DestroyCharacter] 销毁角色 ERR 失败 [playerId:%d]"),
			this, playerId_);
	}
}

// 人物 更新
void UBaseGameModeController::UpdateCharacter(Gamedata::PlayerInfo& syncPlayerInfo, bool isEnterView)
{
	int localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;
	ABaseCharacter* character = Cast<ABaseCharacter>(data->GetPlayer(syncPlayerInfo.playerid()));
	if (character == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [UpdateCharacter] 角色更新失败 ERR 失败 [localPlayerId:%d] [playerId:%d]"),
			this, localPlayerId, syncPlayerInfo.playerid());
		return;
	}
	character->myController->NetCallback_UpdatePlayerInfo(syncPlayerInfo, isEnterView);
}

// 人物 位置
void UBaseGameModeController::MoveCharacter(Gamedata::PlayerUpdateLocation& playerUpdateLocation)
{
	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;
	auto character = Cast<ABaseCharacter>(data->GetPlayer(playerUpdateLocation.playerid()));
	if (character)
	{
		character->myController->NetCallback_Move(playerUpdateLocation);

		//auto location = playerUpdateLocation.playerlocaioninfo().transform_s().location();
		//FVector location2(location.x(), location.y(), location.z());
		//data->actorTrajectoryView->OnUpdatePoint(playerUpdateLocation.playerid(), location2);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [人物] [MoveCharacter] 玩家位置更新 ERR 失败 [localPlayerId:%d] [playerId:%d]"),
			this, localPlayerId, playerUpdateLocation.playerid());
	}
}

// 赛车 生成
void UBaseGameModeController::SpawnRacing(Gamedata::PlayerInfo& syncPlayerInfo)
{
	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;
	int playerId = syncPlayerInfo.playerid();

	if (data->playerMap.Contains(syncPlayerInfo.playerid()))
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [SpawnRacing] [localPlayerId:%d]  角色已经存在  [playerId:%d]"),
			this, localPlayerId, syncPlayerInfo.playerid());
		return;
	}

	auto location = syncPlayerInfo.playerlocaioninfo().transform_s().location();
	auto rotator = syncPlayerInfo.playerlocaioninfo().transform_s().rotator();
	FVector location2(location.x(), location.y(), location.z());
	FRotator rotator2(rotator.pitch(), rotator.yaw(), rotator.roll());
	FTransform spawnTransform(rotator2, location2);

	bool isLocal = false;
	if (syncPlayerInfo.playerid() == localPlayerId)
	{
		isLocal = true;
	}

	// 本地客户端
	if (isLocal)
	{
		//auto character = Cast<APlayerRacingPawn>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetOwner(), APlayerRacingPawn::StaticClass(), spawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		//if (character)
		//{
		//	UGameplayStatics::FinishSpawningActor(character, spawnTransform);
		//	data->playerMap.Add(syncPlayerInfo.playerid(), character);
		//	character->Start(syncPlayerInfo.playerid(), isLocal, syncPlayerInfo);

		//	data->localPlayerCameraComponet = Cast<UCameraComponent>(character->GetComponentByClass(UCameraComponent::StaticClass()));

		//	UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [SpawnRacing] [localPlayerId:%d] 本地客户端 创建角色  [playerId:%d]"),
		//		this, localPlayerId, syncPlayerInfo.playerid());
		//}
	}
	// 模拟客户端
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [SpawnRacing] [localPlayerId:%d] 模拟客户端 创建角色 [playerId:%d]"),
			this, localPlayerId, syncPlayerInfo.playerid());

		FString bpPath = TEXT("Blueprint'/Game/Yxjs/BP/Pawn/BP_SimulationRacingPawn.BP_SimulationRacingPawn_C'");
		UClass* loadObj = LoadClass<AActor>(this, *bpPath);
		if (loadObj == nullptr)
		{
			return;
		}

		auto character = Cast<ASimulationRacingPawn>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetOwner(), loadObj, spawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (character)
		{
			UGameplayStatics::FinishSpawningActor(character, spawnTransform);
			data->playerMap.Add(syncPlayerInfo.playerid(), character);
			character->Start(syncPlayerInfo.playerid());

			UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [SpawnRacing] [localPlayerId:%d] 模拟客户端 创建角色  [playerId:%d]"),
				this, localPlayerId, syncPlayerInfo.playerid());
		}

		data->actorTrajectoryView->OnPlayerCreate(syncPlayerInfo.playerid());
	}
}

// 赛车 销毁
void UBaseGameModeController::DestroyRacing(int playerId_)
{
	auto localPlayerId = data->gameInstance->netMainComponent->cvPlayerInfo.playerId;
	bool isLocal = data->IsLocalPlayer(playerId_);
	if (data->playerMap.Contains(playerId_))
	{
		//if (isLocal) {
		//	auto character = Cast<APlayerRacingPawn>(data->playerMap[playerId_]);
		//	bool destroyFlag = false;
		//	character->SetRole(ENetRole::ROLE_None);
		//	if (character != nullptr)
		//	{
		//		character->End();
		//		destroyFlag = character->Destroy();
		//	}
		//	UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [DestroyRacing] [localPlayerId:%d] 本地客户端 销毁角色  [playerId:%d] [destroyFlag:%d] "),
		//		this, localPlayerId, playerId_, destroyFlag);
		//}
		//else {
		//	auto character = Cast<ASimulationRacingPawn>(data->playerMap[playerId_]);
		//	bool destroyFlag = false;
		//	character->SetRole(ENetRole::ROLE_None);
		//	if (character != nullptr)
		//	{
		//		character->End();
		//		destroyFlag = character->Destroy();
		//	}
		//	data->actorTrajectoryView->OnPlayerDestroy(playerId_);
		//	UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [DestroyRacing] [localPlayerId:%d] 模拟客户端 销毁角色  [playerId:%d] [destroyFlag:%d] "),
		//		this, localPlayerId, playerId_, destroyFlag);
		//}
		//data->playerMap.Remove(playerId_);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [赛车] [DestroyRacing] 销毁角色 ERR 失败 [playerId:%d]"),
			this, playerId_);
	}
}

// 赛车 更新
void UBaseGameModeController::UpdateRacing(Gamedata::PlayerInfo& syncPlayerInfo)
{
}

// 赛车 位置
void UBaseGameModeController::MoveRacing(Gamedata::PlayerUpdateLocation& playerUpdateLocation)
{
	bool isLocal = data->IsLocalPlayer(playerUpdateLocation.playerid());
	//if (isLocal) {
	//	auto myRacingPawn = Cast<APlayerRacingPawn>(data->GetPlayer(playerUpdateLocation.playerid()));
	//	if (myRacingPawn) {
	//		myRacingPawn->controllerComp->NetCallback_PlayerUpdateLocation(playerUpdateLocation);
	//	}
	//}
	//else {
	//	auto myRacingPawn = Cast<ASimulationRacingPawn>(data->GetPlayer(playerUpdateLocation.playerid()));
	//	if (myRacingPawn) {
	//		myRacingPawn->controllerComp->NetCallback_PlayerUpdateLocation(playerUpdateLocation);
	//	}
	//}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 事件 网络资源管理回调放置视图
void UBaseGameModeController::OnlineResources_Callback_To_PlaceView(int32 entityId, FVector2D imageSize, UImage* image, TWeakObjectPtr<UTexture2D> texture2D)
{
	data->editEntityManage->controller->OnlineResources_Callback_SetImage(entityId, imageSize, image, texture2D);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 测试 生成测试box
void UBaseGameModeController::SpawnTestBox()
{
	return;

	FString bpPath = TEXT("Blueprint'/Game/Yxjs/BP/Jiugongge/BP_Box.BP_Box_C'");
	UClass* loadObj = LoadClass<AActor>(this, *bpPath);
	if (loadObj == nullptr)
	{
		return;
	}

	for (int x = -10; x < 10; x++)
	{
		for (int y = -10; y < 10; y++)
		{
			FVector location(x * 1000, y * 1000, 0);

			FTransform spawnTransform(FRotator::ZeroRotator, location);
			auto actor = Cast<AActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), loadObj, spawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
			if (actor != nullptr)
			{
				auto actor2 = UGameplayStatics::FinishSpawningActor(actor, spawnTransform);
			}
		}
	}
}

// test
void UBaseGameModeController::TestFunc()
{
	FTransform t;
	t.SetLocation(FVector::ZeroVector + FVector::UpVector * 1000);
	t.SetRotation(FRotator::ZeroRotator.Quaternion());

}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

