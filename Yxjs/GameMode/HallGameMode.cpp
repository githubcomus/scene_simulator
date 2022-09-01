// Fill out your copyright notice in the Description page of Project Settings.

#include "HallGameMode.h"
//#include "Yxjs.h"
#include "Net/NetMainComponent.h"
#include "OnlineResources/OnlineResourcesComponent.h"
#include "Core/BaseGameInstance.h"
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
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../FunctionalModule/ActorTrajectory/ActorTrajectoryView.h"
#include "../FunctionalModule/OverallSituation/OverallSituationView.h"
#include "Camera/CameraComponent.h"
#include "../UI/HallHUD.h"
#include "../UI/Hall/Hall/Hall_Main.h"

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
AHallGameMode::AHallGameMode()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HUDClass = AHallHUD::StaticClass();
	DefaultPawnClass = nullptr;

	if (GetWorld() && GetGameInstance())
	{
		UGameInstance* gameInstance_ = GetGameInstance();
		UE_LOG(LogTemp, Log, TEXT("[%x] [AHallGameMode::AHallGameMode] 实例化 [gameInstance:%x]"), this, gameInstance_);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [AHallGameMode::AHallGameMode] 构造 "), this);
	}
}

//
void AHallGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

// Called when the game starts or when spawned
void AHallGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("[%x] [AHallGameMode::BeginPlay]  "), this);

	gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController);
	playerController->bShowMouseCursor = true;

	Test();
}

//
void AHallGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


	UE_LOG(LogTemp, Log, TEXT("[%x] [AHallGameMode::EndPlay]  "), this);
}

//
void AHallGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	netConnection->TickFrame(DeltaSeconds);
	if (flagConnect_Callback)
	{
		Connect_Callback(false);
	}
}

/*------------------------------------------------------------------*/

// 连接服务器 UI操作 发起连接
void AHallGameMode::UICall_StartConnect(const FString& account, const FString& password, const FString& ip, int port)
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvLoginInfo = netConnection->cvLoginInfo;

	cvLoginInfo.isConnectServer = true;

	cvLoginInfo.account = account;
	cvLoginInfo.password = password;
	cvLoginInfo.ip = ip;
	cvLoginInfo.port = port;
	netConnection->ConnectionServer(true);
	UE_LOG(LogTemp, Log, TEXT("[%x] [AHallGameMode::UICall_RquestLogin] 登陆服务器 [account:%s] [password:%s] [ip:%s]"),
		this, *account, *password, *ip);
}

// 连接服务器 连接成功 请求 
void AHallGameMode::NetCall_Login()
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvLoginInfo = netConnection->cvLoginInfo;

	char* sendData = (char*)malloc(1024 * 10);
	int sendSize = 0;
	MakePB_PlayerLogin(cvLoginInfo.account, cvLoginInfo.password, sendData, 1024 * 10, sendSize);
	NetRingBuffer::RingInfo info;
	info.data = sendData;
	info.dataSize = sendSize;
	info.type = (int)Gamedata::MsgType::PB_Hall_Login;
	netConnection->netConnection->cvSocketSendInfo.ringBuffer->Write(info);
	free(sendData);
}

// 连接服务器 连接成功 temp
void AHallGameMode::MakePB_PlayerLogin(FString account, FString password, char* outData, int dataSize, int& outSize)
{
	std::string account_(TCHAR_TO_UTF8(*account));
	std::string password_(TCHAR_TO_UTF8(*password));

	std::shared_ptr<Gamedata::PlayerLoginRequest> info = std::make_shared<Gamedata::PlayerLoginRequest>();
	info->set_account(account_);
	info->set_password(password_);

	info->SerializeToArray(outData, dataSize);
	outSize = info->ByteSize();
}

// 连接服务器 连接成功 响应 
void AHallGameMode::NetCallback_Login(char* data_, int dataSize)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AHallHUD* hud = Cast<AHallHUD>(playerController->GetHUD());

	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvLoginInfo = netConnection->cvLoginInfo;
	auto& cvPlayerInfo = netConnection->cvPlayerInfo;

	Gamedata::PlayerLoginResponse playerLoginResponse;
	bool flag1 = playerLoginResponse.ParseFromArray(data_, dataSize);
	if (flag1 == false)return;

	FString msg = UTF8_TO_TCHAR(playerLoginResponse.msg().c_str());

	// 登录成功,跳转到房间UI
	if (playerLoginResponse.code() == Gamedata::ReturnCode::PB_Successful)
	{
		cvPlayerInfo.playerId = playerLoginResponse.playerid();
		UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_Login]  [playerId:%d]  [msg:%s] "),
			this, cvPlayerInfo.playerId, *msg);
		// 对着gamemode发处理结果
		cvPlayerInfo.playerId = playerLoginResponse.playerid();
		cvLoginInfo.isLogin = true;

		hud->LoadRoomUI();
	}
	// 账号密码错误/版本号错误/主动断开连接
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_Login]  [playerId:%d]  [msg:%s] "),
			this, cvPlayerInfo.playerId, *msg);

		netConnection->ConnectionServer(false);

		if (UHall_Main* hallPtr = Cast<UHall_Main>(hud->hallPtr))
		{
			hallPtr->ResponseLoginFail();
		}
	}
}

