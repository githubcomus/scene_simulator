// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceNearDataComponent.h"
#include "PlaceNearControllerComponent.h"
#include "PlaceNearActorView.h"

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
bool UPlaceNearDataComponent::CheckDefaultData()
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

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
UPlaceNearDataComponent::UPlaceNearDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UPlaceNearDataComponent::BeginPlay()
{
	Super::BeginPlay();
}

//
void UPlaceNearDataComponent::Start(UPlaceNearControllerComponent* controller_, APlaceNearActorView* view_)
{
	CheckDefaultData();

	view = view_;
	controller = controller_;


}

//
void UPlaceNearDataComponent::End()
{
}


/*----------------------------------------------------*/
/*----------------------------------------------------*/
