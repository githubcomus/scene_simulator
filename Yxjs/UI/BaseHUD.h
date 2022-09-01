// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Protobuf/game.pb.h"
#include "BaseHUDInfo.h"
#include "BaseHUD.generated.h"

class UUserWidget;


// 主UI数据
USTRUCT()
struct FMyMainUIInfo
{
	GENERATED_BODY()

public:

	// 主UI请求
	UPROPERTY(VisibleAnywhere)
		EGameMainOperationMode mainRequest;
	// 默认子UI
	UPROPERTY(VisibleAnywhere)
		EUIChildName defaultChildUI;
	// 子UI列表
	UPROPERTY(VisibleAnywhere)
		TArray<EUIChildName>childUINameList;

};

// 子UI数据
USTRUCT()
struct FMyChildUIInfo
{
	GENERATED_BODY()

public:
	// 指针
	UPROPERTY(VisibleAnywhere)
		UUserWidget* uiPtr = nullptr;

	// 加载状态
	UPROPERTY(VisibleAnywhere)
		bool isLoadToView = false;

	// 路径
	UPROPERTY(VisibleAnywhere)
		FString path = TEXT("");

	// 切换子UI事件
	UPROPERTY(VisibleAnywhere)
		UFunction* OnExpandCollapse = nullptr;

	// 显示隐藏
	UPROPERTY(VisibleAnywhere)
		bool isShow = false;

	// ui状态(鼠标状态)
	UPROPERTY(VisibleAnywhere)
		bool mouseState = false;

	// 输入模式
	UPROPERTY(VisibleAnywhere)
		ECharacterStatusType inputMode = ECharacterStatusType::CharacterStatusType_None;

	// 层级
	int order = 1;
};


// 常驻UI数据
USTRUCT()
struct FMyPermanentUIInfo
{
	GENERATED_BODY()

public:
	// 指针
	UPROPERTY(VisibleAnywhere)
		UUserWidget* uiPtr = nullptr;

	// 加载状态
	UPROPERTY(VisibleAnywhere)
		bool isLoadToView = false;

	// 路径
	UPROPERTY(VisibleAnywhere)
		FString path = TEXT("");

	// 切换子UI事件
	UPROPERTY(VisibleAnywhere)
		UFunction* OnExpandCollapse = nullptr;

	// 显示隐藏
	UPROPERTY(VisibleAnywhere)
		bool isShow = false;

	// 层级
	int order = 2;
};

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YXJS_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:

	// 主UI数据 key:主UI请求 value:info
	UPROPERTY(VisibleAnywhere)
		TMap<int32, FMyMainUIInfo >mainUIMap;

	// 子UI数据 key:子uiname value:info
	UPROPERTY(VisibleAnywhere)
		TMap<int32, FMyChildUIInfo >childUIMap;

	// 当前主UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EGameMainOperationMode mainRequest = EGameMainOperationMode::game_main_operation_mode_none;

	// 当前子UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EUIChildName childUI = EUIChildName::ui_none;

	// 常驻UI数据 key:子uiname value:info
	UPROPERTY(VisibleAnywhere)
		TMap<int32, FMyPermanentUIInfo >permanentUIMap;

	/*------------------------------------------------------------------*/

	// 子UI
	FString uiChildNameList[100] = { TEXT("空"),TEXT("玩家界面") ,TEXT("赛车界面"), TEXT("编辑界面") ,TEXT("背包界面") ,
	TEXT("时间轴") ,TEXT("激光") ,TEXT("天气"), TEXT("实例合并"), TEXT("python") ,TEXT("新的时间轴"),TEXT("命令触发"),
	TEXT("命令按钮"),TEXT("实例材质"),TEXT("实例收纳"),TEXT("天空球"),TEXT("设置") };

	// 主操作模式
	FString uiMainRequestList[100] = { TEXT("无"),TEXT("玩家"), TEXT("赛车"),TEXT("编辑实例") };


	/*------------------------------------------------------------------*/

	ABaseHUD();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UUserWidget* GetChildUI(EUIChildName name);
	UUserWidget* GetPermanentUI(EPermanentName name);
	EGameMainOperationMode GetUIRequest();
	bool IsUIShow(EUIChildName name);
	void ShowUI(EUIChildName name);
	void HiddenUI(EUIChildName name);
	void GetUIFunction(UUserWidget* ui);
	void GetUIFunction(EUIChildName name);
	ECharacterStatusType GetChildUiInputMode(EUIChildName childName);
	FString GetChildUIName(EUIChildName childName);
	FString GetMainUIName(EGameMainOperationMode mainRequest_);
	void SetUIState(bool showMouse);
	bool IsOpenFullScreen();
	void Call_ChangeMainUI(EGameMainOperationMode mainRequest_);
	UFUNCTION(BlueprintCallable)
		void UICallback_OpenChildUI(EUIChildName pageName, bool open);
	void MainUIChangePretreatment(bool isStart, EUIChildName oldDefaultChildUI, EUIChildName oldChildUI, EUIChildName newChildUI);
	void CallUI_OpenChildUI(EUIChildName name, bool open, bool useAnimation = true);
	void SetInputMode(EUIChildName name);
	void MastUnfold(EUIChildName name, bool isShow, bool useAnimation);
};
