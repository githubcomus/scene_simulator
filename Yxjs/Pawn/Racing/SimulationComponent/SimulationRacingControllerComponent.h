// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Import/CharacterInfo.h"
#include "Pawn/Character/BaseCharacterInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "SimulationRacingControllerComponent.generated.h"

class USimulationRacingDataComponent;
class ASimulationRacingPawn;
class UNetMainComponent;

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class ABaseHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  USimulationRacingControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:


	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 数据组件
	TWeakObjectPtr<USimulationRacingDataComponent> data;

	// 视图
	TWeakObjectPtr < ASimulationRacingPawn> view;

	FVector oldLocation = FVector::ZeroVector;

	FRotator oldRotator = FRotator::ZeroRotator;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this component's properties
	USimulationRacingControllerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//
	void Start(USimulationRacingDataComponent* data_, ASimulationRacingPawn* view_);

	//
	void End();
	void FrameEditMouseIsUIArea();
	void NetCallback_PlayerUpdateLocation(Gamedata::PlayerUpdateLocation& syncPlayerInfo);
	//UNetMainComponent* GetNet();



	/*----------------------------------------------------*/
	/*----------------------------------------------------*/
};
