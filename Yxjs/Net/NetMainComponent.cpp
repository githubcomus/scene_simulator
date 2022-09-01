// Fill out your copyright notice in the Description page of Project Settings.

#include "NetMainComponent.h"
#include "NetConnection.h"
#include "Core/BaseGameInstance.h"
#include "NetDataObject.h"
#include "GameMode/BaseGameMode.h"
#include "GameMode/Component/BaseGameModeController.h"

#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
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
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "Protobuf/NetPBTest.h"

#include "Json/Public/Serialization/JsonSerializer.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Protobuf/the_public.pb.h"
#include "Protobuf/hall.pb.h"
#include "../GameMode/HallGameMode.h"

/*
链接模块
大厅消息请求响应处理
游戏消息请求响应解析
*/

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

// 获取 时间
float UNetMainComponent::GetTime()
{
	auto w = gameInstance->GetWorld();
	if (w)
	{
		return gameInstance->GetWorld()->TimeSeconds;
	}
	return 0;
}

// 获取 时间字符串
std::string UNetMainComponent::GetTimeStr()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UNetMainComponent::GetTimeStr] [world:%x] "), this, gameInstance->GetWorld());
	auto w = gameInstance->GetWorld();
	if (w)
	{
		float time = gameInstance->GetWorld()->TimeSeconds;
		char temp[20];
		sprintf_s(temp, 20, "%f", time);
		std::string str = temp;
		return str;
	}
	//else
	//{
	//    char temp[100];
	//    sprintf_s(temp, 100, "not world [this:%p]", this);
	//    string str = temp;
	//    return str;
	//}
	return "err";
}

// 获取 是否已经登陆
bool UNetMainComponent::IsLogin()
{
	return cvLoginInfo.isLogin;
}

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

// Sets default values for this component's properties
UNetMainComponent::UNetMainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	cvLoginInfo.isConnectServer = false;

	cvLoginInfo.isLogin = false;
	//cvNetDataQueue.mainTreadWorkLock = false;
	//cvNetDataQueue.dataLock = false;
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UNetMainComponent]  "),this);
}

// Called when the game starts
void UNetMainComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

//
void UNetMainComponent::Start(UBaseGameInstance* gameInstance_)
{
	gameInstance = gameInstance_;
	UE_LOG(LogTemp, Log, TEXT("[%x] [UNetMainComponent::Start] [owner:%x]"), this, GetOwner());
	netConnection = std::make_shared<NetConnection>(this);

	data = NewObject<UNetDataObject>(StaticClass());
	data->Start(this);

	netPBTest = std::make_shared<NetPBTest>(gameInstance_, this);

	recvData.data = (char*)malloc(RingInfo_MaxRecvDataSize);
}

//
void UNetMainComponent::End()
{
	if (cvLoginInfo.isConnectServer)
	{
		netConnection->End(true);
	}
	netConnection = nullptr;

	data->End();
	data = nullptr;

	free(recvData.data);
	recvData.data = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[%x] [UNetMainComponent::End] [world:%x]"), this, gameInstance->GetWorld());
}

/*------------------------------------------------------------------*/

// 服务器连接与断开
void UNetMainComponent::ConnectionServer(bool flag)
{
	if (flag)
	{
		std::string accountStr(TCHAR_TO_UTF8(*cvLoginInfo.account));
		std::string passwordStr(TCHAR_TO_UTF8(*cvLoginInfo.password));
		std::string ipStr(TCHAR_TO_UTF8(*cvLoginInfo.ip));
		netConnection->Start(ipStr, cvLoginInfo.port);
	}
	else
	{
		cvLoginInfo.isLogin = false;
		cvLoginInfo.isConnectServer = false;
		netConnection->End(true);
	}
}

/*------------------------------------------------------------------*/

