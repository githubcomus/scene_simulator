// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Net/NetInfo.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "HallGameMode.generated.h"

/**
 *
 */
class ASelectedViewActor;
class AEditEntityManage;
class UBaseGameInstance;
class UNetMainComponent;
class UHallGameModeController;
class ABaseCharacter;
class ASceneStaticMeshView;
class AActorTrajectoryView;
class AOverallSituationView;
class UCameraComponent;

UCLASS()
class  AHallGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	 // 游戏实例
	TWeakObjectPtr < UBaseGameInstance>gameInstance = nullptr;

	// 线程标记
	bool flagConnect_Callback = false;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

public:

	/*------------------------------------------------------------------*/

	//
	AHallGameMode();

	//
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	//
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	//
	virtual void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
		void UICall_StartConnect(const FString& account, const FString& password, const FString& ip, int port);

	UFUNCTION(BlueprintCallable)
		void UICall_JoinRoom(int roomId);

	UFUNCTION(BlueprintCallable)
		void UICall_LeavelRoom();

	UFUNCTION(BlueprintCallable)
		void UICall_RquestLogout();

	void NetCall_Login();
	void NetCall_Logout();

	//void NetCall_CreateRoom(int serverId, int playerId, FString map, FString roomName);

	void NetCall_JoinRoom(int roomId);

	void NetCall_LeavelRoom();

	//void NetCall_JoinMap();

	void NetCallback_Login(char* data_, int dataSize);
	void UICall_StopConnect();
	void Connect_Callback(bool isChildThread);
	void NetCallback_Logout(char* data_, int dataSize);
	void NetCallback_JoinRoom(char* data_, int dataSize);
	//void NetCallback_LoadMap(char* data_, int dataSize);
	void NetCallback_LeaveRoom(char* data_, int dataSize);

	void Test();
	void MakePB_PlayerLogin(FString account, FString password, char* outData, int dataSize, int& outSize);
	void MakePB_PlayerLogout(int playerId, char* outData, int dataSize, int& outSize);
	void MakePB_JoinRoom(int playerId, int roomId, char* outData, int dataSize, int& outSize);
	//void MakePB_JoinMap(int playerId, int roomId, char* outData, int dataSize, int& outSize);
	void MakePB_LeaveRoom(int playerId, int roomId, char* outData, int dataSize, int& outSize);
};