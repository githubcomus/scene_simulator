// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoundEffect/MySoundEffect.h"
#include "Component/MySetting.h"
#include "BaseGameInstance.generated.h"

class UZwXlsDataActorComponent;
class UOnlineResourcesComponent;
class UBaseGameModeController;
class UMyPython;
class UMySetting;
class UMySoundEffect;

class UNetMainComponent;
class ACharacter;


/**
 *
 */
UCLASS()
class  UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 网络连接库
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UNetMainComponent* netMainComponent = nullptr;

	// 本地excel表格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UZwXlsDataActorComponent* zwXlsDataActorComponent = nullptr;

	// 网络资源下载
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UOnlineResourcesComponent* onlineResourcesComponent = nullptr;

	//// py
	//UPROPERTY()
	//	UMyPython* myPython;

	// 设置管理
	UPROPERTY(VisibleAnywhere)
		UMySetting* mySetting;

	// 系统音效
	UPROPERTY(VisibleAnywhere)
		UMySoundEffect* mySoundEffect;

	// 锁帧情况
	CvMySettingType lockFrame = CvMySettingType::SetFrames_999;

	/*------------------------------------------------------------------*/

	UBaseGameInstance();

	~UBaseGameInstance();

	virtual void Init();

	virtual void Shutdown();

	// 地图开始加载
	UFUNCTION()
		void BeginLoadingScreen(const FString& MapName);

	// 地图结束加载
	UFUNCTION()
		void EndLoadingScreen(UWorld* world);

	/*------------------------------------------------------------------*/

	// test
	void Test();

	/*------------------------------------------------------------------*/
};
