// Fill out your copyright notice in the Description page of Project Settings.

#include "NetDataObject.h"
#include "NetMainComponent.h"
#include "NetInfo.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"

#include "Protobuf/the_public.pb.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Json.h"
#include "Json/Public/Serialization/JsonSerializer.h"

#include <string>
//using namespace std;

//
UNetDataObject::UNetDataObject()
{
}

/*------------------------------------------------------------------*/

//
void UNetDataObject::Start(UNetMainComponent* netMainComponent_)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UNetDataObject::Start] "), this);
	netMainComponent = netMainComponent_;
	// this->AddToRoot();
}

//
void UNetDataObject::End()
{
	// this->RemoveFromRoot();
	UE_LOG(LogTemp, Log, TEXT("[%x] [UNetDataObject::End] "), this);
}

/*------------------------------------------------------------------*/
//
//// protobuf编码 玩家登陆
//void UNetDataObject::MakePB_PlayerLogin(FString account, FString password,char* outData,int dataSize , int& outSize)
//{
//	std::string account_(TCHAR_TO_UTF8(*account));
//	std::string password_(TCHAR_TO_UTF8(*password));
//
//	shared_ptr<Gamedata::PlayerLoginRequest> info = std::make_shared<Gamedata::PlayerLoginRequest>();
//	// PlayerLoginRequest hallRequest;
//	info->set_account(account_);
//	info->set_password(password_);
//
//	info->SerializeToArray(outData, dataSize);
//	outSize = info->ByteSize();
//}
//
//// protobuf编码 玩家加入房间
//void UNetDataObject::MakePB_PlayerLogout(int playerId,char* outData, int dataSize, int& outSize)
//{
//	shared_ptr<Gamedata::PlayerLogoutRequest> info = std::make_shared<Gamedata::PlayerLogoutRequest>();
//	info->set_playerid(playerId);
//
//	info->SerializeToArray(outData, dataSize);
//	outSize = info->ByteSize();
//}
//
//// protobuf编码 玩家加入房间
//void UNetDataObject::MakePB_JoinRoom(int playerId, int roomId, char* outData, int dataSize, int& outSize)
//{
//	shared_ptr<Gamedata::JoinRoomRequest> info = std::make_shared<Gamedata::JoinRoomRequest>();
//	info->set_roomid(roomId);
//	info->set_playerid(playerId);
//
//	info->SerializeToArray(outData, dataSize);
//	outSize = info->ByteSize();
//}
//
//// protobuf编码 玩家载入地图
//void UNetDataObject::MakePB_JoinMap(int playerId, int roomId, char* outData, int dataSize, int& outSize)
//{
//	shared_ptr<Gamedata::JoinMapRequest> info = std::make_shared<Gamedata::JoinMapRequest>();
//	info->set_roomid(roomId);
//	info->set_playerid(playerId);
//
//	info->SerializeToArray(outData, dataSize);
//	outSize = info->ByteSize();
//}
//
//// protobuf编码 玩家离开房间
//void UNetDataObject::MakePB_LeaveRoom(int playerId, int roomId, char* outData, int dataSize, int& outSize)
//{
//	shared_ptr<Gamedata::LeavelRoomRequest> info = std::make_shared<Gamedata::LeavelRoomRequest>();
//	info->set_roomid(roomId);
//	info->set_playerid(playerId);
//
//	info->SerializeToArray(outData, dataSize);
//	outSize = info->ByteSize();
//}

/*------------------------------------------------------------------*/
