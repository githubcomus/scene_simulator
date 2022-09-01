// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstanceStaticFunc.generated.h"

class UUserWidget;


// 声音类型
enum  EMySoundEffectType
{
	MySoundEffectType_CustomizeUI_光标决定,
	MySoundEffectType_sfx_ui_roshambo_赢,
	MySoundEffectType_sfx_ui_vs_滑出,
	MySoundEffectType_sfx_ui_赞成升级应用,
	MySoundEffectType_sfx_奖励解锁,
	MySoundEffectType_ui_买,
	MySoundEffectType_ui_怪物吸收,
	MySoundEffectType_ui_水晶,
	MySoundEffectType_UI_确认,
	MySoundEffectType_UI_解锁,
	MySoundEffectType_ui_黄金,
	MySoundEffectType_UI加载,
	MySoundEffectType_UI叮咚音效,
	MySoundEffectType_UI层,
	MySoundEffectType_UI界面按钮点击音效,
	MySoundEffectType_ui确认,
	MySoundEffectType_UI确认音效,
	MySoundEffectType_光标8,
	MySoundEffectType_刀刺中_噌,
	MySoundEffectType_发光闪光_影视转场,
	MySoundEffectType_叮叮_歪歪提示音_重,
	MySoundEffectType_叮当声,
	MySoundEffectType_性感_11,
	MySoundEffectType_按钮3_点击按钮_游戏提示音_系统提示音,
	MySoundEffectType_攻击打击击中_嘭_近战格斗,
	MySoundEffectType_津津见,
	MySoundEffectType_清脆的叮_成功胜利正确,
	MySoundEffectType_点击按钮_游戏ui,
	MySoundEffectType_点击鼠标开关按钮_哒哒哒,
	MySoundEffectType_确定_叮咚_游戏提示音,
	MySoundEffectType_碰撞_砰,
	MySoundEffectType_系统警告回答错误_游戏提示,
	MySoundEffectType_能量或技能_UI_声音充电7,
	MySoundEffectType_菜单按钮3,
	MySoundEffectType_警报_25,
	MySoundEffectType_警报声_游戏提示音,
	MySoundEffectType_超级倒带,
	MySoundEffectType_轻击光标按钮_游戏提示音_系统,
	MySoundEffectType_闪闪发光的_ui,
	MySoundEffectType_鼠标点击_游戏提示,
};

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YXJS_API UGameInstanceStaticFunc : public UObject
{
	GENERATED_BODY()

public:


	/*------------------------------------------------------------------*/

	UGameInstanceStaticFunc();
	//UFUNCTION(BlueprintCallable)
	static void PlaySound(UObject* object, EMySoundEffectType str);
};
