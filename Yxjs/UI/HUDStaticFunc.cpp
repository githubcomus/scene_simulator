// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDStaticFunc.h"
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
#include "BaseHUD.h"
#include "BaseHUDInfo.h"
#include "Game/Permanent/Tips/Tips_Main.h"
#include "Game/Permanent/InputHelp/InputHelp_Panel.h"
#include "Game/Permanent/InputHelp/InputHelp_Main.h"

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

 // UHUDStaticFunc
UHUDStaticFunc::UHUDStaticFunc()
{

}

// 推送消息
void UHUDStaticFunc::PushMessage(UObject* object, FString str)
{
	if (auto playerController = UGameplayStatics::GetPlayerController(object, 0))
	{
		if (auto hud = Cast<ABaseHUD>(playerController->GetHUD()))
		{
			if (auto tips = Cast<UTips_Main>(hud->GetPermanentUI(EPermanentName::ui_name_tips)))
			{
				tips->PushMessage(str);
			}
		}
	}
}

// 切换输入提示信息
void UHUDStaticFunc::ChangeInputHelp(UObject* object, EUIChildName childUI, PlayerOperateType operateType)
{

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(object, 0))
	{
		if (ABaseHUD* hud = Cast<ABaseHUD>(playerController->GetHUD()))
		{
			if (UInputHelp_Main* main = Cast<UInputHelp_Main>(hud->GetPermanentUI(EPermanentName::ui_name_input_help)))
			{
				if (main->panelUserWidget.IsValid())
				{
					main->panelUserWidget->ChangeInputHelp(childUI, operateType);
				}
			}
		}
	}
}
