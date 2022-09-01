// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseHUD.h"
#include "Pawn/Character/BaseCharacter.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerInputComponent.h"
#include "../GameMode/Component/BaseGameModeController.h"

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "HUDStaticFunc.h"
//#include "BaseHUDSetting.h"

/*
 * ui管理器
 * 1.管理主UI和子UI
 * 2.管理鼠标现隐藏
 * 3.管理ui与ui只间的关系

 UI结构

 主UI
 默认子UI
 功能子UI 1
 功能子UI 2
 功能子UI 3

 功能子UI 调用ChangeChildUI
 场景1:功能子UI平行打开->打开自己,关闭其他
 场景2:功能子UI关闭->关闭自己,打开默认

 */

 /*/////////////////////////////////////////////////////////////*/
 // 数据
 /*/////////////////////////////////////////////////////////////*/

 // 获取子UI指针
UUserWidget* ABaseHUD::GetChildUI(EUIChildName name)
{
	if (childUIMap.Find((int)name) == false)
	{
		return nullptr;
	}

	auto ui = &childUIMap[(int)name];
	UClass* classVar = LoadClass<UUserWidget>(NULL, *ui->path);
	if (classVar == nullptr)
	{
		return nullptr;
	}

	if (ui->uiPtr)
	{
		return ui->uiPtr;
	}
	else
	{
		auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ui->uiPtr = CreateWidget<UUserWidget>(playerController, classVar);
		return ui->uiPtr;
	}
}

// 获取常驻UI指针
UUserWidget* ABaseHUD::GetPermanentUI(EPermanentName name)
{
	if (permanentUIMap.Find((int)name) == false)
	{
		return nullptr;
	}

	return permanentUIMap[(int)name].uiPtr;
}

// 获取 当前显示的Type
EGameMainOperationMode ABaseHUD::GetUIRequest()
{
	return mainRequest;
}

// UI是否显示
bool ABaseHUD::IsUIShow(EUIChildName name)
{
	if (childUIMap.Find((int)name))
	{
		auto ui = &childUIMap[(int)name];
		if (ui->uiPtr && ui->isLoadToView)
		{
			return true;
		}
	}
	return false;
}

// 是否打开了全屏UI
bool ABaseHUD::IsOpenFullScreen()
{
	if (IsUIShow(EUIChildName::ui_name_mainui))
	{
		return true;
	}
	else if (IsUIShow(EUIChildName::ui_name_edit_entity))
	{
		return false;
	}
	else if (IsUIShow(EUIChildName::ui_name_timeline))
	{
		return false;
	}
	return false;
}

// 获取UI函数指针
void ABaseHUD::GetUIFunction(EUIChildName name)
{
	if (childUIMap.Find((int)name))
	{
		auto childUiInfo = &childUIMap[(int)name];
		if (childUiInfo->uiPtr)
		{
			childUiInfo->OnExpandCollapse = childUiInfo->uiPtr->FindFunction("OnExpandCollapse");
		}
	}
}

// 获取子UI输入模式
ECharacterStatusType ABaseHUD::GetChildUiInputMode(EUIChildName childName)
{
	if (childUIMap.Find((int)childName) == nullptr)return ECharacterStatusType::CharacterStatusType_None;
	auto childUiInfo = &childUIMap[(int)childName];

	return childUiInfo->inputMode;
}

// 子UI名称
FString ABaseHUD::GetChildUIName(EUIChildName childName)
{

	if (sizeof(uiChildNameList) > (int)childName)
		return uiChildNameList[int(childName)];
	return TEXT("");
}

// 主UI名称
FString ABaseHUD::GetMainUIName(EGameMainOperationMode mainRequest_)
{

	if (sizeof(uiMainRequestList) > (int)mainRequest_)
		return uiMainRequestList[int(mainRequest_)];
	return TEXT("");
}

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

 // ABaseHUD
ABaseHUD::ABaseHUD()
{
}

