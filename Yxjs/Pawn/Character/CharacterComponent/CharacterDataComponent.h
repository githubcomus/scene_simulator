// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Import/CharacterInfo.h"
#include "Json.h"
#include "Pawn/Character/BaseCharacterInfo.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "UI/BaseHUDInfo.h"
#include "CharacterDataComponent.generated.h"

class UCharacterControllerComponent;
class ABaseCharacter;
class UZwSyncActorComponent;
class UNetMainComponent;

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class ABaseHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UCharacterDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 按键信息
	struct InputKeyInfo
	{
		// 右键按下
		bool mouseRightEnter = false;

		// 左键按下
		bool mouseLeftEnter = false;

		// ctrl键按下
		bool ctrlEnter = false;

		// shift键按下
		bool shiftEnter = false;

		// 右键按下时鼠标位置
		FVector2D mousePoint;
	};

	// 滚轮信息
	struct MousePreviousInfo
	{
		//
		TArray<EEditUIBaseOrder>list;

		// now use
		int index;
	};

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 常用指针
	TWeakObjectPtr<UBaseGameInstance> gameInstance;

	// 常用指针
	TWeakObjectPtr<APlayerController> playerController;

	// 常用指针
	TWeakObjectPtr<ABaseGameMode> gameMode;

	// 常用指针
	TWeakObjectPtr<ABaseHUD> hud;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 控制器组件
	TWeakObjectPtr < UCharacterControllerComponent> controller;

	// 视图
	TWeakObjectPtr < ABaseCharacter> view;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 本地 编辑模式在移动或者在拖动实例
	bool isEditMovePawnOrMesh;

	// 本地 编辑模式的移动速度
	float editMoveSpeed;

	// 本地 编辑模式下的输入模式
	bool isEditAndAreaUI;

	//  本地 按键信息
	InputKeyInfo inputKeyInfo;

	// 本地 滚轮信息
	MousePreviousInfo mpi;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 全局 是否本地客户端
	bool isLocal;

	// 全局 最大血量
	int32 maxHealth = 100;

	// 全局 当前血量
	int32 health = 55;

	// 全局 玩家昵称
	FString playerName = "aa bb cc dd";

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 同步 playerId
	int playerId = 0;

	// 同步 操作状态
	//ECharacterStatusType characterStatus;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this component's properties
	UCharacterDataComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	//
	void Start(UCharacterControllerComponent* controller2, ABaseCharacter* view_, int playerId_, bool isLocal_, Gamedata::PlayerInfo& syncPlayerInfo_);

	//
	void End();

	bool CheckDefaultData();
	UNetMainComponent* GetNet();
	// 是否编辑状态
	bool IsEditEntityTransform();
	bool IsEditLightTimeline();
	bool IsEditMoveTimeline();
	bool IsPerviewState();
	ECharacterStatusType GetInputMode();

	// 返回实体ID
	int32 GetEntityId();

	// 是否为本地客户端
	bool IsLocalPlayer();
	Gamedata::PlayerInfo* GetSyncInfo();

	static void ParsingPB_PlayerEnterView(Gamedata::PlayerEnterView& info, TArray<Gamedata::PlayerInfo>& syncPlayerInfoList);
	static void ParsingPB_PlayerUpdateView(Gamedata::PlayerUpdateView& info, TArray<Gamedata::PlayerInfo>& syncPlayerInfoList);
	static void ParsingPB_PlayerLeaveView(Gamedata::PlayerLeaveView& info, TArray<int>& syncPlayerInfoList);
	void MakePB_UpdateLocationRequest(FVector location, FRotator rotation, EMovementMode movementMode, FVector velocity, char* outData, int dataSize, int& outSize, int& type);
	//void MakePB_ChangeJiugonggeRequest(FVector location, char* outData, int dataSize, int& outSize, int& type);
	//void MakePB_UpdatePlayerDataRequest(Gamedata::PlayerInfo& syncPlayerInfo_, char* outData, int dataSize, int& outSize, int& type);
};
