// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetInfo.h"
#include <string>
#include "Json.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Pawn/Character/BaseCharacterInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "NetDataObject.generated.h"

class UNetMainComponent;

/**
 *
 */
UCLASS()
class  UNetDataObject : public UObject
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 玩家Id
	// int playerId;

	/*------------------------------------------------------------------*/

	//
	UNetMainComponent* netMainComponent;

	//
	UNetDataObject();

	//
	void Start(UNetMainComponent* netMainComponent_);

	//
	void End();

	//void MakePB_PlayerLogin(FString account, FString password, char* outData, int dataSize, int& outSize);
	//void MakePB_PlayerLogout(int playerId, char* outData, int dataSize, int& outSize);
	//void MakePB_JoinRoom(int playerId, int roomId, char* outData, int dataSize, int& outSize);
	//void MakePB_JoinMap(int playerId, int roomId, char* outData, int dataSize, int& outSize);
	//void MakePB_LeaveRoom(int playerId, int roomId, char* outData, int dataSize, int& outSize);
};
