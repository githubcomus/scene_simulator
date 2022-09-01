// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameMode.h"
//#include "Yxjs.h"
#include "Net/NetMainComponent.h"
#include "OnlineResources/OnlineResourcesComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "GameMode/Component/BaseGameModeLevelManage.h"
#include "Pawn/Character/BaseCharacter.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/SceneStaticMesh/SceneStaticMeshView.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "UI/BaseHUD.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceControllerScenesTimelineObject.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "../FunctionalModule/ActorTrajectory/ActorTrajectoryView.h"
#include "../FunctionalModule/OverallSituation/OverallSituationView.h"
#include "Camera/CameraComponent.h"
#include "../UI/HallHUD.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 获取 自身玩家ID
int ABaseGameMode::GetLocalPlayerId()
{
	return gameInstance->netMainComponent->cvPlayerInfo.playerId;
}

// 获取 自身玩家
APawn* ABaseGameMode::GetLocalPlayer()
{
	if (gameInstance.IsValid() && gameInstance->netMainComponent &&
		gameInstance->netMainComponent->cvPlayerInfo.playerId != 0)
	{
		auto playerId = gameInstance->netMainComponent->cvPlayerInfo.playerId;
		if (playerMap.Find(playerId))
		{
			return playerMap[playerId];
		}
	}
	return nullptr;
}

// 获取 玩家
APawn* ABaseGameMode::GetPlayer(int playerId)
{
	if (gameInstance.IsValid())
	{
		if (playerMap.Find(playerId))
		{
			return playerMap[playerId];
		}
	}
	return nullptr;
}

// 获取 玩家同步数据
Gamedata::PlayerInfo* ABaseGameMode::GetPlayerSyncInfo(int playerId)
{
	auto it = playerSyncInfoMap.find(playerId);
	if (it != playerSyncInfoMap.end())
	{
		return &playerSyncInfoMap[playerId];
	}
	return nullptr;
}

// 新增 玩家同步数据
void ABaseGameMode::AddPlayerSyncInfo(int playerId, Gamedata::PlayerInfo* playerSyncInfo)
{
	RemovePlayerSyncInfo(playerId);
	playerSyncInfoMap.emplace(playerId, *playerSyncInfo);
}

// 移除 玩家同步数据
void ABaseGameMode::RemovePlayerSyncInfo(int playerId)
{
	auto it = playerSyncInfoMap.find(playerId);
	if (it != playerSyncInfoMap.end())
	{
		playerSyncInfoMap.erase(it);
	}
}

// 判断 是否是本地玩家
bool ABaseGameMode::IsLocalPlayer(int playerId)
{
	if (playerId != 0 && gameInstance->netMainComponent->cvPlayerInfo.playerId == playerId)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 判断 实例是否已经存在
bool ABaseGameMode::IsEntityExis(int entityId)
{
	if (editEntityManage && editEntityManage->data->GetSyncInfo(entityId))
	{
		return true;
	}
	return false;
}

// 主操作模式返回角色
ABaseGameMode::CvPawnType ABaseGameMode::GetPawnType(Gamedata::GameMainOperationMode mainOperationMode)
{
	if (mainOperationMode == Gamedata::GameMainOperationMode::GameMainOperationMode_Character
		|| mainOperationMode == Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity)
	{
		return ABaseGameMode::CvPawnType::CvPawnType_character;
	}
	else if (mainOperationMode == Gamedata::GameMainOperationMode::GameMainOperationMode_Racing)
	{
		return ABaseGameMode::CvPawnType::CvPawnType_racing;
	}
	return ABaseGameMode::CvPawnType::CvPawnType_character;
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
ABaseGameMode::ABaseGameMode()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HUDClass = ABaseHUD::StaticClass();

	controller = CreateDefaultSubobject<UBaseGameModeController>(TEXT("UBaseGameModeController"));
	levelManage = CreateDefaultSubobject<UBaseGameModeLevelManage>(TEXT("UBaseGameModeLevelManage"));

	DefaultPawnClass = nullptr;
	lastTimeFrameMap = 0;

	if (GetWorld() && GetGameInstance())
	{
		UGameInstance* gameInstance_ = GetGameInstance();
		UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseGameMode::ABaseGameMode] 实例化 [gameInstance:%x]"), this, gameInstance_);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseGameMode::ABaseGameMode] 构造 "), this);
	}
}

//
void ABaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

// Called when the game starts or when spawned
void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseGameMode::BeginPlay]  "), this);

	controller->Start(this);
	levelManage->Start(this);

	gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController);

	Start();
}

//
void ABaseGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	levelManage->End();
	controller->End();
	End();

	UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseGameMode::EndPlay]  "), this);
}

// 地图加载成功
void ABaseGameMode::EndLoadingScreen()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseGameMode::EndLoadingScreen] 地图加载成功 "), this);
	NetCall_JoinMap();
}

