// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDStaticFunc.generated.h"

class UUserWidget;


// 当前具体操作
enum PlayerOperateType
{
	PlayerOperateType_None,
	PlayerOperateType_entity_backpack_drop,
	PlayerOperateType_timeline_edit,
	PlayerOperateType_timeline_cant_edit,
};

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YXJS_API UHUDStaticFunc : public UObject
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	UHUDStaticFunc();


	UFUNCTION(BlueprintCallable)
		static void PushMessage(UObject* object, FString str);

	static void ChangeInputHelp(UObject* object, EUIChildName childUI, PlayerOperateType operateType);
};
