// Fill out your copyright notice in the Description page of Project Settings.

#include "MySetting.h"

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include <Curves/CurveVector.h>
#include <GameFramework/GameUserSettings.h>
#include "../BaseGameInstance.h"
#include "../../UI/BaseHUD.h"
//#include "DLSSLibrary.h"

/*/////////////////////////////////////////////////////////////*/
// 视图
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// 数据
/*/////////////////////////////////////////////////////////////*/

// 屏蔽掉tab保存数据
bool UMySetting::MySettingCanSave()
{
	auto hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (hud)
	{
		if (hud->childUI == EUIChildName::ui_name_setting)
		{
			return true;
		}
	}
	return false;
}

// 获取当前窗口模式

// 获取分辨率列表
TArray<FIntPoint> UMySetting::GetResolutionList()
{
	TArray<FIntPoint> resolutions_;
	bool resolutionsFlag = UKismetSystemLibrary::GetSupportedFullscreenResolutions(resolutions_);
	return resolutions_;
}

/*/////////////////////////////////////////////////////////////*/
// 控制器
/*/////////////////////////////////////////////////////////////*/

//
UMySetting::UMySetting()
{

}

// BeginPlay
void UMySetting::Start(UBaseGameInstance* owner)
{
	owner_ = owner;

	DelayRun(true);
}

// EndPlay
void UMySetting::End()
{
}

// 设置分辨率
void UMySetting::SetResolution(FIntPoint point)
{

	if (MySettingCanSave() == false)
	{
		return;
	}

	UGameUserSettings* setting = GEngine->GetGameUserSettings();
	setting->SetScreenResolution(point);
	setting->ApplyResolutionSettings(true);
	setting->SaveSettings();
}

// 设置窗口模式
void UMySetting::SetWindow(bool windows)
{

	if (MySettingCanSave() == false)
	{
		return;
	}

	UGameUserSettings* setting = GEngine->GetGameUserSettings();
	if (windows)
	{
		setting->SetFullscreenMode(EWindowMode::Fullscreen);
	}
	else
	{
		setting->SetFullscreenMode(EWindowMode::Windowed);
	}
	setting->SetScreenResolution(setting->GetScreenResolution());
	setting->ApplyResolutionSettings(true);
	setting->SaveSettings();
}

// 设置最大帧数
void UMySetting::SetFrames(CvMySettingType type)
{

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)return;

	if (type == SetFrames_30)
	{
		playerController->ConsoleCommand(TEXT("t.MaxFPS 30"), true);
	}
	else if (type == SetFrames_60)
	{
		playerController->ConsoleCommand(TEXT("t.MaxFPS 60 "), true);
	}
	else if (type == SetFrames_999)
	{
		playerController->ConsoleCommand(TEXT("t.MaxFPS 999 "), true);
	}

	// 保存配置
	GConfig->SetInt(TEXT("GameSetting"), TEXT("SetFrames"), (int)type, GGameIni);

	owner_->lockFrame = type;
}

// 设置游戏内的帧数情况?

// 设置各种质量
void UMySetting::SetQuality(ESettingLineType type, CvMySettingIntType value)
{

	if (MySettingCanSave() == false)
	{
		return;
	}

	UGameUserSettings* setting = GEngine->GetGameUserSettings();

	switch (type)
	{
	case ESettingLineType::ESettingLineType_Shadow:
		setting->SetShadingQuality((int32)value);
		break;
	case ESettingLineType::ESettingLineType_PostProcessing:
		setting->SetPostProcessingQuality((int32)value);
		break;
	case ESettingLineType::ESettingLineType_VisualEffect:
		setting->SetVisualEffectQuality((int32)value);
		break;
	case ESettingLineType::ESettingLineType_Texture:
		setting->SetTextureQuality((int32)value);
		break;
	case ESettingLineType::ESettingLineType_ViewDistance:
		setting->SetViewDistanceQuality((int32)value);
		break;
	case ESettingLineType::ESettingLineType_AntiAliasing:
		setting->SetAntiAliasingQuality((int32)value);
		break;
	case ESettingLineType::ESettingLineType_VerticalSync:
		break;
	}
	setting->ApplyNonResolutionSettings();
	setting->SaveSettings();
}