// 接收消息 接收网络消息线程 写入数据
void UNetMainComponent::NetCallback_MsgBuf(int type, char* data_, int dataSize)
{
	// if (cvNetDataQueue.testLock)
	// 	return;

	auto world = gameInstance->GetWorld();
	if (IsValid(gameInstance) == false
		|| world == nullptr
		|| IsValid(world) == false)
	{
		return;
	}

	//if (type == Gamedata::MsgType::PB_Hall_Login
	//	|| type == Gamedata::MsgType::PB_Hall_Logout
	//	|| type == Gamedata::MsgType::PB_Hall_JoinRoom
	//	|| type == Gamedata::MsgType::PB_Hall_JoinMap
	//	|| type == Gamedata::MsgType::PB_Hall_LeaveRoom
	//	|| type == Gamedata::MsgType::PB_PlayerEnterView
	//	|| type == Gamedata::MsgType::PB_PlayerLeaveView) {
	//}
	//else {
	//	UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_MsgBuf]  [type:%d]  [time:%f]"),
	//		this, type, gameInstance->GetWorld()->TimeSeconds);
	//	return;
	//}

	cvNetDataQueue.lastTimeNetCallback_MsgBuf = gameInstance->GetWorld()->TimeSeconds;

	NetRingBuffer::RingInfo info;
	info.data = data_;
	info.dataSize = dataSize;
	info.type = type;
	netConnection->cvSocketRecvInfo.ringBuffer->Write(info);

	// 立刻通知线程读取
	cvNetDataQueue.lastTimeReadData = true;
}

// 接收消息 主线程 响应消息读取
void UNetMainComponent::TickFrame(float DeltaTime)
{
	bool read = false;
	auto nowTime = gameInstance->GetWorld()->TimeSeconds;
	if ((nowTime - cvNetDataQueue.lastTime) > 0.05)
	{
		cvNetDataQueue.lastTime = nowTime;
		read = true;
	}
	// if (cvNetDataQueue.testLock)
	// 	return;

	if (read || cvNetDataQueue.lastTimeReadData)
	{
		if (recvData.data && netConnection->cvSocketRecvInfo.ringBuffer->Read(recvData))
		{
			cvNetDataQueue.lastTimeReadData = true;

			NetCallback_MsgBuf2(recvData.type, recvData.data, recvData.dataSize);

			// if (recvData.data != nullptr)free(recvData.data);
		}
		else
		{
			cvNetDataQueue.lastTimeReadData = false;
		}
	}

	// 心跳帧可以写在这里减少一条线程
}