// 开始
void ABaseGameMode::Start()
{
	// 生成必要的actor
	{
		FTransform t;
		t.SetLocation(FVector::ZeroVector);
		t.SetRotation(FRotator::ZeroRotator.Quaternion());


		FString path = TEXT("Blueprint'/Game/Yxjs/BP/FunctionalModule/BP_SelectedViewActor.BP_SelectedViewActor_C'");
		UClass* objectClass = LoadClass<AActor>(NULL, *path);
		if (objectClass)
		{
			selectedViewActor = Cast<ASelectedViewActor>(
				UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), objectClass, t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
			if (selectedViewActor)
			{
				UGameplayStatics::FinishSpawningActor(selectedViewActor, t);
			}
		}
	}
	{
		FTransform t;
		t.SetLocation(FVector::ZeroVector);
		t.SetRotation(FRotator::ZeroRotator.Quaternion());
		editEntityManage = Cast<AEditEntityManage>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AEditEntityManage::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (editEntityManage)
		{
			UGameplayStatics::FinishSpawningActor(editEntityManage, t);
		}
	}
	{
		FTransform t;
		t.SetLocation(FVector::ZeroVector);
		t.SetRotation(FRotator::ZeroRotator.Quaternion());
		sceneStaticMeshView = Cast<ASceneStaticMeshView>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ASceneStaticMeshView::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (sceneStaticMeshView)
		{
			UGameplayStatics::FinishSpawningActor(sceneStaticMeshView, t);
		}
	}

	{
		FTransform t;
		t.SetLocation(FVector::ZeroVector);
		t.SetRotation(FRotator::ZeroRotator.Quaternion());
		actorTrajectoryView = Cast<AActorTrajectoryView>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AActorTrajectoryView::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (actorTrajectoryView)
		{
			UGameplayStatics::FinishSpawningActor(actorTrajectoryView, t);
		}
	}

	// skyControllerActor 需要再 AOverallSituationView 之前
	{
		FString path = TEXT("Blueprint'/Game/Yxjs/BP/FunctionalModule/ActorController/BP_Sky_Actor.BP_Sky_Actor_C'");
		UClass* objectClass = LoadClass<AActor>(NULL, *path);
		if (objectClass)
		{
			FTransform t;
			t.SetLocation(FVector::ZeroVector);
			t.SetRotation(FRotator::ZeroRotator.Quaternion());
			skyControllerActor = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), objectClass, t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (skyControllerActor)
			{
				UGameplayStatics::FinishSpawningActor(skyControllerActor, t);
			}
		}
	}
	{
		FTransform t;
		t.SetLocation(FVector::ZeroVector);
		t.SetRotation(FRotator::ZeroRotator.Quaternion());
		overallSituationView = Cast<AOverallSituationView>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AOverallSituationView::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (overallSituationView)
		{
			UGameplayStatics::FinishSpawningActor(overallSituationView, t);
		}
	}
}

// 结束
void ABaseGameMode::End()
{
	// 销毁必要的actor
	if (selectedViewActor)
	{
		selectedViewActor->Destroy();
		selectedViewActor = nullptr;
	}
	if (editEntityManage)
	{
		editEntityManage->Destroy();
		editEntityManage = nullptr;
	}
	if (sceneStaticMeshView)
	{
		sceneStaticMeshView->Destroy();
		sceneStaticMeshView = nullptr;
	}
	if (actorTrajectoryView)
	{
		actorTrajectoryView->Destroy();
		actorTrajectoryView = nullptr;
	}
	if (overallSituationView)
	{
		overallSituationView->Destroy();
		overallSituationView = nullptr;
	}
}

//
void ABaseGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (gameInstance.IsValid() && gameInstance->netMainComponent)
	{
		gameInstance->netMainComponent->TickFrame(DeltaSeconds);
	}
	if (gameInstance.IsValid() && gameInstance->onlineResourcesComponent)
	{
		gameInstance->onlineResourcesComponent->TickFrame(DeltaSeconds);
	}

	auto now = GetWorld()->TimeSeconds;
	if (now - lastTimeFrameMap > 10)
	{
		lastTimeFrameMap = now;
		FrameMap();
	}
}

// 帧函数 地图心跳
void ABaseGameMode::FrameMap()
{
	auto now = GetWorld()->TimeSeconds;
	//UE_LOG(LogTemp, Log, TEXT("[%x] [ABaseGameMode::FrameMap] test [now:%f]"), this, now);
}

// 请求 玩家加入地图
void ABaseGameMode::NetCall_JoinMap()
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;

	char* sendData = (char*)malloc(1024 * 10);
	int sendSize = 0;
	//MakePB_JoinMap(cvPlayerInfo.playerId, cvPlayerInfo.roomId, sendData, 1024 * 10, sendSize);

	{
		auto& cvPlayerInfo = netConnection->cvPlayerInfo;
		std::shared_ptr<Gamedata::JoinMapRequest> info = std::make_shared<Gamedata::JoinMapRequest>();
		info->set_roomid(cvPlayerInfo.roomId);
		info->set_playerid(cvPlayerInfo.playerId);

		info->SerializeToArray(sendData, 1024 * 10);
		sendSize = info->ByteSize();
	}

	NetRingBuffer::RingInfo info;
	info.data = sendData;
	info.dataSize = sendSize;
	info.type = (int)Gamedata::MsgType::PB_Hall_JoinMap;
	netConnection->netConnection->cvSocketSendInfo.ringBuffer->Write(info);
	free(sendData);
}

// 响应 大厅 玩家载入地图
void ABaseGameMode::NetCallback_LoadMap(char* data_, int dataSize)
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvPlayerInfo = netConnection->cvPlayerInfo;

	auto gameMode = UGameplayStatics::GetGameMode(gameInstance->GetWorld());
	auto playerController = UGameplayStatics::GetPlayerController(gameInstance->GetWorld(), 0);
	auto hud = playerController->GetHUD();

	Gamedata::JoinMapResponse info;
	bool flag1 = info.ParseFromArray(data_, dataSize);
	if (flag1 == false)return;

	FString msg = UTF8_TO_TCHAR(info.msg().c_str());

	UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_LoadMap]  [playerId:%d]  [msg:%s] "),
		this, cvPlayerInfo.playerId, *msg);
}

// 调试命令
void ABaseGameMode::DebugOrder(const FString& str)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [命令] [DebugOrder] [msg:%s] "), this, *str);
}