//
void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();


	// 主UI
	{
		FMyMainUIInfo myMainUIInfo;
		myMainUIInfo.mainRequest = EGameMainOperationMode::game_main_operation_mode_none;
		myMainUIInfo.defaultChildUI = EUIChildName::ui_none;
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_none);
		mainUIMap.Add((int)EGameMainOperationMode::game_main_operation_mode_none, myMainUIInfo);
	}
	{
		FMyMainUIInfo myMainUIInfo;
		myMainUIInfo.mainRequest = EGameMainOperationMode::game_main_operation_mode_base_character;
		myMainUIInfo.defaultChildUI = EUIChildName::ui_name_mainui;
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_mainui);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_sky_level);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_setting);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_character);
		mainUIMap.Add((int)EGameMainOperationMode::game_main_operation_mode_base_character, myMainUIInfo);
	}
	{
		FMyMainUIInfo myMainUIInfo;
		myMainUIInfo.mainRequest = EGameMainOperationMode::game_main_operation_mode_racing;
		myMainUIInfo.defaultChildUI = EUIChildName::ui_name_racing;
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_racing);
		mainUIMap.Add((int)EGameMainOperationMode::game_main_operation_mode_racing, myMainUIInfo);
	}
	{
		FMyMainUIInfo myMainUIInfo;
		myMainUIInfo.mainRequest = EGameMainOperationMode::game_main_operation_mode_edit_etity;
		myMainUIInfo.defaultChildUI = EUIChildName::ui_name_edit_entity;
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_edit_entity);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_sky);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_timeline);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_command_trigger);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_entity_backpack);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_entity_material);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_entity_storage);
		myMainUIInfo.childUINameList.Add(EUIChildName::ui_name_command_button);
		mainUIMap.Add((int)EGameMainOperationMode::game_main_operation_mode_edit_etity, myMainUIInfo);
	}

	// 子UI
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Character/MyNone/BP_UI_None.BP_UI_None_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_None;
		childUi.mouseState = false;
		childUIMap.Add((int)EUIChildName::ui_none, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Character/Main/BP_UI_CharacterMain.BP_UI_CharacterMain_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_None;
		childUi.mouseState = false;
		childUIMap.Add((int)EUIChildName::ui_name_mainui, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/Main/BP_UI_EditEntity_Main.BP_UI_EditEntity_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_edit_entity, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/Timeline/BP_UI_Timeline_Main.BP_UI_Timeline_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditTimeline;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_timeline, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/UltraDynamicSky/MyData/BP/UI_Sky_UI.UI_Sky_UI_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_sky, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/CommandTrigger/BP_UI_CommandTrigger_Main.BP_UI_CommandTrigger_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_LockEntity;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_command_trigger, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/CommandButton/BP_UI_CommandButton_Main.BP_UI_CommandButton_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_command_button, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/EntityBackpack/BP_UI_EntityBackpack_Main.BP_UI_EntityBackpack_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_entity_backpack, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/EntityMaterial/BP_UI_EntityMaterial_Main.BP_UI_EntityMaterial_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_entity_material, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/EntityStorage/BP_UI_EntityStorage_Main.BP_UI_EntityStorage_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_entity_storage, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Character/SkyLevel/BP_UI_SkyLevel_Main.BP_UI_SkyLevel_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_Player_Mouse;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_sky_level, childUi);
	}

	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Character/Setting/BP_UI_Setting_Main.BP_UI_Setting_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_Player_Mouse;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_setting, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Character/Character/BP_UI_Character_Main.BP_UI_Character_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_Player_Mouse;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_character, childUi);
	}


	// 暂时没有启用
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/Laser/BP_UI_Laser.BP_UI_Laser_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_None;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_laser, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/NewTimeline/BP_NewTimeline_Main.BP_NewTimeline_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditTimeline;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_new_timeline, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/EditEntity/MyPython/BP_UI_MyPython_Main.BP_UI_MyPython_Main_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_EditEntity;
		childUi.mouseState = true;
		childUIMap.Add((int)EUIChildName::ui_name_python, childUi);
	}
	{
		FMyChildUIInfo childUi;
		childUi.isShow = false;
		childUi.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Racing/BP_UI_RacingMain.BP_UI_RacingMain_C'");
		childUi.inputMode = ECharacterStatusType::CharacterStatusType_None;
		childUi.mouseState = false;
		childUIMap.Add((int)EUIChildName::ui_name_racing, childUi);
	}

	// 常驻UI
	{
		FMyPermanentUIInfo permanentUI;
		permanentUI.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Permanent/Tips/BP_UI_Tips_Main.BP_UI_Tips_Main_C'");
		permanentUIMap.Add((int)EPermanentName::ui_name_tips, permanentUI);
	}
	{
		FMyPermanentUIInfo permanentUI;
		permanentUI.path = TEXT("WidgetBlueprint'/Game/Yxjs/UI/Permanent/InputHelp/BP_UI_InputHelp_Main.BP_UI_InputHelp_Main_C'");
		permanentUIMap.Add((int)EPermanentName::ui_name_input_help, permanentUI);
	}

	// 加载常驻UI
	{
		auto func = [&](EPermanentName name) {
			auto ui = &permanentUIMap[(int)name];
			UClass* classVar = LoadClass<UUserWidget>(NULL, *ui->path);
			if (classVar)
			{
				auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				ui->uiPtr = CreateWidget<UUserWidget>(playerController, classVar);
				ui->uiPtr->AddToViewport();

				ui->isLoadToView = true;
			}
		};

		func(EPermanentName::ui_name_tips);
		func(EPermanentName::ui_name_input_help);
	}

}

