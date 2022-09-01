// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Import/CharacterInfo.h"
#include "Import/GameModeInfo.h"
#include "UI/BaseHUD.h"
#include "Net/NetInfo.h"
#include "Protobuf/game.pb.h"
#include "Json.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "BaseGameModeLevelManage.generated.h"

class ABaseGameMode;

class UImage;


// 我的天空图信息
USTRUCT(BlueprintType)
struct FMySkyMapInfo
{
	GENERATED_BODY()

public:

	// 地图名称
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString mapName = TEXT("");

	// 地图显示名称
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString showName = TEXT("");

	// 地图描述
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString describe = TEXT("");

	// 缩略图路径
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString iconPath = TEXT("");


};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UBaseGameModeLevelManage : public UActorComponent
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 数据组件
	ABaseGameMode* data;

	// 天空数据列表
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FMySkyMapInfo> levelInfoList;

	// 当前加载下表
	int loadLevelIndex = 0;

	// 加载过程
	bool isRunLoad = false;

	// 初始化
	bool isLoadLevelInit = false;

	/*------------------------------------------------------------------*/

	// Sets default values for this component's properties
	UBaseGameModeLevelManage();
	void Start(ABaseGameMode* data_);
	void End();

	void LoadLevelInit();
	//void ChangeSkyLevel();
	UFUNCTION(BlueprintCallable)
		void LoadSky_job1(bool mast, int oldIndex, int newIndex);
	UFUNCTION(BlueprintCallable)
		void LoadSky_job2();
	UFUNCTION(BlueprintCallable)
		void LoadSky_job3();
	void CloseDlss(int sky_level_index);
	void ChangeNewSkyLevel(int newIndex, bool isInit);
};
