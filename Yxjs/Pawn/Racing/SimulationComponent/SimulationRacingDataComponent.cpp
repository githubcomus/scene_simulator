// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationRacingDataComponent.h"
#include "Pawn/Racing/Simulation/SimulationRacingPawn.h"
#include "Pawn/Racing/SimulationComponent/SimulationRacingDataComponent.h"

#include "GameMode/BaseGameMode.h"
#include "Core/BaseGameInstance.h"
#include "UI/BaseHud.h"
#include "GameFramework/PlayerController.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Tools/CommonlyObject.h"
#include "Kismet/GameplayStatics.h"
#include <concrt.h>


/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 检查必须的数据
bool USimulationRacingDataComponent::CheckDefaultData()
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
UNetMainComponent* USimulationRacingDataComponent::GetNet()
{
	if (gameInstance.IsValid())
	{
		return gameInstance->netMainComponent;
	}
	return nullptr;
}

// 返回同步数据
Gamedata::PlayerInfo* USimulationRacingDataComponent::GetSyncInfo()
{
	//return &syncPlayerInfo;
	//auto gameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode.IsValid())
	{
		return gameMode->GetPlayerSyncInfo(playerId);
	}
	return nullptr;
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
USimulationRacingDataComponent::USimulationRacingDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void USimulationRacingDataComponent::BeginPlay()
{
	Super::BeginPlay();
}

//
void USimulationRacingDataComponent::Start(USimulationRacingControllerComponent* controller_, ASimulationRacingPawn* view_, int playerId_)
{
	CheckDefaultData();

	view = view_;
	controller = controller_;
	playerId = playerId_;


}

//
void USimulationRacingDataComponent::End()
{
}


/*----------------------------------------------------*/
/*----------------------------------------------------*/