// 设置垂直同步
void UMySetting::SetVSyncEnabled(bool isEnable)
{

	if (MySettingCanSave() == false)
	{
		return;
	}

	UGameUserSettings* setting = GEngine->GetGameUserSettings();

	setting->SetVSyncEnabled(isEnable);
	setting->ApplyNonResolutionSettings();
	setting->SaveSettings();
}

// 设置DLSS
void UMySetting::SetDLSS(int type)
{

	if (MySettingCanSave() == false)
	{
		return;
	}

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)return;
	bool flag = false;


	//// dlss 是否可用
	//bool flag1 = UDLSSLibrary::IsDLSSSupported();
	//if (flag1)
	//{
	//	bool flag2 = UDLSSLibrary::IsDLSSModeSupported(UDLSSMode(type));
	//	if (flag2)
	//	{
	//		if (type > 0) {
	//			playerController->ConsoleCommand(TEXT("r.NGX.Enable 1"), true);
	//			playerController->ConsoleCommand(TEXT("r.NGX.DLSS.Enable 1"), true);
	//			UDLSSLibrary::SetDLSSMode(UDLSSMode(type));
	//		}
	//		else {
	//			UDLSSLibrary::SetDLSSMode(UDLSSMode(type));
	//			playerController->ConsoleCommand(TEXT("r.NGX.DLSS.Enable 0"), true);
	//		}

	//		flag = true;
	//	}
	//}
	//FString str = flag ? TEXT("允许设置") : TEXT("禁止设置");
	//UE_LOG(LogTemp, Log, TEXT("[%x] [设置] [SetDLSS]  [%sDLSS] [类型:%d] "), this, *str, type);

	// 保存配置
	//GConfig->SetInt(TEXT("GameSetting"), TEXT("DLSS"), (int)type, GGameIni);
}

// 强制关闭DLSS
void UMySetting::CloseDlss()
{
	//auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//if (playerController == nullptr)return;

	//bool flag1 = UDLSSLibrary::IsDLSSSupported(); // dlss 是否可用
	//if (flag1)
	//{
	//	UDLSSLibrary::SetDLSSMode(UDLSSMode::Off);
	//	playerController->ConsoleCommand(TEXT("r.NGX.DLSS.Enable 0"), true);
	//}

	//GConfig->SetInt(TEXT("GameSetting"), TEXT("DLSS"), (int)0, GGameIni); // 保存配置
}

// 延迟执行
void UMySetting::DelayRun(bool isStart)
{
	float relayTime = 1;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)return;

	// 预备执行
	if (isStart)
	{
		// 定时器采集
		if (timerHandle_DelayRun.IsValid())
		{
			playerController->GetWorldTimerManager().ClearTimer(timerHandle_DelayRun);
		}
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &UMySetting::DelayRun, false);
		playerController->GetWorldTimerManager().SetTimer(timerHandle_DelayRun, RespawnDelegate, relayTime, true);
	}
	// 循环判断
	else
	{
		if (GetWorld()->TimeSeconds > 3)
		{
			if (timerHandle_DelayRun.IsValid())
			{
				playerController->GetWorldTimerManager().ClearTimer(timerHandle_DelayRun);
			}
			SetSetting();
		}
	}
}