// 链接服务器 UI操作 断开连接
void AHallGameMode::UICall_StopConnect()
{

	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;

	//1. 向服务器发送一个消息

	//2. 主动断开
	netConnection->ConnectionServer(false);
}

// 连接服务器 连接失败 回调
void AHallGameMode::Connect_Callback(bool isChildThread)
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

/*------------------------------------------------------------------*/

// 加入房间 UI调用 
void AHallGameMode::UICall_JoinRoom(int roomId)
{
	NetCall_JoinRoom(roomId);
}

// 加入房间 请求 
void AHallGameMode::NetCall_JoinRoom(int roomId)
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvLoginInfo = netConnection->cvLoginInfo;
	auto& cvPlayerInfo = netConnection->cvPlayerInfo;

	cvPlayerInfo.roomId = roomId;

	char* sendData = (char*)malloc(1024 * 10);
	int sendSize = 0;
	MakePB_JoinRoom(cvPlayerInfo.playerId, cvPlayerInfo.roomId, sendData, 1024 * 10, sendSize);
	NetRingBuffer::RingInfo info;
	info.data = sendData;
	info.dataSize = sendSize;
	info.type = (int)Gamedata::MsgType::PB_Hall_JoinRoom;
	netConnection->netConnection->cvSocketSendInfo.ringBuffer->Write(info);
	free(sendData);
}

// 加入房间 protobuf编码 玩家加入房间
void AHallGameMode::MakePB_JoinRoom(int playerId, int roomId, char* outData, int dataSize, int& outSize)
{
	std::shared_ptr<Gamedata::JoinRoomRequest> info = std::make_shared<Gamedata::JoinRoomRequest>();
	info->set_roomid(roomId);
	info->set_playerid(playerId);

	info->SerializeToArray(outData, dataSize);
	outSize = info->ByteSize();
}

// 加入房间 响应  
void AHallGameMode::NetCallback_JoinRoom(char* data_, int dataSize)
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvLoginInfo = netConnection->cvLoginInfo;
	auto& cvPlayerInfo = netConnection->cvPlayerInfo;

	auto gameMode = UGameplayStatics::GetGameMode(gameInstance->GetWorld());
	auto playerController = UGameplayStatics::GetPlayerController(gameInstance->GetWorld(), 0);
	auto hud = playerController->GetHUD();

	Gamedata::JoinRoomResponse info;
	bool flag1 = info.ParseFromArray(data_, dataSize);
	if (flag1 == false)return;

	FString msg = UTF8_TO_TCHAR(info.msg().c_str());

	bool flag = false;
	if (info.code() == Gamedata::ReturnCode::PB_Successful)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_JoinRoom]  [playerId:%d]  [msg:%s] "),
			this, cvPlayerInfo.playerId, *msg);
		flag = true;

		UGameplayStatics::OpenLevel(gameInstance->GetWorld(), FName(TEXT("GameMap")));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_JoinRoom]  [playerId:%d]  [msg:%s] "),
			this, cvPlayerInfo.playerId, *msg);

		netConnection->ConnectionServer(false);
	}

	FString cmd = FString::Printf(TEXT("Event_ResponseJoinRoom %d"), flag);
	FOutputDeviceDebug device;
	gameMode->CallFunctionByNameWithArguments(*cmd, device, nullptr, true);
}

/*------------------------------------------------------------------*/

// 离开房间 UI调用 
void AHallGameMode::UICall_LeavelRoom()
{
	NetCall_LeavelRoom();
}

// 离开房间 请求 
void AHallGameMode::NetCall_LeavelRoom()
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvLoginInfo = netConnection->cvLoginInfo;
	auto& cvPlayerInfo = netConnection->cvPlayerInfo;

	char* sendData = (char*)malloc(1024 * 10);
	int sendSize = 0;
	MakePB_LeaveRoom(cvPlayerInfo.playerId, cvPlayerInfo.roomId, sendData, 1024 * 10, sendSize);
	NetRingBuffer::RingInfo info;
	info.data = sendData;
	info.dataSize = sendSize;
	info.type = (int)Gamedata::MsgType::PB_Hall_LeaveRoom;
	netConnection->netConnection->cvSocketSendInfo.ringBuffer->Write(info);
	free(sendData);
}

// 离开房间 protobuf编码 玩家离开房间
void AHallGameMode::MakePB_LeaveRoom(int playerId, int roomId, char* outData, int dataSize, int& outSize)
{
	std::shared_ptr<Gamedata::LeavelRoomRequest> info = std::make_shared<Gamedata::LeavelRoomRequest>();
	info->set_roomid(roomId);
	info->set_playerid(playerId);

	info->SerializeToArray(outData, dataSize);
	outSize = info->ByteSize();
}

