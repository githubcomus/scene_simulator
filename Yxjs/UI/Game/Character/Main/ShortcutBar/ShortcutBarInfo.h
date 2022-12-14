// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//#include "Protobuf/game.pb.h"
#include "ShortcutBarInfo.generated.h"

class APlaceViewLaserLightActor;

class UBoxComponent;
class UMaterialInstanceConstant;
class UWidgetComponent;
class UUserWidget;


// [] 信息
USTRUCT(BlueprintType)
struct FPlayerShortcutBarInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> commandList;
};

//// [实例命令控制器] 信息
//USTRUCT(BlueprintType)
//struct FPlayerEntityButtonInfo
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//		TArray<FPlayerButtonKeyInfo> keyList;
//
//
//};