// 接收消息 主线程 总响应数据
void UNetMainComponent::NetCallback_MsgBuf2(int type, char* data_, int dataSize)
{
	auto myGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(gameInstance->GetWorld()));
	auto hallGameMode = Cast<AHallGameMode>(UGameplayStatics::GetGameMode(gameInstance->GetWorld()));
	//if (gameMode == nullptr)return;

	auto nowTime = gameInstance->GetWorld()->TimeSeconds;;
	auto var = nowTime - cvNetDataQueue.lastTimeNetCallback_MsgBuf;
	// UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_MsgBuf2]  [time:%f----%f] [耗时:%f] "),
	//        this, cvNetDataQueue.lastTimeNetCallback_MsgBuf, nowTime, var);

	//if (type== Gamedata::MsgType::PB_Hall_Login
	//	|| type == Gamedata::MsgType::PB_Hall_Logout
	//	|| type == Gamedata::MsgType::PB_Hall_JoinRoom
	//	|| type == Gamedata::MsgType::PB_Hall_JoinMap
	//	|| type == Gamedata::MsgType::PB_Hall_LeaveRoom
	//	|| type == Gamedata::MsgType::PB_PlayerEnterView
	//	|| type == Gamedata::MsgType::PB_PlayerLeaveView) {
	//
	//}
	//else {
	//	return;
	//}
	//TArray<Gamedata::EntityInfo> syncEntityInfoList;

	switch (type)
	{
		/*----------------------------------------------------*/
		/*----------------------------------------------------*/

	case Gamedata::MsgType::PB_Hall_Login:
		hallGameMode->NetCallback_Login(data_, dataSize);
		break;
	case Gamedata::MsgType::PB_Hall_Logout:
		hallGameMode->NetCallback_Logout(data_, dataSize);
		break;
	case Gamedata::MsgType::PB_Hall_JoinRoom:
		hallGameMode->NetCallback_JoinRoom(data_, dataSize);
		break;
	case Gamedata::MsgType::PB_Hall_LeaveRoom:
		hallGameMode->NetCallback_LeaveRoom(data_, dataSize);
		break;

		/*----------------------------------------------------*/
		/*----------------------------------------------------*/

	case Gamedata::MsgType::PB_Hall_JoinMap:
		myGameMode->NetCallback_LoadMap(data_, dataSize);
		break;
	case Gamedata::MsgType::PB_PlayerEnterView:
	{
		Gamedata::PlayerEnterView info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			TArray<Gamedata::PlayerInfo> syncList;
			UCharacterDataComponent::ParsingPB_PlayerEnterView(info, syncList);
			for (auto node : syncList)
			{
				myGameMode->controller->NetCallback_PlayerEnterView(node);
			}
		}
		break;
	}
	case Gamedata::MsgType::PB_PlayerLeaveView:
	{
		Gamedata::PlayerLeaveView info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			TArray<int> syncList;
			UCharacterDataComponent::ParsingPB_PlayerLeaveView(info, syncList);
			for (auto node : syncList)
			{
				myGameMode->controller->NetCallback_PlayerLeaveView(node);
			}
		}
		break;
	}
	case Gamedata::MsgType::PB_PlayerUpdateView:
	{
		Gamedata::PlayerUpdateView info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			TArray<Gamedata::PlayerInfo> syncList;
			UCharacterDataComponent::ParsingPB_PlayerUpdateView(info, syncList);
			for (auto node : syncList)
			{
				myGameMode->controller->NetCallback_PlayerUpdateView(node, false);
			}
		}
		break;
	}
	case Gamedata::MsgType::PB_PlayerUpdateLocation:
	{
		Gamedata::PlayerUpdateLocation info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			myGameMode->controller->NetCallback_PlayerUpdateLocation(info);
		}
		break;
	}
	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	case Gamedata::MsgType::PB_EntityEnterView:
	{
		Gamedata::EntityEnterView info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			int size = info.entityinfolist_size();
			for (int i = 0; i < size; i++)
			{
				auto node = info.entityinfolist()[i];
				auto it = myGameMode->syncInfoList.find(node.entityid());
				// 待处理任务
				if (it != myGameMode->syncInfoList.end())
				{
					myGameMode->syncInfoList.erase(it);
				}
				// 不存在于视野
				if (myGameMode->IsEntityExis(node.entityid()) == false)
				{
					ABaseGameMode::RealSyncInfo realSyncInfo;
					realSyncInfo.type = ABaseGameMode::ERealSyncType::ERealSyncType_add;
					realSyncInfo.entityId = node.entityid();
					realSyncInfo.entity.CopyFrom(node);
					myGameMode->syncInfoList.emplace(realSyncInfo.entityId, realSyncInfo);
				}
			}
		}
		break;
	}
	case Gamedata::MsgType::PB_EntityLeaveView:
	{
		Gamedata::EntityLeaveView info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			int size = info.entityidlist_size();
			for (int i = 0; i < size; i++)
			{
				auto node = info.entityidlist()[i];

				auto it = myGameMode->syncInfoList.find(node);
				// 待处理任务
				if (it != myGameMode->syncInfoList.end())
				{
					myGameMode->syncInfoList.erase(it);
				}
				// 存在于视野
				if (myGameMode->IsEntityExis(node))
				{
					ABaseGameMode::RealSyncInfo realSyncInfo;
					realSyncInfo.type = ABaseGameMode::ERealSyncType::ERealSyncType_remove;
					realSyncInfo.entityId = node;
					myGameMode->syncInfoList.emplace(realSyncInfo.entityId, realSyncInfo);
				}
			}
		}
		break;
	}
	case Gamedata::MsgType::PB_EntityUpdateView:
	{
		Gamedata::EntityUpdateView info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			int size = info.entityinfolist_size();
			for (int i = 0; i < size; i++)
			{
				auto node = info.entityinfolist()[i];

				auto it = myGameMode->syncInfoList.find(node.entityid());
				// 待处理任务
				if (it != myGameMode->syncInfoList.end())
				{
					if (it->second.type == ABaseGameMode::ERealSyncType::ERealSyncType_update)
					{
					}
					else if (it->second.type == ABaseGameMode::ERealSyncType::ERealSyncType_remove)
					{
					}
					else if (it->second.type == ABaseGameMode::ERealSyncType::ERealSyncType_add)
					{
					}
				}
				// 真实新增
				else
				{
					ABaseGameMode::RealSyncInfo realSyncInfo;
					realSyncInfo.type = ABaseGameMode::ERealSyncType::ERealSyncType_update;
					realSyncInfo.entityId = node.entityid();
					realSyncInfo.entity.CopyFrom(node);
					myGameMode->syncInfoList.emplace(realSyncInfo.entityId, realSyncInfo);
				}
			}
		}
		break;
	}
	case Gamedata::MsgType::PB_EntityOperation:
	{
		Gamedata::EntityOperationTotal info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			myGameMode->controller->NetCallback_EntityOperation(info);
		}
		break;
	}

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	//case Gamedata::MsgType::BP_OverallSituationData_EnterView:
	//{
	//	Gamedata::OverallSituationData info;
	//	if (gameMode && info.ParseFromArray(data_, dataSize)) {
	//		gameMode->controller->NetCallback_BP_OverallSituationData_EnterView(info);
	//	}
	//	break;
	//}
	case Gamedata::MsgType::BP_OverallSituationData_Change:
	{
		Gamedata::OverallSituationDataResponse info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			myGameMode->controller->NetCallback_BP_OverallSituationData_Change(info);
		}
		break;
	}
	case Gamedata::MsgType::PB_EventInfo:
	{
		Gamedata::EventInfo info;
		if (myGameMode && info.ParseFromArray(data_, dataSize))
		{
			myGameMode->controller->NetCallback_PB_EventInfo(info);
		}
		break;
	}
	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	default:
		break;
	}
}

