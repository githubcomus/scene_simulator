// Fill out your copyright notice in the Description page of Project Settings.

#include "Hall_Main.h"
//#include "ShortcutBar/ShortcutBarPanelUserWidget.h"
#include "Pawn/Character/CharacterComponent/PlayerEditEntityComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterControllerComponent.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Yxjs/UI/BaseHUD.h"
#include "Yxjs/Pawn/Character/BaseCharacter.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Yxjs/GameMode/HallGameMode.h"
#include "../../HallHUD.h"

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
void UHall_Main::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UHall_Main::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UHall_Main::NativeDestruct()
{
	Super::NativeDestruct();
}

//
FReply UHall_Main::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UHall_Main::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UHall_Main::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UHall_Main::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
FReply UHall_Main::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

//
FReply UHall_Main::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftShift)
	{
	}
	return FReply::Handled();
}

//
FReply UHall_Main::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
	if (InKeyEvent.GetKey() == EKeys::LeftShift)
	{
	}
	return FReply::Handled();
}

//
void UHall_Main::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	EditableText_账号 = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_账号")));
	EditableText_密码 = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_密码")));
	EditableText_IP = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_IP")));
	EditableText_端口 = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_端口")));
	Button_登陆服务端 = Cast<UButton>(GetWidgetFromName(TEXT("Button_登陆服务端")));
	TextBlock_button_str = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_button_str")));

	if (EditableText_账号.IsValid() == false
		|| EditableText_密码.IsValid() == false
		|| EditableText_IP.IsValid() == false
		|| EditableText_端口.IsValid() == false
		|| Button_登陆服务端.IsValid() == false
		|| TextBlock_button_str.IsValid() == false

		)
	{
		return;
	}

	//Button_登陆服务端

	FScriptDelegate func;
	func.BindUFunction(this, TEXT("MyButtonClicked"));
	Button_登陆服务端->OnClicked.AddUnique(func);
}

//
void UHall_Main::Start()
{

	// 从数据库读取,填入默认值

}

//
void UHall_Main::OnExpandCollapse(bool isShow, bool useAnimation)
{


	//// 粗暴无动画模式
	//if (isShow) {
	//	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//}
	//else {
	//	SetVisibility(ESlateVisibility::Hidden);
	//}

	//if (auto Event_ChangeChildUI = FindFunction("Event_ChangeChildUI")) {

	//	struct FUIInfo_ChangeChildUIBroadcast {
	//		bool isShow;
	//		bool useAnimation = false;
	//	};
	//	FUIInfo_ChangeChildUIBroadcast info;
	//	info.isShow = isShow;
	//	info.useAnimation = useAnimation;
	//	ProcessEvent(Event_ChangeChildUI, &info);
	//};

	//auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	//if (character == nullptr)return;
	//if (isShow) {
	//	//character->myController->Call_EnterStaus(true);
	//}
	//else {
	//	//character->myController->Call_EnterStaus(false);
	//}
	//if (shortcutBar.IsValid()) {
	//	shortcutBar->OnExpandCollapse(isShow, this);
	//}

}

// 登录按钮点击
void UHall_Main::MyButtonClicked()
{

	AHallGameMode* gameMode = Cast<AHallGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode == nullptr)return;


	UE_LOG(LogTemp, Log, TEXT("[%x] [大厅] [MyButtonClicked] 登录按钮"), this);
	// 未登录
	if (isLogin == false)
	{
		TextBlock_button_str->SetText(FText::FromString(TEXT("点击取消连接...")));
		isLogin = true;

		FString account = EditableText_账号->GetText().ToString();
		FString password = EditableText_密码->GetText().ToString();
		FString ip = EditableText_IP->GetText().ToString();
		std::string portStr(TCHAR_TO_UTF8(*EditableText_端口->GetText().ToString()));
		int port = atoi(portStr.c_str());

		if (account == TEXT("")
			|| password == TEXT("")
			|| ip == TEXT("")
			|| port == 0
			)
		{

			UE_LOG(LogTemp, Log, TEXT("[%x] [大厅] [MyButtonClicked] 数据错误"), this);

			return;
		}

		gameMode->UICall_StartConnect(account, password, ip, port);

		SaveUIData();

	}
	// 取消登录
	else
	{
		TextBlock_button_str->SetText(FText::FromString(TEXT("连接服务器")));
		isLogin = false;
		gameMode->UICall_StopConnect();
	}
}

// 登录失败
void UHall_Main::ResponseLoginFail()
{

	//APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//AHallHUD* hud = Cast<AHallHUD>(playerController->GetHUD());

	UE_LOG(LogTemp, Log, TEXT("[%x] [大厅] [ResponseLoginFail] 登录失败"), this);
	TextBlock_button_str->SetText(FText::FromString(TEXT("连接服务器")));
	isLogin = false;
}

// 将当前面板数据保存到ini
void UHall_Main::SaveUIData()
{

}