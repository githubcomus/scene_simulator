// Fill out your copyright notice in the Description page of Project Settings.

#include "HallHUD.h"
#include "Pawn/Character/BaseCharacter.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"

#include "GameMode/BaseGameMode.h"
#include "GameMode/Component/BaseGameModeController.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Protobuf/the_public.pb.h"
#include "Protobuf/hall.pb.h"

#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "Protobuf/NetPBTest.h"

#include "Json/Public/Serialization/JsonSerializer.h"
#include "Hall/Hall/Hall_Main.h"
#include "Hall/Room/Room_Main.h"

/*
 * 总ui管理器
 *
 */

 // 注册所有UI
 /*
  * 此处注册的所有UI都是单例
  *
  */

  /*/////////////////////////////////////////////////////////////*/
  // 视图
  /*/////////////////////////////////////////////////////////////*/

  // AHallHUD
AHallHUD::AHallHUD()
{
}

//
void AHallHUD::BeginPlay()
{
	Super::BeginPlay();

	// 创建大厅UI/房间UI
	LoadHallUI();
}

//
void AHallHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// 切换进大厅UI
void AHallHUD::LoadHallUI()
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (roomPtr)
	{
		roomPtr->RemoveFromViewport();
	}

	FString path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Hall/Hall/BP_UI_Hall_Main.BP_UI_Hall_Main_C'");
	UClass* classVar = LoadClass<UUserWidget>(NULL, *path);
	if (classVar)
	{
		hallPtr = CreateWidget<UUserWidget>(playerController, classVar);
		hallPtr->AddToViewport();
		Cast<UHall_Main>(hallPtr)->Start();
	}
}

// 切换进房间UI
void AHallHUD::LoadRoomUI()
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (hallPtr)
	{
		hallPtr->RemoveFromViewport();
	}

	FString path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Hall/Room/BP_UI_Room_Main.BP_UI_Room_Main_C'");
	UClass* classVar = LoadClass<UUserWidget>(NULL, *path);
	if (classVar)
	{
		roomPtr = CreateWidget<UUserWidget>(playerController, classVar);
		roomPtr->AddToViewport();
		Cast<URoom_Main>(roomPtr)->Start();
	}
}