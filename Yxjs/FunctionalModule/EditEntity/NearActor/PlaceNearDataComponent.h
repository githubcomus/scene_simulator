// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Import/CharacterInfo.h"
#include "Pawn/Character/BaseCharacterInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "PlaceNearDataComponent.generated.h"

class UPlaceNearControllerComponent;
class APlaceNearActorView;
class UZwSyncActorComponent;

class UBaseGameInstance;
class UPlayerController;
class ABaseGameMode;
class ABaseHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlaceNearDataComponent : public UActorComponent
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
	TWeakObjectPtr < UPlaceNearControllerComponent> controller;

	// 视图
	TWeakObjectPtr < APlaceNearActorView> view;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this component's properties
	UPlaceNearDataComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	//
	void Start(UPlaceNearControllerComponent* controller_, APlaceNearActorView* view_);

	//
	void End();

	bool CheckDefaultData();

};
