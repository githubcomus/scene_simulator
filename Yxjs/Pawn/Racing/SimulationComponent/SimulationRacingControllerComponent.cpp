// Fill out your copyright notice in the Description page of Project Settings.

#include "SimulationRacingControllerComponent.h"
#include "Pawn/Racing/Simulation/SimulationRacingPawn.h"
#include "Pawn/Racing/SimulationComponent/SimulationRacingDataComponent.h"
#include "Net/NetMainComponent.h"

#include "GameMode/BaseGameMode.h"
#include "Core/BaseGameInstance.h"
#include "UI/BaseHud.h"
#include "GameFramework/PlayerController.h"

//#include "JsonSerializer.h"
//#include "WidgetBlueprintLibrary.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "Tools/CommonlyObject.h"
#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include <AIModule/Classes/AIController.h>
#include "GameFramework/CharacterMovementComponent.h"
//#include <../Plugins/Runtime/PhysXVehicles/Source/PhysXVehicles/Public/WheeledVehicleMovementComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
USimulationRacingControllerComponent::USimulationRacingControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//UE_LOG(LogTemp, Log, TEXT("[%x] [USimulationRacingControllerComponent]  "),this);
}

// Called when the game starts
void USimulationRacingControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

//
void USimulationRacingControllerComponent::Start(USimulationRacingDataComponent* data_, ASimulationRacingPawn* view_)
{
	view = view_;
	data = data_;

	data->CheckDefaultData();
}

//
void USimulationRacingControllerComponent::End()
{
}

// Called every frame
void USimulationRacingControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 网络回调 更新玩家位置
void USimulationRacingControllerComponent::NetCallback_PlayerUpdateLocation(Gamedata::PlayerUpdateLocation& playerUpdateLocation)
{
	if (data->CheckDefaultData() == false)return;

	auto localPlayerId = data->GetNet()->cvPlayerInfo.playerId;
	auto moveComponent = Cast<UCharacterMovementComponent>(view->GetMovementComponent());

	data->GetSyncInfo()->mutable_playerlocaioninfo()->mutable_transform_s()->CopyFrom(playerUpdateLocation.playerlocaioninfo().transform_s());
	data->GetSyncInfo()->mutable_playerlocaioninfo()->mutable_velocity()->CopyFrom(playerUpdateLocation.playerlocaioninfo().velocity());
	data->GetSyncInfo()->mutable_playerlocaioninfo()->set_charactermovementmodetype(playerUpdateLocation.playerlocaioninfo().charactermovementmodetype());

	{
		auto velocity = playerUpdateLocation.playerlocaioninfo().velocity();
		FVector velocity2(velocity.x(), velocity.y(), velocity.z());
		auto location = playerUpdateLocation.playerlocaioninfo().transform_s().location();
		FVector location2(location.x(), location.y(), location.z());
		auto rotator = playerUpdateLocation.playerlocaioninfo().transform_s().rotator();
		FRotator rotator2(rotator.pitch(), rotator.yaw(), rotator.roll());

		EMovementMode movementMode = EMovementMode(playerUpdateLocation.playerlocaioninfo().charactermovementmodetype());
		//auto moveSize = (location2 - view->GetActorLocation()).Size();

		velocity2 = velocity2 * (2.0 / 4.0);

		//UKismetSystemLibrary::DrawDebugCapsule(GetOwner(), location2, 10, 10, FRotator::ZeroRotator, FLinearColor::Red, 2, 10);

		auto replicatedmovement = view->GetReplicatedMovement();
		replicatedmovement.Location = location2;
		replicatedmovement.Rotation = rotator2;
		replicatedmovement.LinearVelocity = velocity2;
		replicatedmovement.AngularVelocity = FVector::ZeroVector;
		view->SetReplicatedMovement(replicatedmovement);

		//auto player_racing_info = playerUpdateLocation.playerlocaioninfo().player_racing_info();
		//moveComponent->SetReplicatedVehicleState(player_racing_info.steering_input(),
		//	player_racing_info.throttle_input(),
		//	player_racing_info.brake_input(), player_racing_info.handbrake_input(), player_racing_info.current_gear());

		//if (oldLocation.Equals(FVector::ZeroVector)) {
		//	oldLocation = view->GetActorLocation();
		//	oldRotator = view->GetActorRotation();
		//}

		//FVector moveL = oldLocation;
		//FRotator moveR = oldRotator;
		//moveComponent->NetworkSmoothingMode = ENetworkSmoothingMode::Linear;
		//moveComponent->Velocity = velocity2* (1.0 / 4.0);
		//moveComponent->Velocity = velocity2.Rotation().Vector()* 20;
		//moveComponent->Velocity = FVector(0, 0, 0);

		//moveComponent->SmoothCorrection(
		//	moveL, moveR.Quaternion(),
		//	location2, rotator2.Quaternion()
		//);

		moveComponent->Velocity = velocity2;
		//moveComponent->SmoothCorrection(
		//	view->GetActorLocation(), view->GetActorRotation().Quaternion(),
		//	location2, rotator2.Quaternion()
		//);

		//oldLocation = location2;
		//oldRotator = rotator2;

		moveComponent->MovementMode = movementMode;
		moveComponent->bJustTeleported = false;
		moveComponent->bNetworkSmoothingComplete = false;
		moveComponent->SmoothCorrection(view->GetActorLocation(), view->GetActorRotation().Quaternion(), location2, rotator2.Quaternion());
		view->OnUpdateSimulatedPosition(view->GetActorLocation(), view->GetActorRotation().Quaternion());
	}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/