// 离开房间 响应  
void AHallGameMode::NetCallback_LeaveRoom(char* data_, int dataSize)
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvLoginInfo = netConnection->cvLoginInfo;
	auto& cvPlayerInfo = netConnection->cvPlayerInfo;

	auto gameMode = UGameplayStatics::GetGameMode(gameInstance->GetWorld());
	auto playerController = UGameplayStatics::GetPlayerController(gameInstance->GetWorld(), 0);
	auto hud = playerController->GetHUD();

	Gamedata::LeavelRoomResponse info;
	bool flag1 = info.ParseFromArray(data_, dataSize);
	if (flag1 == false)return;

	FString msg = UTF8_TO_TCHAR(info.msg().c_str());

	bool flag = false;
	if (info.code() == Gamedata::ReturnCode::PB_Successful)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_LeaveRoom]  [playerId:%d]  [msg:%s] "),
			this, cvPlayerInfo.playerId, *msg);
		flag = true;

		// 重置登陆界面
		UGameplayStatics::OpenLevel(gameInstance->GetWorld(), FName(TEXT("HallMap")));

		FString cmd = FString::Printf(TEXT("Event_ResponseLogin %d"), flag);
		FOutputDeviceDebug device;
		gameMode->CallFunctionByNameWithArguments(*cmd, device, nullptr, true);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [NetCallback_LeaveRoom]  [playerId:%d]  [msg:%s] "),
			this, cvPlayerInfo.playerId, *msg);
	}
}

/*------------------------------------------------------------------*/

//
void AHallGameMode::Test()
{

	FVector location(22, 33, 44);
	FRotator rotator(90, 3, 0);

	FTransform relativeTransform;
	relativeTransform.SetLocation(location);
	relativeTransform.SetRotation(rotator.Quaternion());
	relativeTransform.SetScale3D(FVector::OneVector);
	auto tempR2 = rotator;
	UE_LOG(LogTemp, Log, TEXT("[%x] [旋转] [UICallback_TimelineMove] AAA  [相对旋转:%f %f %f]"), this, tempR2.Pitch, tempR2.Yaw, tempR2.Roll);
	auto tempR3 = relativeTransform.GetRotation().Rotator();
	UE_LOG(LogTemp, Log, TEXT("[%x] [旋转] [UICallback_TimelineMove] BBB  [相对旋转:%f %f %f] "), this, tempR3.Pitch, tempR3.Yaw, tempR3.Roll);
}

/*------------------------------------------------------------------*/

// 需要移除的垃圾接口

// 体面离开服务器 登出退出服务器
void AHallGameMode::UICall_RquestLogout()
{
	auto gameMode = UGameplayStatics::GetGameMode(gameInstance->GetWorld());
	NetCall_Logout();
	UE_LOG(LogTemp, Log, TEXT("[%x] [AHallGameMode::UICall_RquestLogout] 登出服务器 "), this);
}

// 体面离开服务器 请求 登出
void AHallGameMode::NetCall_Logout()
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;
	auto& cvLoginInfo = netConnection->cvLoginInfo;
	auto& cvPlayerInfo = netConnection->cvPlayerInfo;

	char* sendData = (char*)malloc(1024 * 10);
	int sendSize = 0;
	MakePB_PlayerLogout(cvPlayerInfo.playerId, sendData, 1024 * 10, sendSize);
	NetRingBuffer::RingInfo info;
	info.data = sendData;
	info.dataSize = sendSize;
	info.type = (int)Gamedata::MsgType::PB_Hall_Logout;
	netConnection->netConnection->cvSocketSendInfo.ringBuffer->Write(info);
	free(sendData);
}

// 体面离开服务器 protobuf编码 玩家退出房间
void AHallGameMode::MakePB_PlayerLogout(int playerId, char* outData, int dataSize, int& outSize)
{
	std::shared_ptr<Gamedata::PlayerLogoutRequest> info = std::make_shared<Gamedata::PlayerLogoutRequest>();
	info->set_playerid(playerId);

	info->SerializeToArray(outData, dataSize);
	outSize = info->ByteSize();
}

// 体面离开服务器 响应 登出
void AHallGameMode::NetCallback_Logout(char* data_, int dataSize)
{
	auto netConnection = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->netMainComponent;
	if (netConnection == nullptr)return;

	auto gameMode = UGameplayStatics::GetGameMode(gameInstance->GetWorld());
	auto playerController = UGameplayStatics::GetPlayerController(gameInstance->GetWorld(), 0);
	auto hud = playerController->GetHUD();

	Gamedata::PlayerLogoutResponse info;
	bool flag1 = info.ParseFromArray(data_, dataSize);
	if (flag1 == false)return;

	FString msg = UTF8_TO_TCHAR(info.msg().c_str());

	netConnection->ConnectionServer(false);

	//FString cmd = FString::Printf(TEXT("Event_ResponseLogout"));
	//FOutputDeviceDebug device;
	//gameMode->CallFunctionByNameWithArguments(*cmd, device, nullptr, true);
}