//
void ABaseHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//setting->End();
	Super::EndPlay(EndPlayReason);
}

// 显示UI
void ABaseHUD::ShowUI(EUIChildName name)
{
	if (childUIMap.Find((int)name))
	{
		auto ui = &childUIMap[(int)name];
		if (IsUIShow(name))
		{
			return;
		}
		else
		{
			auto flag = GetChildUI(name);
			if (flag)
			{
				ui->uiPtr->AddToViewport();
				ui->isLoadToView = true;
				GetUIFunction(name);
			}
		}
	}
}

// 隐藏UI
void ABaseHUD::HiddenUI(EUIChildName name)
{
	if (childUIMap.Find((int)name))
	{
		auto ui = &childUIMap[(int)name];
		if (IsUIShow(name) == false)
		{
			return;
		}
		else
		{
			auto flag = GetChildUI(name);
			if (flag)
			{
				ui->uiPtr->RemoveFromViewport();
				ui->isLoadToView = false;
			}
		}
	}
}

// 切换主UI
void ABaseHUD::Call_ChangeMainUI(EGameMainOperationMode mainRequest_)
{
	if (mainUIMap.Find((int)mainRequest_) == nullptr)return;
	auto& newMainUiInfo = mainUIMap[(int)mainRequest_];
	auto& oldMainUiInfo = mainUIMap[(int)mainRequest];
	auto oldChildUI = childUI;

	EUIChildName newChildUI_ = newMainUiInfo.defaultChildUI;

	if (mainRequest == mainRequest_)return;

	UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [Call_ChangeMainUI] [主UI切换] [%s-->%s] "), this, *uiMainRequestList[(int)mainRequest], *uiMainRequestList[(int)mainRequest_]);

	MainUIChangePretreatment(true, oldMainUiInfo.defaultChildUI, oldChildUI, newChildUI_); // 预处理,关闭UI前存储数据

	// 老UI
	{
		for (auto node : oldMainUiInfo.childUINameList)
		{
			CallUI_OpenChildUI(node, false, false);	// 唯有此处不用动画直接最低点
			HiddenUI(node);
		}
	}

	// 新UI
	{
		//SetUIState(newMainUiInfo.mouseState);
		for (auto node : newMainUiInfo.childUINameList)
		{
			if (node != newChildUI_)
			{
				ShowUI(node);
				CallUI_OpenChildUI(node, false);
			}
		}
		ShowUI(newChildUI_);
		CallUI_OpenChildUI(newChildUI_, true);
	}
	mainRequest = mainRequest_;
	childUI = newChildUI_;

	//UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [Call_ChangeMainUI] [主UI切换] 完毕 [%s-->%s] "), this, *uiMainRequestList[(int)mainRequest], *uiMainRequestList[(int)mainRequest_]);

	// 存储老版本的数据预处理,打开新版本的数据预处理
	MainUIChangePretreatment(false, oldMainUiInfo.defaultChildUI, oldChildUI, newChildUI_);
}

// 切换主UI 预处理 Call_ChangeMainUI->
void ABaseHUD::MainUIChangePretreatment(bool isStart, EUIChildName oldDefaultChildUI, EUIChildName oldChildUI, EUIChildName newChildUI)
{

	auto tempFunc = [&](EUIChildName name, EUITempDateType type) {

		auto editEntity = GetChildUI(name);
		if (auto func = editEntity->FindFunction("SaveTempDate"))
		{
			struct TempStruct
			{
				EUITempDateType type;
			};
			TempStruct info;
			info.type = type;
			editEntity->ProcessEvent(func, &info);
		}

	};


	auto tempFunc2 = [&](EUIChildName name) {

		auto editEntity = GetChildUI(name);
		if (auto func = editEntity->FindFunction("UseTempDate"))
		{
			struct TempStruct
			{
			};
			TempStruct info;
			editEntity->ProcessEvent(func, &info);
		}

	};

	// 缓存
	if (isStart)
	{
		//UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [MainUIChangePretreatment] [缓存] [%s] "), this, *GetMainUIName(mainRequest));
		if (oldChildUI == EUIChildName::ui_name_edit_entity)
		{
			tempFunc(oldDefaultChildUI, EUITempDateType::UITempDateType_EditEntity);
		}
		else if (oldChildUI == EUIChildName::ui_name_timeline)
		{
			tempFunc(oldDefaultChildUI, EUITempDateType::UITempDateType_Timeline);
		}
		else if (oldChildUI == EUIChildName::ui_name_command_trigger)
		{
			tempFunc(oldDefaultChildUI, EUITempDateType::UITempDateType_CommandTrigger);
		}
	}// 预处理
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [MainUIChangePretreatment] [预处理] [%s] "), this, *GetMainUIName(mainRequest));
		if (newChildUI == EUIChildName::ui_name_edit_entity)
		{
			tempFunc2(newChildUI);
		}
	}
}

