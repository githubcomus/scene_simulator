// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Import/CharacterInfo.h"
#include "Import/GameModeInfo.h"
#include "UI/BaseHUD.h"
#include "Net/NetInfo.h"
//#include "FunctionalModule/Selected/SelectedInfo.h"
//#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Protobuf/game.pb.h"
#include "Json.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "BaseGameModeController.generated.h"

// struct Gamedata::PlayerInfo;
class ABaseGameMode;

class UImage;



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UBaseGameModeController : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 发送消息队列数据
	struct  CvQueueInfo
	{
		// 执行时间
		float lastTime;
	};

	// 数据组件
	ABaseGameMode* data;

	// 发送消息队列数据
	CvQueueInfo cvQueueInfo;

	//
	FString skyLevelName = TEXT("MyUltraDynamicSkyMap");


	// 海洋 浅海 Map_Caustics_And_GodRays  深海 Map_Oceanology_QuadTree
	// 体积雾 雾气 ExplainVolumetricFog_NoiseStack 光辉 ExplainVolumetricFog_Global
	// 黑洞 
	FString volumetricLevelName = TEXT("Map_Caustics_And_GodRays");//Map_Caustics_And_GodRays // MyVolumetricMap //Level_03_no_terrain  ExplainVolumetricFog_Global Map_HeightMap

	// 线程标记
	bool flagConnect_Callback = false;

	/*------------------------------------------------------------------*/

	// Sets default values for this component's properties
	UBaseGameModeController();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void NetCall_GameData(EMsgType type, void* msgInfo);
	void Connect_Callback(bool isChildThread);
	//
	void Start(ABaseGameMode* data_);

	//
	void End();

	void NetCallback_PlayerEnterView(Gamedata::PlayerInfo& newSyncPlayerInfo);
	void NetCallback_PlayerUpdateView(Gamedata::PlayerInfo& syncPlayerInfo, bool isEnterView);
	void NetCallback_UpdataUI(Gamedata::PlayerInfo* oldSyncPlayerInfo, Gamedata::PlayerInfo* newSyncPlayerInfo, bool isEnterView);
	void NetCallback_PlayerLeaveView(int playerId_);
	void NetCallback_PlayerUpdateLocation(Gamedata::PlayerUpdateLocation& syncPlayerInfo);
	void NetCallback_EntityEnterView(Gamedata::EntityInfo* syncEntityInfo);
	void NetCallback_EntityLeaveView(int entityId_);
	void NetCallback_EntityUpdateView(Gamedata::EntityInfo* syncEntityInfo);

	void NetCallback_EntityOperation(Gamedata::EntityOperationTotal& info);
	void NetCallback_BP_OverallSituationData_Change(Gamedata::OverallSituationDataResponse& info);
	void NetCallback_PB_EventInfo(Gamedata::EventInfo& info);
	void SpawnCharacter(Gamedata::PlayerInfo& syncPlayerInfo);
	void DestroyCharacter(int playerId_);
	void UpdateCharacter(Gamedata::PlayerInfo& syncPlayerInfo, bool isEnterView);
	void MoveCharacter(Gamedata::PlayerUpdateLocation& playerUpdateLocation);
	void SpawnRacing(Gamedata::PlayerInfo& syncPlayerInfo);
	void DestroyRacing(int playerId_);
	void UpdateRacing(Gamedata::PlayerInfo& syncPlayerInfo);
	void MoveRacing(Gamedata::PlayerUpdateLocation& playerUpdateLocation);
	void SpawnTestBox();
	void OnlineResources_Callback_To_PlaceView(int32 entityId, FVector2D imageSize, UImage* image, TWeakObjectPtr<UTexture2D> texture2D);

	void TestFunc();
};
