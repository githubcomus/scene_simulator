// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSkyComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "FunctionalModule/OverallSituation/OverallSituationView.h"
#include <Kismet/GameplayStatics.h>

/*------------------------------------------------------------------*/

// 检查必须的数据
bool UPlayerSkyComponent::CheckTargetData()
{
	//bool checkTwice = false;

	//if (skyActor.IsValid() == false || weatherActor.IsValid() == false || skyControllerActor.IsValid() == false) {
	//	TArray<AActor*>actorList;
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actorList);
	//	for (auto node : actorList) {
	//		if (node->GetName().StartsWith(TEXT("Ultra_Dynamic_Sky"))) {
	//			skyActor = node;
	//		}
	//		else if (node->GetName().StartsWith(TEXT("Ultra_Dynamic_Weather"))) {
	//			weatherActor = node;
	//		}
	//	}

	//	auto gameMode_ = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	if (gameMode_) {
	//		skyControllerActor = gameMode_->skyControllerActor;
	//	}

	//	checkTwice = true;

	//}

	//bool flag = true;
	//if (checkTwice) {
	//	if (skyActor.IsValid() == false || weatherActor.IsValid() == false || skyControllerActor.IsValid() == false) {
	//		flag = false;
	//	}
	//}

	//return flag;

	return true;
}

// 检查必须的数据
bool UPlayerSkyComponent::CheckDefaultData()
{
	bool checkTwice = false;
	if (gameInstance.IsValid() == false || gameMode.IsValid() == false
		|| playerController.IsValid() == false)
	{
		gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		checkTwice = true;
	}

	bool flag = true;
	if (checkTwice)
	{
		if (gameInstance.IsValid() == false || gameMode.IsValid() == false
			|| playerController.IsValid() == false)
		{
			flag = false;
		}
	}

	return flag;
}

/*------------------------------------------------------------------*/

// Sets default values for this component's properties
UPlayerSkyComponent::UPlayerSkyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPlayerSkyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// START
void UPlayerSkyComponent::Start()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSkyComponent::Start] "), this);
	//character = Cast<ABaseCharacter>(GetOwner());
}

// END
void UPlayerSkyComponent::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerSkyComponent::End]   "), this);
}

/*------------------------------------------------------------------*/

// Called every frame
void UPlayerSkyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*------------------------------------------------------------------*/

// 网络调用
void UPlayerSkyComponent::NetCall_ChangeSkySet(int time, int weather, int cloud_coverageoverall, int cloud_speed, int moon_color)
{
	if (CheckDefaultData() == false || CheckTargetData() == false)return;

	gameMode->overallSituationView->NetCall_ChangeSkySet(time, weather, cloud_coverageoverall, cloud_speed, moon_color);
}

/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/