// 设置子UI显示隐藏 外部调用
void ABaseHUD::UICallback_OpenChildUI(EUIChildName pageName, bool isShow)
{
	/*
	 功能子UI 调用ChangeChildUI
	 场景1:功能子UI平行打开->打开自己,关闭其他
	 场景2:功能子UI关闭->关闭自己,打开默认

	 关闭的先,打开的后
	*/

	FString str = isShow ? TEXT("显示") : TEXT("隐藏");
	UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [UICallback_OpenChildUI] [子UI%s] [主:%s] [子:%s] "), this, *str, *GetMainUIName(mainRequest), *GetChildUIName(pageName));

	if (mainUIMap.Find((int)mainRequest) == nullptr)return;
	auto& mainUiInfo = mainUIMap[(int)mainRequest];

	if (childUIMap.Find((int)pageName) == nullptr)return;
	auto childUiInfo = &childUIMap[(int)pageName];

	// 打开自己,关闭其他
	if (isShow)
	{
		switch (pageName)
		{ // 特殊处理
		case EUIChildName::ui_none:
			break;
		default:
		{
			for (auto node : mainUiInfo.childUINameList)
			{
				if (node != pageName)
				{
					CallUI_OpenChildUI(node, false);
				}
			}
		}
		break;
		}
		CallUI_OpenChildUI(pageName, true);
		childUI = pageName;
	}
	// 关闭自己,打开默认
	else
	{
		CallUI_OpenChildUI(pageName, false);
		CallUI_OpenChildUI(mainUiInfo.defaultChildUI, true);
		childUI = mainUiInfo.defaultChildUI;
	}

}

// 设置子UI显示隐藏 UICallback_OpenChildUI->
void ABaseHUD::CallUI_OpenChildUI(EUIChildName name, bool isShow, bool useAnimation)
{
	if (childUIMap.Find((int)name))
	{
		auto childUiInfo = &childUIMap[(int)name];
		if (childUiInfo->isShow != isShow)
		{
			childUiInfo->isShow = isShow;
			FString str = isShow ? TEXT("显示") : TEXT("隐藏");
			UE_LOG(LogTemp, Log, TEXT("[%x] [主操作模式] [CallUI_OpenChildUI] [设置子UI%s] [%s] "), this, *str, *GetChildUIName(name));
			SetUIState(childUiInfo->mouseState);
			MastUnfold(name, isShow, useAnimation);
			if (isShow)
			{
				SetInputMode(name);
				UHUDStaticFunc::ChangeInputHelp(this, name, PlayerOperateType::PlayerOperateType_None);

				UGameInstanceStaticFunc::PlaySound(this, EMySoundEffectType::MySoundEffectType_点击按钮_游戏ui);
			}
		}
	}
}

// 设置子UI显示隐藏 设置鼠标隐藏显示
void ABaseHUD::SetUIState(bool showMouse)
{
	auto playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// 游客模式,隐藏鼠标
	if (showMouse == false)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController);
		playerController->bShowMouseCursor = false;
	}
	// 编辑模式,显示鼠标
	else
	{
		playerController->bShowMouseCursor = true;

		EMouseLockMode InMouseLockMode = EMouseLockMode::DoNotLock;
		bool bHideCursorDuringCapture = false;
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(InMouseLockMode);
		InputMode.SetHideCursorDuringCapture(bHideCursorDuringCapture);
		playerController->SetInputMode(InputMode);

	}
}

// 设置子UI显示隐藏 设置输入模式 ->CallUI_OpenChildUI
void ABaseHUD::SetInputMode(EUIChildName name)
{

	auto inputMode = GetChildUiInputMode(name);
	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character)
	{
		character->inputComp->inputMode = inputMode;
	}
}

// 设置子UI显示隐藏 调用UI ->CallUI_OpenChildUI
void ABaseHUD::MastUnfold(EUIChildName name, bool isShow, bool useAnimation)
{

	if (childUIMap.Find((int)name))
	{
		auto childUiInfo = &childUIMap[(int)name];
		if (childUiInfo->uiPtr && childUiInfo->OnExpandCollapse)
		{
			struct FUIInfo_ChangeChildUIBroadcast
			{
				bool isShow;
				bool useAnimation = false;
			};
			FUIInfo_ChangeChildUIBroadcast info;
			info.isShow = isShow;
			info.useAnimation = useAnimation;
			childUiInfo->uiPtr->ProcessEvent(childUiInfo->OnExpandCollapse, &info);
		}
	}
}

/*------------------------------------------------------------------*/
