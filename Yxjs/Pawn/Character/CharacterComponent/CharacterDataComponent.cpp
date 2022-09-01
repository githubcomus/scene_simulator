// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterDataComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "PlayerInputComponent.h"

#include "GameMode/BaseGameMode.h"
#include "Core/BaseGameInstance.h"
#include "UI/BaseHud.h"
#include "GameFramework/PlayerController.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include "Tools/CommonlyObject.h"
#include "Kismet/GameplayStatics.h"
#include <concrt.h>
#include "Yxjs/Net/NetMainComponent.h"

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 检查必须的数据
bool UCharacterDataComponent::CheckDefaultData()
{
	bool checkTwice = false;
	if (gameInstance.IsValid() == false || gameMode.IsValid() == false
		|| playerController.IsValid() == false || hud.IsValid() == false)
	{
		gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController.IsValid())hud = Cast<ABaseHUD>(playerController->GetHUD());

		checkTwice = true;
	}

	bool flag = true;
	if (checkTwice)
	{
		if (gameInstance.IsValid() == false || gameMode.IsValid() == false
			|| playerController.IsValid() == false || hud.IsValid() == false)
		{
			flag = false;
		}
	}

	return flag;
}

//
UNetMainComponent* UCharacterDataComponent::GetNet()
{
	if (gameInstance.IsValid())
	{
		return gameInstance->netMainComponent;
	}
	return nullptr;
}

// 是否编辑实例状态
bool UCharacterDataComponent::IsEditEntityTransform()
{
	if (GetInputMode() == ECharacterStatusType::CharacterStatusType_EditEntity)
	{
		return true;
	}
	return false;
}

// 是否编辑灯光状态
bool UCharacterDataComponent::IsEditLightTimeline()
{
	//if (GetInputMode() == ECharacterStatusType::CharacterStatusType_EditLightLocation)
	//{
	//	return true;
	//}
	return false;
}

// 是否编辑时间轴状态
bool UCharacterDataComponent::IsEditMoveTimeline()
{
	if (GetInputMode() == ECharacterStatusType::CharacterStatusType_EditTimeline)
	{
		return true;
	}
	return false;
}

// 是否预览状态
bool UCharacterDataComponent::IsPerviewState()
{
	if (GetInputMode() == ECharacterStatusType::CharacterStatusType_Preview)
	{
		return true;
	}
	return false;
}

// 返回实体ID
int32 UCharacterDataComponent::GetEntityId()
{
	return playerId;
}

// 是否为本地客户端
bool UCharacterDataComponent::IsLocalPlayer()
{
	return isLocal;
}

// 返回同步数据
Gamedata::PlayerInfo* UCharacterDataComponent::GetSyncInfo()
{
	//return &syncPlayerInfo;
	//auto gameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	return gameMode->GetPlayerSyncInfo(playerId);
}

// 获取 当前操作模式设置
ECharacterStatusType UCharacterDataComponent::GetInputMode()
{
	//CvInputMode type = CvInputMode::set_player;
	//if (IsEditEntityTransform())
	//{
	//	type = ECharacterStatusType::CharacterStatusType_EditEntity;
	//}
	//else if (IsEditLightTimeline())
	//{
	//	type = ECharacterStatusType::CharacterStatusType_EditLightLocation;
	//}
	//else if (IsEditMoveTimeline())
	//{
	//	type = ECharacterStatusType_EditTimeline;
	//}
	//return characterStatus;
	//return type;
	return view->inputComp->inputMode;
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
UCharacterDataComponent::UCharacterDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	playerId = 0;
	isLocal = false;
	//jiugonggeKey = TEXT("");
	isEditMovePawnOrMesh = false;
	editMoveSpeed = 30;
	isEditAndAreaUI = false;
	inputKeyInfo.mouseRightEnter = false;
	inputKeyInfo.ctrlEnter = false;
	inputKeyInfo.mouseLeftEnter = false;

	mpi.index = 0;
	mpi.list.Add(EEditUIBaseOrder::WorldLocation);
	mpi.list.Add(EEditUIBaseOrder::WorldRotator);
	mpi.list.Add(EEditUIBaseOrder::Scale);
	mpi.list.Add(EEditUIBaseOrder::RelativelyLocation);
	mpi.list.Add(EEditUIBaseOrder::RelativelyRotator);
}

// Called when the game starts
void UCharacterDataComponent::BeginPlay()
{
	Super::BeginPlay();
}

//
void UCharacterDataComponent::Start(UCharacterControllerComponent* controller2, ABaseCharacter* view_
	, int playerId_, bool isLocal_, Gamedata::PlayerInfo& syncPlayerInfo_)
{
	CheckDefaultData();

	view = view_;
	controller = controller2;
	playerId = playerId_;
	isLocal = isLocal_;
	//syncPlayerInfo = syncPlayerInfo_;

	if (isLocal)
	{
		//auto location = GetOwner()->GetActorLocation();
		//jiugonggeKey = UCommonlyObject::GetJiugongge(location.X, location.Y, baseGameMode->syncSize);

		//jiugonggeKey = GetJiugongge(location.X, location.Y);
	};
}