// 设置
void UMySetting::SetSetting()
{
	//GConfig->SetString(TEXT("/Script/WindowsTargetPlatform.WindowsTargetSettings"), TEXT("DefaultGraphicsRHI"), TEXT("DefaultGraphicsRHI_DX11"), GEngineIni);

	//GConfig->SetString(TEXT("/Script/WindowsTargetPlatform.WindowsTargetSettings"), TEXT("DefaultGraphicsRHI"), TEXT("DefaultGraphicsRHI_DX12"), GEngineIni);

	//UE_LOG(LogTemp, Log, TEXT("[%x] [设置] [SetSetting]   "), this);

	bool isEdit = false;
#if WITH_EDITORONLY_DATA
	isEdit = true;
#endif
	if (isEdit)
	{
		return;
	}

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)return;
	UGameUserSettings* setting = GEngine->GetGameUserSettings();

	// 读取最大帧数并设置,无则按照默认配置
	{
		bool useConfig = false;
		int type = 0;
		if (GConfig->GetInt(TEXT("GameSetting"), TEXT("SetFrames"), type, GGameIni))
		{
			SetFrames((CvMySettingType)type);
			useConfig = true;
			owner_->lockFrame = (CvMySettingType)type;
		}
		else
		{
			SetFrames(SetFrames_999);
			owner_->lockFrame = SetFrames_999;
		}
		UE_LOG(LogTemp, Log, TEXT("[%x] [我的设置] [SetSetting] [最大帧数] [useConfig:%d] [type:%d] --"), this, useConfig, type);
	}

	{

		int type = 0;
		//if (GConfig->GetInt(TEXT("GameSetting"), TEXT("SetFrames"), type, GGameIni)) {

			//	bool flag = false;
			//	bool flag1 = UDLSSLibrary::IsDLSSSupported(); // dlss 是否可用
			//	if (flag1)
			//	{
			//		bool flag2 = UDLSSLibrary::IsDLSSModeSupported(UDLSSMode(type));
			//		if (flag2)
			//		{
			//			if (type > 0) {
			//				playerController->ConsoleCommand(TEXT("r.NGX.Enable 1"), true);
			//				playerController->ConsoleCommand(TEXT("r.NGX.DLSS.Enable 1"), true);
			//			}
			//			else {
			//				playerController->ConsoleCommand(TEXT("r.NGX.DLSS.Enable 0"), true);
			//			}

			//			UDLSSLibrary::SetDLSSMode(UDLSSMode(type));
			//			flag = true;
			//		}
			//	}
			//	FString str = flag ? TEXT("允许设置") : TEXT("禁止设置");
			//	UE_LOG(LogTemp, Log, TEXT("[%x] [我的设置] [SetSetting]  [%sDLSS] [类型:%d] "), this, *str, type);
			//}

	}
}

// 保存字符串
void UMySetting::SetStringKey(FString key, FString value)
{
	GConfig->SetString(TEXT("GameSetting"), *key, *value, GGameIni);
}

// 获取字符串
FString UMySetting::GetStringKey(FString key)
{
	FString value = TEXT("");
	if (GConfig->GetString(TEXT("GameSetting"), *key, value, GGameIni))
	{
	}
	return value;
}

// 保存字符串
void UMySetting::SetIntKey(FString key, int value)
{
	GConfig->SetInt(TEXT("GameSetting"), *key, value, GGameIni);
}

// 获取字符串
int UMySetting::GetIntKey(FString key)
{
	int value = 0;
	if (GConfig->GetInt(TEXT("GameSetting"), *key, value, GGameIni))
	{
	}
	return value;
}

// 获取玩家登录信息
CvMyPlayerLogin UMySetting::GetPlayerLoginInfo()
{
	CvMyPlayerLogin info;

	// 初始化
	{
		FString value = TEXT("");
		if (GConfig->GetString(TEXT("GameSetting"), TEXT("accont"), value, GGameIni) == false)
		{
			CvMyPlayerLogin tempInfo;
			SetPlayerLoginInfo(tempInfo);
		}
	}

	info.account = GetStringKey(TEXT("account"));
	info.password = GetStringKey(TEXT("password"));
	info.ip = GetStringKey(TEXT("ip"));
	info.port = GetIntKey(TEXT("port"));
	return info;
}

// 保存玩家登录信息
void UMySetting::SetPlayerLoginInfo(CvMyPlayerLogin info)
{
	SetStringKey(TEXT("account"), info.account);
	SetStringKey(TEXT("password"), info.password);
	SetStringKey(TEXT("ip"), info.ip);
	SetIntKey(TEXT("port"), info.port);
}
