
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Protobuf/game.pb.h"
#include "SimulationRacingPawn.generated.h"


class USimulationRacingDataComponent;
class USimulationRacingControllerComponent;

class USpringArmComponent;
class UCameraComponent;
class USplineComponent;
class UWidgetComponent;


UCLASS()
class  ASimulationRacingPawn : public ACharacter
{
	GENERATED_BODY()

public:

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// 数据组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USimulationRacingDataComponent* dataComp;

	// 控制器组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USimulationRacingControllerComponent* controllerComp;

	// 血条
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWidgetComponent* widgetComponent;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	//
	int oldSpeed = 0;

	/*----------------------------------------------------*/
	/*----------------------------------------------------*/

	// Sets default values for this character's properties
	ASimulationRacingPawn(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitUi();
	//
	void Start(int playerId_);

	//
	void End();


	/*----------------------------------------------------*/
	/*----------------------------------------------------*/
};
