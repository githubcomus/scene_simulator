// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Import/CharacterInfo.h"
#include "Pawn/Character/BaseCharacterInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "SimulationRacingDataComponent.generated.h"

class USimulationRacingControllerComponent;
class ASimulationRacingPawn;
class UZwSyncActorComponent;

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class ABaseHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  USimulationRacingDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:


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
	TWeakObjectPtr < USimulationRacingControllerComponent> controller;

	// 视图
	TWeakObjectPtr < ASimulationRacingPawn> view;

	//
	int playerId = 0;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this component's properties
	USimulationRacingDataComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	//
	void Start(USimulationRacingControllerComponent* controller2, ASimulationRacingPawn* view, int playerId_);

	//
	void End();

	bool CheckDefaultData();
	UNetMainComponent* GetNet();
	Gamedata::PlayerInfo* GetSyncInfo();

};
