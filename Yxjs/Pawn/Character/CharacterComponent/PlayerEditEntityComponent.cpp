// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn/Character/CharacterComponent/PlayerEditEntityComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "FunctionalModule/OverallSituation/OverallSituationView.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManage.h"
#include "Yxjs/FunctionalModule/EditEntity/MyPython/MyPythonActor.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "../BaseCharacter.h"
#include "Yxjs/UI/Game/EditEntity/Timeline/TimelineMainUserWidget.h"
#include "Yxjs/UI/BaseHUD.h"
#include "UI/Game/EditEntity/Timeline/TimelineMainUserWidget.h"
#include "UI/Game/EditEntity/Timeline/TimelinePanelUserWidget.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManageController.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 检查必须的数据
bool UPlayerEditEntityComponent::CheckTargetData()
{
	bool flag = true;
	//if (myPythonActor.IsValid() == false) {
	//	auto gameMode_ = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	if (gameMode_) {
	//		myPythonActor = gameMode_->editEntityManage->myPythonActor;
	//		flag = true;
	//	}
	//}
	selectComponent = Cast<ABaseCharacter>(GetOwner())->selectComponent;

	return flag;
}

// 检查必须的数据
bool UPlayerEditEntityComponent::CheckDefaultData()
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

// 能否展开UI
bool UPlayerEditEntityComponent::CanOpenUI()
{
	if (selectComponent.IsValid())
	{
		return selectComponent->IsSelectEntity();
	}
	return false;
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

// Sets default values for this component's properties
UPlayerEditEntityComponent::UPlayerEditEntityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPlayerEditEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// START
void UPlayerEditEntityComponent::Start()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEditEntityComponent::Start] "), this);
	CheckDefaultData();
	CheckTargetData();
	view = Cast<ABaseCharacter>(GetOwner());
}

// END
void UPlayerEditEntityComponent::End()
{
	//UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEditEntityComponent::End]   "), this);
}

// 键盘输入 切换编辑实例于预览状态
void UPlayerEditEntityComponent::SwitchPreviewStaus(bool inEnter)
{
	bool flag = false;
	if (inEnter)
	{
		// 记录此时的情况
		//view->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);
		Gamedata::PlayerInfo newSyncInfo;
		newSyncInfo.CopyFrom(*view->myData->GetSyncInfo());
		newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);
		view->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
	}
	else
	{
		// 记录此时的情况
		//view->myController->NetCall_UpdatePlayerInfo(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);
		Gamedata::PlayerInfo newSyncInfo;
		newSyncInfo.CopyFrom(*view->myData->GetSyncInfo());
		newSyncInfo.set_character_pawn_type(Gamedata::GameMainOperationMode::GameMainOperationMode_edit_etity);
		view->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
	}
}

// 键盘输入 UI按钮功能
void UPlayerEditEntityComponent::Function_EditUiOrder(EEditUIBaseOrder order)
{
	if (order == EEditUIBaseOrder::WorldLocation)
	{
		view->selectComponent->Input_SetWorkMode(ESelectedWorkMode::WorldLocation);
	}
	else if (order == EEditUIBaseOrder::WorldRotator)
	{
		view->selectComponent->Input_SetWorkMode(ESelectedWorkMode::WorldRotator);
	}
	else if (order == EEditUIBaseOrder::Scale)
	{
		view->selectComponent->Input_SetWorkMode(ESelectedWorkMode::Scale);
	}
	else if (order == EEditUIBaseOrder::RelativelyLocation)
	{
		view->selectComponent->Input_SetWorkMode(ESelectedWorkMode::RelativelyLocation);
	}
	else if (order == EEditUIBaseOrder::RelativelyRotator)
	{
		view->selectComponent->Input_SetWorkMode(ESelectedWorkMode::RelativelyRotator);
	}
	else if (order == EEditUIBaseOrder::ChangeSpeed)
	{
	}
	else if (order == EEditUIBaseOrder::ComeSelected)
	{
	}
	else if (order == EEditUIBaseOrder::OpenMoveItem)
	{
	}
	else if (order == EEditUIBaseOrder::NewInstance)
	{
		auto location = view->placeComponent->GetHitLocation();
		FTransform t;
		t.SetLocation(location);
		t.SetRotation(FRotator::ZeroRotator.Quaternion());
		t.SetScale3D(FVector(1, 1, 1));

		view->placeComponent->SpawnMeshInstance(2022000, false, t);
	}
	else if (order == EEditUIBaseOrder::CopyInstance)
	{
		view->selectComponent->OperationResultsSwitch(UPlayerSelectedComponent::Operation_Type::Operation_Type_paste);
	}
	else if (order == EEditUIBaseOrder::DeleteInstance)
	{
		view->selectComponent->OperationResultsSwitch(UPlayerSelectedComponent::Operation_Type::Operation_Type_Delete);
	}
}

// 键盘输入 鼠标滚轮切基础功能
void UPlayerEditEntityComponent::Function_NextFunction(int var)
{

	if (selectComponent->IsSelectEntity() == false)return;

	int count = view->myData->mpi.list.Num();
	// 下一个
	if (var == 1)
	{
		if (view->myData->mpi.index == count - 1)
		{
			view->myData->mpi.index = 0;
		}
		else
		{
			view->myData->mpi.index++;
		}
		auto order = view->myData->mpi.list[view->myData->mpi.index];
		Function_EditUiOrder(order);
	}
	// 上一个
	else if (var == -1)
	{
		if (view->myData->mpi.index == 0)
		{
			view->myData->mpi.index = count - 1;
		}
		else
		{
			view->myData->mpi.index--;
		}
		auto order = view->myData->mpi.list[view->myData->mpi.index];
		Function_EditUiOrder(order);
	}
}