//
void UCharacterDataComponent::End()
{
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 全部需要删除掉的内容

// protobuf解码 玩家进入视野
void UCharacterDataComponent::ParsingPB_PlayerEnterView(Gamedata::PlayerEnterView& info, TArray<Gamedata::PlayerInfo>& syncPlayerInfoList)
{
	int size = info.playerinfolist_size();
	for (int i = 0; i < size; i++)
	{
		auto node = info.playerinfolist()[i];

		syncPlayerInfoList.Add(node);

		// Gamedata::PlayerInfo syncPlayerInfo;
		// syncPlayerInfo.location = FVector(node.x(), node.y(), node.z());
		// syncPlayerInfo.rotator = FRotator(node.pitch(), node.yaw(), node.roll());
		// syncPlayerInfo.playerId = node.playerid();
		// syncPlayerInfo.status = node.status();
		// syncPlayerInfoList.Add(syncPlayerInfo);
	}
}

// protobuf解码 玩家更新
void UCharacterDataComponent::ParsingPB_PlayerUpdateView(Gamedata::PlayerUpdateView& info, TArray<Gamedata::PlayerInfo>& syncPlayerInfoList)
{
	int size = info.playerinfolist_size();
	for (int i = 0; i < size; i++)
	{
		auto node = info.playerinfolist()[i];

		syncPlayerInfoList.Add(node);
		// Gamedata::PlayerInfo syncPlayerInfo;
		// syncPlayerInfo.location = FVector(node.x(), node.y(), node.z());
		// syncPlayerInfo.rotator = FRotator(node.pitch(), node.yaw(), node.roll());
		// syncPlayerInfo.playerId = node.playerid();
		// syncPlayerInfo.status = node.status();
		// syncPlayerInfoList.Add(syncPlayerInfo);
	}
}

// protobuf解码 玩家更新
void UCharacterDataComponent::ParsingPB_PlayerLeaveView(Gamedata::PlayerLeaveView& info, TArray<int>& syncPlayerInfoList)
{
	int size = info.playeridlist_size();
	for (int i = 0; i < size; i++)
	{
		auto node = info.playeridlist()[i];
		syncPlayerInfoList.Add(node);
	}
}

// protobuf编码 位置更变
void UCharacterDataComponent::MakePB_UpdateLocationRequest(FVector location, FRotator rotation, EMovementMode movementMode, FVector velocity,
	char* outData, int dataSize, int& outSize, int& type)
{
	//shared_ptr<Gamedata::UpdateLocationRequest> info = std::make_shared<Gamedata::UpdateLocationRequest>();
	//info->set_playerid(playerId);
	//Gamedata::FVector* location2 = info->mutable_location();
	//location2->set_x(location.X);
	//location2->set_y(location.Y);
	//location2->set_z(location.Z);

	//info->set_yaw(rotation.Yaw);
	//info->set_charactermovementmodetype(Gamedata::CharacterMovementModeType(movementMode));
	//Gamedata::FVector* velocity2 = info->mutable_velocity();
	//velocity2->set_x(velocity.X);
	//velocity2->set_y(velocity.Y);
	//velocity2->set_z(velocity.Z);

	//info->SerializeToArray(outData, dataSize);
	//outSize = info->ByteSize();
	//type = (int)Gamedata::MsgType::PB_PlayerMoveRequest;
}

//// protobuf编码 九宫格更变
//void UCharacterDataComponent::MakePB_ChangeJiugonggeRequest(FVector location, char* outData, int dataSize, int& outSize, int& type)
//{
//	shared_ptr<Gamedata::ChangeJiugonggeRequest> info = std::make_shared<Gamedata::ChangeJiugonggeRequest>();
//	info->set_playerid(playerId);
//	Gamedata::FVector* location2 = info->mutable_location();
//	location2->set_x(location.X);
//	location2->set_y(location.Y);
//	location2->set_z(location.Z);
//	info->SerializeToArray(outData, dataSize);
//	outSize = info->ByteSize();
//	type = (int)Gamedata::MsgType::PB_PlayerJiugonggeRequest;
//}
//
//// protobuf编码 更新
//void UCharacterDataComponent::MakePB_UpdatePlayerDataRequest(Gamedata::PlayerInfo& syncPlayerInfo_, char* outData, int dataSize, int& outSize, int& type)
//{
//	shared_ptr<Gamedata::UpdatePlayerDataRequest> info = std::make_shared<Gamedata::UpdatePlayerDataRequest>();
//	info->set_playerid(playerId);
//
//	// shared_ptr<PlayerInfo> playerInfo = std::make_shared<PlayerInfo>();
//
//	// info->set_allocated_playerinfo(playerInfo.get());
//
//	auto playerInfo = info->mutable_playerinfo();
//
//	playerInfo->CopyFrom(syncPlayerInfo_);
//
//	info->SerializeToArray(outData, dataSize);
//	outSize = info->ByteSize();
//	type = (int)Gamedata::MsgType::PB_PlayerUpdateRequest;
//}

/*----------------------------------------------------*/
/*----------------------------------------------------*/