// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn/Character/CharacterComponent/PlayerPythonComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "FunctionalModule/OverallSituation/OverallSituationView.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManage.h"
#include "Yxjs/FunctionalModule/EditEntity/MyPython/MyPythonActor.h"
#include <Kismet/GameplayStatics.h>

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 检查必须的数据
bool UPlayerPythonComponent::CheckTargetData()
{
	bool flag = false;
	if (myPythonActor.IsValid() == false)
	{
		auto gameMode_ = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gameMode_)
		{
			myPythonActor = gameMode_->editEntityManage->myPythonActor;
			flag = true;
		}
	}

	return flag;
}

// 检查必须的数据
bool UPlayerPythonComponent::CheckDefaultData()
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

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

// Sets default values for this component's properties
UPlayerPythonComponent::UPlayerPythonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPlayerPythonComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// START
void UPlayerPythonComponent::Start()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPythonComponent::Start] "), this);
	CheckDefaultData();
	CheckTargetData();
}

// END
void UPlayerPythonComponent::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerPythonComponent::End]   "), this);
}

// Called every frame
void UPlayerPythonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

// 用户调用 打开关闭UI
void UPlayerPythonComponent::UICall_SetOpenCloseUI(bool flag)
{
	isOpenUI = flag;
}

// 用户调用 运行/关闭python
void UPlayerPythonComponent::UICall_SetRunPython(bool flag)
{
	//myPythonActor->SetRunPython(flag);
}

// 网络调用
void UPlayerPythonComponent::NetCall_ChangeSkySet(int time, int weather, int cloud_coverageoverall, int cloud_speed, int moon_color)
{
	if (CheckDefaultData() == false || CheckTargetData() == false)return;

	gameMode->overallSituationView->NetCall_ChangeSkySet(time, weather, cloud_coverageoverall, cloud_speed, moon_color);
}

/*------------------------------------------------------------------*/
