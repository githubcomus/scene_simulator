// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameModeLevelManage.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "Net/NetMainComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "Yxjs/FunctionalModule/ActorTrajectory/ActorTrajectoryView.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "Pawn/Character/BaseCharacterInfo.h"
//#include "Pawn/Racing/Player/PlayerRacingPawn.h"
//#include "Pawn/Racing/PlayerComponent/RacingControllerComponent.h"
//#include "Pawn/Racing/PlayerComponent/RacingDataComponent.h"
#include "Yxjs/Pawn/Racing/Simulation/SimulationRacingPawn.h"
#include "Yxjs/Pawn/Racing/SimulationComponent/SimulationRacingControllerComponent.h"
#include "Yxjs/Pawn/Racing/SimulationComponent/SimulationRacingDataComponent.h"
#include "Yxjs/FunctionalModule/EditEntity/TempComponent/PlaceControllerScenesTimelineObject.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/Timeline/EditEntityManageTimelineComponent.h"
#include "Yxjs/FunctionalModule/OverallSituation/OverallSituationView.h"
#include "Pawn/Character/CharacterComponent/PlayerInputComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerEditEntityComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerCommandComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Camera/CameraComponent.h"
#include "Yxjs/Pawn/Character/CharacterComponent/PlayerTimelineComponent.h"
#include "Yxjs/FunctionalModule/EditEntity/Command/EditEntityManageCommandComponent.h"
#include <Engine/LevelStreaming.h>
#include "../../UI/BaseHUD.h"
#include "Yxjs/Core/Component/ZwXlsDataActorComponent.h"
#include "Yxjs/Core/Component/MySetting.h"
#include "Yxjs/UI/HUDStaticFunc.h"

/*
游戏消息响应分发

*/

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// Sets default values for this component's properties
UBaseGameModeLevelManage::UBaseGameModeLevelManage()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

//
void UBaseGameModeLevelManage::Start(ABaseGameMode* data_)
{
	data = data_;


	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance == nullptr)return;
	if (gameInstance->zwXlsDataActorComponent == nullptr)return;
	xlsdata::XlsData& xlsData = gameInstance->zwXlsDataActorComponent->xlsData;

	std::vector<xlsdata::SceneTableDefine>& table = xlsData.SceneTable_;
	int count = table.size();
	for (int i = 0; i < count; i++)
	{
		xlsdata::SceneTableDefine& node = table[i];

		FMySkyMapInfo info;
		info.mapName = UTF8_TO_TCHAR(node.MapName_.c_str());
		info.showName = UTF8_TO_TCHAR(node.Name_.c_str());
		info.describe = UTF8_TO_TCHAR(node.Description_.c_str());
		info.iconPath = UTF8_TO_TCHAR(node.IconPath_.c_str());
		levelInfoList.Add(info);
	}


}

