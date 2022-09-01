// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Import/CharacterInfo.h"
#include "Pawn/Character/BaseCharacterInfo.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Json.h"
#include "Protobuf/game.pb.h"
#include "GameMode/BaseGameMode.h"
#include "UI/BaseHud.h"
#include "CharacterControllerComponent.generated.h"

class ABaseCharacter;
class UCharacterDataComponent;
class UZwSyncActorComponent;
class ABaseCharacter;
class UBaseGameInstance;
class AEditEntityManage;

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class ABaseHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UCharacterControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 控制部分信息
	struct CvControllerInfo
	{
		// 最后执行时间
		float lastTime;

		// 上一次位置
		FVector lastLocation;

		// 上一次旋转
		FRotator lastRotator;

		// 上一次采集九宫格数据
		float lastTimeJiugongge;

		// 上一次速度
		FVector lastVelocity;
	};

	// 屏幕信息
	struct CvViewportInfo
	{
		//
		int mouseX;

		//
		int mouseY;
	};

	// 登录保护
	struct CvLoadProtectInfo
	{

		// 定时器
		FTimerHandle timerHandle_DelayRun;

		//
		float startTime = 0;
	};

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 数据组件
	TWeakObjectPtr<UCharacterDataComponent> data;

	// 视图
	TWeakObjectPtr <ABaseCharacter> view;

	// 控制部分信息
	CvControllerInfo cvControllerInfo;

	// 屏幕信息
	CvViewportInfo cvi;

	// 登录保护
	CvLoadProtectInfo loadProtectInfo;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/


	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	TWeakObjectPtr<AEditEntityManage> GetEditEntityManage();
	// Sets default values for this component's properties
	UCharacterControllerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//
	void Start(UCharacterDataComponent* data_, ABaseCharacter* view_);

	//
	void End();
	void PossessedBy();
	void LoadToGameProtect_Run(bool isStart);
	void LoadToGameProtect_Work(bool isStart);
	bool EntityIsLoadOver();
	void FrameEditMouseIsUIArea();
	void EditModeInUIArea(bool flag);
	void NetCall_Move();
	void NetCallback_UpdatePlayerInfo(Gamedata::PlayerInfo& syncPlayerInfo, bool isEnterView);

	void LoadToGameProtect();
	void NetCallback_Move(Gamedata::PlayerUpdateLocation& syncPlayerInfo);
	void FrameGetSyncTransform();

	void Function_LeaveRoom();
	void Function_OpenBackpack();
	void Input_OpenTimelineUI();
	void Input_ChangePawnType();
	//void Call_EnterStaus(bool isEnter);
	void NetCall_UpdatePlayerInfo(Gamedata::PlayerInfo& newSyncInfo);

	/*----------------------------------------------------*/
};
