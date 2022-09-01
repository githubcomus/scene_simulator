// Fill out your copyright notice in the Description page of Project Settings.

#include "SettingItemSelectUserWidget.h"
#include "SettingPanelUserWidget.h"
#include "Core/Component/MySetting.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>
#include <Components/ComboBoxString.h>
#include <GameFramework/GameUserSettings.h>
#include "../../../../Core/BaseGameInstance.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void USettingItemSelectUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	TextBlock_selectStr = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_selectStr")));
	TextBlock_name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_name")));
	ComboBoxString_0 = Cast<UComboBoxString>(GetWidgetFromName(TEXT("ComboBoxString_0")));
	if (TextBlock_selectStr.IsValid() == false
		|| TextBlock_name.IsValid() == false
		|| ComboBoxString_0.IsValid() == false) {
		return;
	}

	if (selectIndex >= selectList.Num())selectIndex = 0;

	TextBlock_name->SetText(FText::FromString(*name));
	TextBlock_selectStr->SetText(FText::FromString(TEXT("")));

	// 分辨率
	if (lineType == ESettingLineType::ESettingLineType_Resolution) {
		TArray<FIntPoint> resolutList = UMySetting::GetResolutionList();
		for (auto node : resolutList) {
			selectList.Add(FString::Printf(TEXT("%d*%d"), node.X, node.Y));
		}
	}
	// 窗口
	else if (lineType == ESettingLineType::ESettingLineType_Window) {
	}

	//
	{
		int tempIndex = 0;
		while (ComboBoxString_0->GetOptionCount() != 0 && tempIndex < 100) {
			auto option = ComboBoxString_0->GetOptionAtIndex(0);
			ComboBoxString_0->RemoveOption(option);
			tempIndex++;
		}
		if (selectList.Num() > 0) {
			for (auto node : selectList) {
				ComboBoxString_0->AddOption(node);
			}
			ComboBoxString_0->SetSelectedIndex(selectIndex);
			TextBlock_selectStr->SetText(FText::FromString(selectList[selectIndex]));
		}
	}

}

//
void USettingItemSelectUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void USettingItemSelectUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void USettingItemSelectUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

//
FReply USettingItemSelectUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply USettingItemSelectUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void USettingItemSelectUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void USettingItemSelectUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
void USettingItemSelectUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	TextBlock_selectStr = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_selectStr")));
	TextBlock_name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_name")));
	ComboBoxString_0 = Cast<UComboBoxString>(GetWidgetFromName(TEXT("ComboBoxString_0")));
	if (TextBlock_selectStr.IsValid() == false
		|| TextBlock_name.IsValid() == false
		|| ComboBoxString_0.IsValid() == false) {
		return;
	}

	FScriptDelegate func;
	func.BindUFunction(this, TEXT("MySelectionChanged"));
	ComboBoxString_0->OnSelectionChanged.AddUnique(func);

}

//
void USettingItemSelectUserWidget::Start()
{
	UGameUserSettings* setting = GEngine->GetGameUserSettings();
	if (setting == nullptr)return;

	// 分辨率
	if (lineType == ESettingLineType::ESettingLineType_Resolution) {
		TArray<FIntPoint> resolutList = UMySetting::GetResolutionList();
		int32 index = resolutList.Find(setting->GetScreenResolution());
		if (index != INDEX_NONE)
		{
			selectIndex = index;
		}
	}
	// 窗口
	else if (lineType == ESettingLineType::ESettingLineType_Window) {
		EWindowMode::Type type = setting->GetFullscreenMode();
		if (type == EWindowMode::Fullscreen
			|| type == EWindowMode::Type::WindowedFullscreen) {
			selectIndex = 0;
		}
		else {
			selectIndex = 1;
		}
	}
	// 最大帧数
	else if (lineType == ESettingLineType::ESettingLineType_Frames) {
		int type = 0;
		if (GConfig->GetInt(TEXT("GameSetting"), TEXT("SetFrames"), type, GGameIni)) {
			if ((CvMySettingType)type == CvMySettingType::SetFrames_30) {
				selectIndex = 0;
			}
			else if ((CvMySettingType)type == CvMySettingType::SetFrames_60) {
				selectIndex = 1;
			}
			else if ((CvMySettingType)type == CvMySettingType::SetFrames_999) {
				selectIndex = 2;
			}
		}
	}
	else if (lineType == ESettingLineType::ESettingLineType_Shadow) {
		selectIndex = setting->GetShadowQuality();
	}
	else if (lineType == ESettingLineType::ESettingLineType_PostProcessing) {
		selectIndex = setting->GetPostProcessingQuality();
	}

	else if (lineType == ESettingLineType::ESettingLineType_VisualEffect) {
		selectIndex = setting->GetVisualEffectQuality();
	}

	else if (lineType == ESettingLineType::ESettingLineType_Texture) {
		selectIndex = setting->GetTextureQuality();
	}

	else if (lineType == ESettingLineType::ESettingLineType_ViewDistance) {
		selectIndex = setting->GetViewDistanceQuality();
	}

	else if (lineType == ESettingLineType::ESettingLineType_AntiAliasing) {
		selectIndex = setting->GetAntiAliasingQuality();
	}

	else if (lineType == ESettingLineType::ESettingLineType_VerticalSync) {
		selectIndex = setting->IsVSyncEnabled();
	}
	else if (lineType == ESettingLineType::ESettingLineType_DLSS) {

		int type = 0;
		if (GConfig->GetInt(TEXT("GameSetting"), TEXT("DLSS"), type, GGameIni))
		{
			selectIndex = (int)type;
		}
	}

	if (selectList.Num() > 0) {
		ComboBoxString_0->SetSelectedIndex(selectIndex);
		TextBlock_selectStr->SetText(FText::FromString(selectList[selectIndex]));
	}
}

