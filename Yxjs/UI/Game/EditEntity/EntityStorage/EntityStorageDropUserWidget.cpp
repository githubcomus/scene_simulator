// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityStorageDropUserWidget.h"
#include "EntityStoragePanelUserWidget.h"
#include "Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/PlayerPreviewComponent.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>
#include <Components/EditableText.h>
#include <Components/Overlay.h>
#include "EntityStorageItemUserWidget.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

//
void UEntityStorageDropUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEntityStorageDropUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEntityStorageDropUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

//
bool UEntityStorageDropUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDrop] 拖拽释放  "), this);
	return true;
}

//
void UEntityStorageDropUserWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDragEnter] 进入拖拽  "), this);

	/*
	流程如下:
	<1>item发起拖拽,生成预览
	<2>drop这里结束拖拽
	<3>模拟鼠标左键松开
	<4>重新在game里捕获鼠标移动
	<5>game中捕获鼠标弹起,销毁预览
	*/

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//1. 进入预览区域,结束拖拽
	UWidgetBlueprintLibrary::CancelDragDrop();

	//SetVisibility(ESlateVisibility::Hidden);

	//2. 模拟鼠标弹起

	//2. 开启键盘捕获
	//playerController->bShowMouseCursor = true;// UI和游戏都捕获
	//EMouseLockMode InMouseLockMode = EMouseLockMode::DoNotLock;
	//bool bHideCursorDuringCapture = false;
	//FInputModeGameAndUI InputMode;
	//InputMode.SetLockMouseToViewportBehavior(InMouseLockMode);
	//InputMode.SetHideCursorDuringCapture(bHideCursorDuringCapture);
	//playerController->SetInputMode(InputMode);
	//UWidgetBlueprintLibrary::SetFocusToGameViewport();

	//FKey key(TEXT("LeftMouseButton"));
	//playerController->InputKey(key, EInputEvent::IE_Pressed, 1, true);
	//UWidgetBlueprintLibrary::SetFocusToGameViewport();
	//playerController->InputKey(key, EInputEvent::IE_Released, 1, true);

	//2. 输入模式改为预览
	owner->ChangeEditEntityOrPreview(false);

	//3. 启动预览
	auto item = Cast<UEntityStorageItemUserWidget>(InOperation->Payload);
	if (item == nullptr)return;
	CreatePreview(&item->entitySyncInfo);

	//4. 拖拽窗口关闭
	owner->BP_UI_EntityStorage_Drop->SetVisibility(ESlateVisibility::Hidden);
}

//
void UEntityStorageDropUserWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [实例收纳] [NativeOnDragEnter] 离开拖拽  "), this);

}

//
void UEntityStorageDropUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

}

// 创建预览
void UEntityStorageDropUserWidget::CreatePreview(Gamedata::PlayerStorageEntity* syncInfo)
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;

	character->previewComponent->OpenClosePerview(false, nullptr, false);
	character->previewComponent->OpenClosePerview(true, syncInfo->mutable_entity_info(), false);
}
