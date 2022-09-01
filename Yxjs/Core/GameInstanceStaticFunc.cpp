// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstanceStaticFunc.h"
#include "Pawn/Character/BaseCharacter.h"
#include "GameMode/BaseGameMode.h"
#include "Core/SoundEffect/MySoundEffect.h"

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"

/*
 */

 /*/////////////////////////////////////////////////////////////*/
 // 数据
 /*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

 // UGameInstanceStaticFunc
UGameInstanceStaticFunc::UGameInstanceStaticFunc()
{
}

void UGameInstanceStaticFunc::PlaySound(UObject* object, EMySoundEffectType str)
{
	if (auto gameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(object)))
	{
		gameInstance->mySoundEffect->PlaySound(str);
	}
}