// 选择项目更变
void USettingItemSelectUserWidget::MySelectionChanged(FString  SelectedItem, ESelectInfo::Type  SelectionType) {


	auto gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance == nullptr)return;
	if (gameInstance->mySetting == nullptr)return;

	bool isEdit = false;
#if WITH_EDITORONLY_DATA
	isEdit = true;
#endif

	UE_LOG(LogTemp, Log, TEXT("[%x] [设置界面] [MySelectionChanged:%s] [isEdit:%d] --"), this, *SelectedItem, isEdit);

	int index = 0;
	if (selectList.Find(SelectedItem, index) == false)return;

	TextBlock_selectStr->SetText(FText::FromString(SelectedItem));

	switch (lineType) {
	case ESettingLineType::ESettingLineType_Resolution:
	{
		TArray<FIntPoint> resolutList = UMySetting::GetResolutionList();
		if (isEdit == false) {
			gameInstance->mySetting->SetResolution(resolutList[index]);
		}
	}
	break;
	case ESettingLineType::ESettingLineType_Window:
		if (isEdit == false) {
			gameInstance->mySetting->SetWindow(index == 0 ? true : false);
		}
		break;
	case ESettingLineType::ESettingLineType_Quality:
		break;
	case ESettingLineType::ESettingLineType_Frames:
	{
		if (index == 0) {
			gameInstance->mySetting->SetFrames(CvMySettingType::SetFrames_30);
		}
		else if (index == 1) {
			gameInstance->mySetting->SetFrames(CvMySettingType::SetFrames_60);
		}
		else if (index == 2) {
			gameInstance->mySetting->SetFrames(CvMySettingType::SetFrames_999);
		}
	}
	break;
	case ESettingLineType::ESettingLineType_Shadow:
		gameInstance->mySetting->SetQuality(lineType, (CvMySettingIntType)index);
		break;
	case ESettingLineType::ESettingLineType_PostProcessing:
		gameInstance->mySetting->SetQuality(lineType, (CvMySettingIntType)index);
		break;
	case ESettingLineType::ESettingLineType_VisualEffect:
		gameInstance->mySetting->SetQuality(lineType, (CvMySettingIntType)index);
		break;
	case ESettingLineType::ESettingLineType_Texture:
		gameInstance->mySetting->SetQuality(lineType, (CvMySettingIntType)index);
		break;
	case ESettingLineType::ESettingLineType_ViewDistance:
		gameInstance->mySetting->SetQuality(lineType, (CvMySettingIntType)index);
		break;
	case ESettingLineType::ESettingLineType_AntiAliasing:
		gameInstance->mySetting->SetQuality(lineType, (CvMySettingIntType)index);
		break;
	case ESettingLineType::ESettingLineType_VerticalSync:
		gameInstance->mySetting->SetVSyncEnabled((bool)index);
		break;
	case ESettingLineType::ESettingLineType_DLSS:
		gameInstance->mySetting->SetDLSS(index);
		break;
	default:
		break;
	}
}