/*------------------------------------------------------------------*/

// 发送消息 房间业务数据
void UNetMainComponent::NetCall_GameData(int type, char* data_, int dataSize)
{
	cvNetDataQueue.lastTimeSendData = gameInstance->GetWorld()->TimeSeconds;

	NetRingBuffer::RingInfo info;
	info.data = data_;
	info.dataSize = dataSize;
	info.type = type;
	netConnection->cvSocketSendInfo.ringBuffer->Write(info);
}

/*------------------------------------------------------------------*/

// 连接线程回调 连接成功 登陆
void UNetMainComponent::Callback_ConnectSuccess()
{
	auto hallGameMode = Cast<AHallGameMode>(UGameplayStatics::GetGameMode(gameInstance->GetWorld()));
	if (hallGameMode)
	{
		hallGameMode->NetCall_Login();
	}
}

// 连接线程回调 连接失败 关闭整个连接
void UNetMainComponent::Callback_ConnectFailed()
{

	if (AHallGameMode* hallGameMode = Cast<AHallGameMode>(UGameplayStatics::GetGameMode(gameInstance->GetWorld())))
	{
		hallGameMode->Connect_Callback(true);
	}
	else if (ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(gameInstance->GetWorld())))
	{
		baseGameMode->controller->Connect_Callback(true);
	}
}

/*------------------------------------------------------------------*/

// 测试 发送一条规范的消息
void UNetMainComponent::TestSendData()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UNetMainComponent::TestSendData] [world:%x] "), this, gameInstance->GetWorld());
	netConnection->TestSendData();
}

// 请求 测试
void UNetMainComponent::NetCall_Test()
{
	// string str3 = "ffffffffuck";
	// NetRingBuffer::RingInfo info;
	// info.str = str3;
	// netConnection->cvSocketSendInfo.ringBuffer->Write(info);
}

/*------------------------------------------------------------------*/

// 显示日志
void UNetMainComponent::ShowLog(std::string str)
{
	// 解决中文乱码
	FString str2 = UTF8_TO_TCHAR(str.c_str());
	UE_LOG(LogTemp, Log, TEXT("%s"), *str2);

	//UE_LOG(LogTemp, Log, TEXT("[%x] [UNetMainComponent::ShowLog] [world:%x] "), this, gameInstance->GetWorld());
}

/*------------------------------------------------------------------*/