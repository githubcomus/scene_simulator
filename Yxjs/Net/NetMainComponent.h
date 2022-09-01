// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetInfo.h"
//#include "Json/Public/Serialization/JsonSerializer.h"
#include <atomic>
#include <string>
#include <list>
#include "Pawn/Character/BaseCharacterInfo.h"
#include "NetConnection.h"
#include "NetRingBuffer.h"

#include "NetMainComponent.generated.h"

//using namespace std;

class NetPBTest;
class UBaseGameInstance;
class NetConnection;
class UNetDataObject;

#define RingInfo_MaxRecvDataSize 1024*1024

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UNetMainComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// 登陆信息
	struct CvLoginInfo
	{
		// 是否已经连接服务器
		bool isConnectServer;

		// 账号
		FString account;

		// 密码
		FString password;

		// 是否登陆
		bool isLogin;

		// IP
		FString ip;

		// prot
		int port;
	};

	// 玩家信息
	struct CvPlayerInfo
	{
		// 玩家Id
		int playerId;

		// 房间Id
		int roomId;
	};

	// 网络数据队列
	struct CvNetDataQueue
	{
		float lastTime = 0;

		// 上一次是否读取到数据
		bool lastTimeReadData = false;

		// 接收数据处理耗时
		float lastTimeNetCallback_MsgBuf = 0;

		// 发送数据处理耗时
		float lastTimeSendData = 0;
	};

	/*------------------------------------------------------------------*/

	// owner
	UBaseGameInstance* gameInstance;

	// 连接网络组件
	std::shared_ptr<NetConnection> netConnection;

	// data
	UPROPERTY()
		UNetDataObject* data;

	// 登陆信息
	CvLoginInfo cvLoginInfo;

	// 玩家信息
	CvPlayerInfo cvPlayerInfo;

	//
	std::shared_ptr<NetPBTest> netPBTest;

	// 队列数据
	CvNetDataQueue cvNetDataQueue;

	// 读缓存
	NetRingBuffer::RingInfo recvData;

	/*------------------------------------------------------------------*/

	// Sets default values for this component's properties
	UNetMainComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// 主线程队列 处理来自网络的消息
	UFUNCTION(BlueprintCallable)
		void TickFrame(float DeltaTime);

	//
	void Start(UBaseGameInstance* gameInstance_);

	//
	void End();

	//
	void ShowLog(std::string str);

	// 获取
	float GetTime();

	// 获取 时间字符串
	std::string GetTimeStr();

	// 获取 是否已经登陆
	UFUNCTION(BlueprintCallable)
		bool IsLogin();
	void ConnectionServer(bool flag);

	void NetCall_Test();
	void NetCallback_MsgBuf(int type, char* data_, int dataSize);
	void NetCallback_MsgBuf2(int type, char* data_, int dataSize);

	void Callback_ConnectSuccess();
	void Callback_ConnectFailed();
	void NetCall_GameData(int type, char* data_, int dataSize);

	void NetCallback_Game_SyncEntityInfo(TSharedPtr<FJsonObject> jsonParsed,
		int32 playerId, int32 code, FString& msg);

	void TestSendData();

	/*------------------------------------------------------------------*/
};
