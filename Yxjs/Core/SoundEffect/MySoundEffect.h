// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "Core/GameInstanceStaticFunc.h"
#include "MySoundEffect.generated.h"

class UBaseGameInstance;

// 调用的时候是枚举,阅读的时候是汉字,填表是汉字,对比的时候也是汉字



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UMySoundEffect : public UObject
{
	GENERATED_BODY()

public:

	std::string soundEffectList[1000] = {
	"CustomizeUI_光标决定",
"sfx_ui_roshambo_赢",
"sfx_ui_vs_滑出",
"sfx_ui_赞成升级应用",
"sfx_奖励解锁",
"ui_买",
"ui_怪物吸收",
"ui_水晶",
"UI_确认",
"UI_解锁",
"ui_黄金",
"UI加载",
"UI叮咚音效",
"UI层",
"UI界面按钮点击音效",
"ui确认",
"UI确认音效",
"光标8",
"刀刺中_噌",
"发光闪光_影视转场",
"叮叮_歪歪提示音_重",
"叮当声",
"性感_11",
"按钮3_点击按钮_游戏提示音_系统提示音",
"攻击打击击中_嘭_近战格斗",
"津津见",
"清脆的叮_成功胜利正确",
"点击按钮_游戏ui",
"点击鼠标开关按钮_哒哒哒",
"确定_叮咚_游戏提示音",
"碰撞_砰",
"系统警告回答错误_游戏提示",
"能量或技能_UI_声音充电7",
"菜单按钮3",
"警报_25",
"警报声_游戏提示音",
"超级倒带",
"轻击光标按钮_游戏提示音_系统",
"闪闪发光的_ui",
"鼠标点击_游戏提示"
	};

	//
	TWeakObjectPtr< UBaseGameInstance> owner_;

	// 声音字典 key:name value:info
	std::map<std::string, xlsdata::SoundEffectTableDefine*> soundMap;

	// 声音异步加载序号
	int32 soundHandNumber = 1;

	// 声音异步加载map
	TMap<int32, TSharedPtr<struct FStreamableHandle>>handleMap;

	/*------------------------------------------------------------------*/


	UMySoundEffect();
	void Start(UBaseGameInstance* owner);
	void End();


	/**
		* 函数:    PlaySound :播放声音
		* 返回:    void
		* 参数:    FString path: 资源路径
		* 参数:    bool start : 默认true
		* 参数:    int jobId: 默认0
		* 参数:    bool useLocation : true:使用位置,false:使用2d播放
		* 参数:    FVector location : 播放位置
		*/
	void PlaySound(FString path, bool start = true, int jobId = 0, bool useLocation = false, FVector location = FVector::ZeroVector);

	void PlaySound(EMySoundEffectType type);
};
