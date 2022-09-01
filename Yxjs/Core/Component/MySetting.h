// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "../../UI/Game/Character/Setting/SettingInfo.h"
#include "MySetting.generated.h"

class UBaseGameInstance;

// 各种配置
enum CvMySettingType
{
	SetFrames_30,
	SetFrames_60,
	SetFrames_999,


};

// 通用配置
enum CvMySettingIntType
{
	low = 0,
	medium = 1,
	high = 2,
	epic = 3,
	cinematic = 4
};

// 玩家登录
struct CvMyPlayerLogin
{
	FString account = TEXT("aaa");
	FString password = TEXT("aaa");
	FString ip = TEXT("127.0.0.1");
	int port = 8240;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UMySetting : public UObject
{
	GENERATED_BODY()

public:

	//
	TWeakObjectPtr< UBaseGameInstance> owner_;

	// 定时器
	FTimerHandle timerHandle_DelayRun;

	/*------------------------------------------------------------------*/

	bool MySettingCanSave();
	static TArray<FIntPoint> GetResolutionList();


	UMySetting();
	void Start(UBaseGameInstance* owner);
	void End();
	void SetResolution(FIntPoint point);
	void SetWindow(bool windows);
	void SetFrames(CvMySettingType type);

	void SetQuality(ESettingLineType type, CvMySettingIntType value);
	void SetVSyncEnabled(bool isEnable);
	void SetDLSS(int type);
	void CloseDlss();
	void DelayRun(bool isStart);
	void SetSetting();
	void SetStringKey(FString key, FString value);
	void SetIntKey(FString key, int value);
	int GetIntKey(FString key);
	CvMyPlayerLogin GetPlayerLoginInfo();
	void SetPlayerLoginInfo(CvMyPlayerLogin info);
	FString GetStringKey(FString key);
};
