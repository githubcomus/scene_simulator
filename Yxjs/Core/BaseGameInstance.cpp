// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameInstance.h"
#include "Net/NetMainComponent.h"
#include "OnlineResources/OnlineResourcesComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Core/Component/ZwXlsDataActorComponent.h"

#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/AssetManager.h"
#include "UMG/Public/Components/Image.h"
#include "MyPython/MyPython.h"
#include "Component/MySetting.h"
#include "../UI/BaseHUD.h"

/*------------------------------------------------------------------*/

//
UBaseGameInstance::UBaseGameInstance()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UBaseGameInstance::UBaseGameInstance]"), this);

	zwXlsDataActorComponent = CreateDefaultSubobject<UZwXlsDataActorComponent>(TEXT("UZwXlsDataActorComponent"));

	// 竟然没有传入getowner,惊呆了
	netMainComponent = CreateDefaultSubobject<UNetMainComponent>(TEXT("UNetMainComponent"));
	onlineResourcesComponent = CreateDefaultSubobject<UOnlineResourcesComponent>(TEXT("UOnlineResourcesComponent"));
}

//
UBaseGameInstance::~UBaseGameInstance()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UBaseGameInstance::~UBaseGameInstance]"), this);
}

//
void UBaseGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Log, TEXT("[%x] [UBaseGameInstance::Init] [world:%x] [netMain owner:%x]"), this, GetWorld(),
		netMainComponent->GetOwner());

	mySetting = NewObject<UMySetting>(this, TEXT("UMySetting"));
	mySetting->Start(this);

	zwXlsDataActorComponent->Start(this);

	netMainComponent->Start(this);

	onlineResourcesComponent->Start(this);

	mySoundEffect = NewObject<UMySoundEffect>(this, TEXT("UMySoundEffect"));
	mySoundEffect->Start(this);


	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UBaseGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UBaseGameInstance::EndLoadingScreen);

	//myPython = NewObject<UMyPython>();
	//myPython->Start();

	//myPython->TestFunc();

	UE_LOG(LogTemp, Log, TEXT("[%x] [设置] [UBaseGameInstance] 20210516_1812 __ "), this);

	//GConfig->SetString(TEXT("/Script/WindowsTargetPlatform.WindowsTargetSettings"), TEXT("DefaultGraphicsRHI"), TEXT("DefaultGraphicsRHI_DX11"), GEngineIni);
}

//
void UBaseGameInstance::Shutdown()
{
	Super::Shutdown();

	mySetting->End();

	mySoundEffect->End();

	onlineResourcesComponent->End();

	netMainComponent->End();

	zwXlsDataActorComponent->End();

	//if (myPython) {
	//	myPython->End();
	//}
	//myPython = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[%x] [UBaseGameInstance::Shutdown] [world:%x]"), this, GetWorld());
}

/*------------------------------------------------------------------*/

// 地图开始加载
void UBaseGameInstance::BeginLoadingScreen(const FString& MapName)
{
}

// 地图结束加载
void UBaseGameInstance::EndLoadingScreen(UWorld* world)
{
	// 传入进来的world参数是错误的,在开启多个客户端的情况下需要区分
	auto world2 = GetWorld();
	if (world2 != world)
	{
		return;
	}

	auto gameMode = UGameplayStatics::GetGameMode(world);
	auto gameMode2 = Cast<ABaseGameMode>(gameMode);
	if (gameMode2)
	{
		auto level = gameMode2->GetLevel();
		FString name = level->GetFName().ToString();
		int playerId = netMainComponent->cvPlayerInfo.playerId;

		UE_LOG(LogTemp, Log,
			TEXT(
				"[%x] [UBaseGameInstance::EndLoadingScreen] [gamemode:%x] [world:%x] [world2:%x] [GetLevel:%s] [playerId;%d]"
			),
			this, gameMode2, world, world2, *name, playerId);

		if (gameMode2)
		{
			gameMode2->EndLoadingScreen();
		}
		else
		{
		}
	}
}

/*------------------------------------------------------------------*/

// test
void UBaseGameInstance::Test()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UBaseGameInstance::Test]"), this);
}

/*------------------------------------------------------------------*/