//
void UBaseGameModeLevelManage::End()
{
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 玩家加载进来以后加载关卡,从玩家身上读取数据
void UBaseGameModeLevelManage::LoadLevelInit()
{

	if (isLoadLevelInit)return;
	isLoadLevelInit = true;

	//LoadSky_job1(true, 0, 0);


	//ChangeNewSkyLevel(0, true);

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

	int sky_level_index = character->myData->GetSyncInfo()->sky_level_index();

	// 如果是浅海则强行关闭dlss
	CloseDlss(sky_level_index);
	ChangeNewSkyLevel(sky_level_index, true);

}

//// 切换关卡
//void UBaseGameModeLevelManage::ChangeSkyLevel() {
//
//	if (levelInfoList.Num() <= 1)return;
//
//	int oldIndex = loadLevelIndex;
//	loadLevelIndex++;
//	if (loadLevelIndex >= levelInfoList.Num()) {
//		loadLevelIndex = 0;
//	}
//
//	LoadSky_job1(false, oldIndex, loadLevelIndex);
//}

// 切换到指定关卡
void UBaseGameModeLevelManage::ChangeNewSkyLevel(int newIndex, bool isInit)
{

	if (levelInfoList.Num() <= 1)return;
	if (isInit == false && newIndex == loadLevelIndex)return;

	//1. 处理DLSS
	{
		if (loadLevelIndex == 3)
		{
			UHUDStaticFunc::PushMessage(this, TEXT("DLSS已经强制关闭,请重新设置"));
		}
		CloseDlss(newIndex);
	}

	//2. 加载关卡
	int oldIndex = loadLevelIndex;
	loadLevelIndex = newIndex;
	LoadSky_job1(isInit, oldIndex, newIndex);

	//3. 数据存储
	if (isInit == false)
	{

		auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
		if (character == nullptr)return;
		auto hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		if (hud == nullptr)return;

		//character->myData->GetSyncInfo()->set_sky_level_index(newIndex);
		//character->myController->NetCall_UpdatePlayerInfo((Gamedata::GameMainOperationMode)hud->mainRequest);
		Gamedata::PlayerInfo newSyncInfo;
		newSyncInfo.set_character_pawn_type((Gamedata::GameMainOperationMode)hud->mainRequest);
		newSyncInfo.CopyFrom(*character->myData->GetSyncInfo());
		newSyncInfo.set_sky_level_index(newIndex);
		character->myController->NetCall_UpdatePlayerInfo(newSyncInfo);
	}

}

// 关卡任务开始
void UBaseGameModeLevelManage::LoadSky_job1(bool mast, int oldIndex, int newIndex)
{

	// 数据有问题
	if (oldIndex >= levelInfoList.Num() || newIndex >= levelInfoList.Num())return;

	// 不重复加载
	if (mast == false && oldIndex == newIndex)
	{
		return;
	}

	// 加载过程中,执行失败
	if (isRunLoad)
	{
		UE_LOG(LogTemp, Log, TEXT("[%x] [蓝图关卡] [LoadSky_job1] 加载失败,当前有任务正在进行"), this);
		return;
	}
	isRunLoad = true;

	auto& oldInfo = levelInfoList[oldIndex];
	auto& newInfo = levelInfoList[newIndex];

	// 已经加载->卸载流程
	bool isLoad = false;
	auto levelInfo = UGameplayStatics::GetStreamingLevel(GetWorld(), FName(*oldInfo.mapName));
	if (levelInfo != NULL)
	{
		isLoad = levelInfo->IsLevelLoaded();
	}

	if (isLoad)
	{

		UE_LOG(LogTemp, Log, TEXT("[%x] [蓝图关卡] [LoadSky_job2] 卸载关卡开始 [%s] [%s]"), this, *oldInfo.showName, *oldInfo.mapName);

		FLatentActionInfo LatentInfo;
		LatentInfo.ExecutionFunction = TEXT("LoadSky_job2");
		LatentInfo.CallbackTarget = this;
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = GetTypeHash(FGuid::NewGuid());
		UGameplayStatics::UnloadStreamLevel(GetWorld(), FName(*oldInfo.mapName), LatentInfo, true);
	}
	// 未加载->直接加载
	else
	{
		LoadSky_job2();
	}
}

// 加载关卡开始
void UBaseGameModeLevelManage::LoadSky_job2()
{

	auto& newInfo = levelInfoList[loadLevelIndex];
	UE_LOG(LogTemp, Log, TEXT("[%x] [蓝图关卡] [LoadSky_job2] 加载关卡开始 [%s] [%s]"), this, *newInfo.showName, *newInfo.mapName);

	FLatentActionInfo LatentInfo;
	LatentInfo.ExecutionFunction = TEXT("LoadSky_job3");
	LatentInfo.CallbackTarget = this;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = GetTypeHash(FGuid::NewGuid());
	UGameplayStatics::LoadStreamLevel(GetWorld(), FName(*newInfo.mapName), true, true, LatentInfo);
}

// 加载关卡完毕
void UBaseGameModeLevelManage::LoadSky_job3()
{
	auto& newInfo = levelInfoList[loadLevelIndex];

	UE_LOG(LogTemp, Log, TEXT("[%x] [蓝图关卡] [LoadSky_job3] 加载关卡完毕 [%s] [%s]"), this, *newInfo.showName, *newInfo.mapName);

	isRunLoad = false;
}

/*----------------------------------------------------*/

// 强制关闭DLSS
void UBaseGameModeLevelManage::CloseDlss(int sky_level_index)
{


	if (sky_level_index != 3)return;

	UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance == nullptr)return;
	if (gameInstance->mySetting == nullptr)return;
	gameInstance->mySetting->CloseDlss();

	UHUDStaticFunc::PushMessage(this, TEXT("该关卡不支持DLSS,强制关闭"));
}