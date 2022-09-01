// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Net/NetInfo.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "BaseGameMode.generated.h"

/**
 *
 */
class ASelectedViewActor;
class AEditEntityManage;
class UBaseGameInstance;
class UNetMainComponent;
class UBaseGameModeController;
class UBaseGameModeLevelManage;

class ABaseCharacter;
class ASceneStaticMeshView;
class AActorTrajectoryView;
class AOverallSituationView;
class UCameraComponent;

UCLASS()
class  ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/*----------------------------------------------------*/

	// 角色
	enum CvPawnType
	{
		CvPawnType_character,

		CvPawnType_racing,
	};

	/*----------------------------------------------------*/

	// 同步数据队列

	//
	enum ERealSyncType
	{
		ERealSyncType_add,
		ERealSyncType_update,
		ERealSyncType_remove,
		ERealSyncType_none
	};

	// 同步数据
	struct RealSyncInfo
	{
		// 类型
		ERealSyncType type;

		// 数据
		Gamedata::EntityInfo entity;

		// 数据
		int entityId;
	};

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 控制器组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBaseGameModeController* controller = nullptr;

	// 关卡管理器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBaseGameModeLevelManage* levelManage = nullptr;

	//
	UPROPERTY()
		ASelectedViewActor* selectedViewActor = nullptr;

	//
	UPROPERTY()
		AEditEntityManage* editEntityManage = nullptr;

	//
	UPROPERTY()
		ASceneStaticMeshView* sceneStaticMeshView = nullptr;

	//
	UPROPERTY()
		AActorTrajectoryView* actorTrajectoryView = nullptr;

	//
	UPROPERTY()
		AOverallSituationView* overallSituationView = nullptr;

	// 天气控制器
	UPROPERTY()
		AActor* skyControllerActor = nullptr;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	 // 游戏实例
	TWeakObjectPtr < UBaseGameInstance>gameInstance = nullptr;

	// 最后执行时间
	float lastTimeFrameMap;

	// 全局 九宫格直径
	int syncSize = 400 * 100;

	// 同步数据缓存
	//std::list<RealSyncInfo> syncInfo;
	std::map<int, RealSyncInfo> syncInfoList;

	// 玩家字典
	UPROPERTY()
		TMap<int, APawn*> playerMap;

	// 玩家数据字典
	std::map<int, Gamedata::PlayerInfo> playerSyncInfoMap;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 本地玩家的摄像机位置(模拟玩家UI需要面朝摄像机)
	TWeakObjectPtr<UCameraComponent> localPlayerCameraComponet;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

public:

	/*------------------------------------------------------------------*/

	//
	ABaseGameMode();

	//
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	//
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	//
	virtual void Tick(float DeltaSeconds);

	void EndLoadingScreen();

	void Start();
	void End();
	void FrameMap();

	void NetCall_JoinMap();
	void NetCallback_LoadMap(char* data_, int dataSize);
	UFUNCTION(BlueprintCallable)
		void DebugOrder(const FString& str);
	int GetLocalPlayerId();
	APawn* GetLocalPlayer();
	APawn* GetPlayer(int playerId);
	void NetCallback_EntityOperation(Gamedata::EntityOperationTotal& info);

	Gamedata::PlayerInfo* GetPlayerSyncInfo(int playerId);
	void AddPlayerSyncInfo(int playerId, Gamedata::PlayerInfo* playerSyncInfo);
	void RemovePlayerSyncInfo(int playerId);
	bool IsLocalPlayer(int playerId);
	bool IsEntityExis(int entityId);
	ABaseGameMode::CvPawnType GetPawnType(Gamedata::GameMainOperationMode mainOperationMode);
};