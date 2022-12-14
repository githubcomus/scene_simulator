// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerEntityBackpackComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerPlaceComponent.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Pawn/Character/CharacterComponent/PlayerMaterialComponent.h"
#include "Core/BaseGameInstance.h"
#include "Tools/CalculationObject.h"
#include "GameMode/BaseGameMode.h"
#include "FunctionalModule/EditEntity/EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "Net/NetMainComponent.h"
#include "Pawn/Character/BaseCharacter.h"
#include "UI/BaseHud.h"
#include "OnlineResources/OnlineResourcesComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Yxjs/UI/Game/EditEntity/EntityBackpack/EntityBackpackMainUserWidget.h"
#include "Yxjs/UI/Game/EditEntity/EntityBackpack/EntityBackpackPanelUserWidget.h"

#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include <math.h>

/*------------------------------------------------------------------*/
//
//// ?????? ?????????????????????
//void UPlayerEntityBackpackComponent::InitBackpackData()
//{
//	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
//
//	// ???????????????
//	for (auto it : gameInstance_->zwXlsDataActorComponent->menuBarMap)
//	{
//		auto id = it.first;
//		auto info = it.second;
//
//		if (info.MenuType_ == xlsdata::MenuType::MenuType_Backpack
//			&& info.MenuLevel_ == 0)
//		{
//			FString name = UTF8_TO_TCHAR(info.Name_.c_str());
//			TMap<int, CvBackpackLeftItemInfo> first;
//			biiMap.Add((int)info.FirstMenuType_, first);
//			UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [GetItemInfo] ????????? [firstXlsId:%d] [name:%s]"),
//				this, (int)info.FirstMenuType_, *name);
//		}
//	}
//
//	// ???????????????
//	for (auto it : biiMap)
//	{
//		auto firstType = it.Key;
//		auto info = it.Value;
//
//		for (auto it2 : gameInstance_->zwXlsDataActorComponent->menuBarMap)
//		{
//			auto id2 = it2.first;
//			auto info2 = it2.second;
//
//			if (info2.MenuType_ == xlsdata::MenuType::MenuType_Backpack
//				&& info2.MenuLevel_ == 1
//				&& (int)info2.FirstMenuType_ == firstType
//				)
//			{
//				auto first = &biiMap[firstType];
//
//				CvBackpackLeftItemInfo blii;
//
//				blii.listIndex = first->Num();
//				blii.firstMenuType = (int)info2.FirstMenuType_;
//				blii.secondMenuType = (int)info2.SecondMenuType_;
//				blii.name = UTF8_TO_TCHAR(info2.Name_.c_str());
//				blii.iconPath = UTF8_TO_TCHAR(info2.IconPath_.c_str());
//
//				first->Add(blii.secondMenuType, blii);
//
//				UE_LOG(LogTemp, Log, TEXT("[%x] [????????????] [GetItemInfo] ????????? [firstType:%d---secondMenuType:%d]   [name:%s]"),
//					this, firstType, blii.secondMenuType, *blii.name);
//
//				SetItemInfo((int)info2.FirstMenuType_, blii.secondMenuType);
//			}
//		}
//	}
//}
//
//// ?????? ???????????????
//void UPlayerEntityBackpackComponent::SetItemInfo(int firstType, int secondType)
//{
//	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
//
//	for (auto it : gameInstance_->zwXlsDataActorComponent->backpacktMap)
//	{
//		auto xlsId = it.first;
//		auto info = it.second;
//
//		if ((int)info.FirstMenuType_ == firstType
//			&& (int)info.SecondMenuType_ == secondType)
//		{
//			auto first = &biiMap[firstType];
//			auto second = &biiMap[firstType][secondType];
//
//			CvBackpackItemInfo bii;
//
//			bii.xlsId = xlsId;
//			bii.listIndex = second->list.Num();
//
//			bii.row = bii.listIndex / gridColumnCount;
//			bii.column = bii.listIndex % gridColumnCount;
//
//			bii.leftIndex = second->listIndex;
//			bii.name = UTF8_TO_TCHAR(info.Name_.c_str());
//			bii.description = UTF8_TO_TCHAR(info.Name_.c_str());
//			bii.iconPath = UTF8_TO_TCHAR(info.IconPath_.c_str());
//
//			if ((xlsdata::ItemType)second->secondMenuType == xlsdata::ItemType::ItemType_Mesh)
//			{
//				bii.place = GetPlaceXlsInfoWithXlsId(xlsId);
//			}
//			else if ((xlsdata::ItemType)second->secondMenuType == xlsdata::ItemType::ItemType_Material)
//			{
//				bii.material = GetMaterialXlsInfoWithXlsId(xlsId);
//			}
//
//			//UE_LOG(LogTemp, Log, TEXT("[%x] [GetItemInfo] ????????? [firstType:%d---secondMenuType:%d]   [name:%s]"),
//			//       this, firstType, secondType,*bii.name);
//			second->list.Add(bii);
//		}
//	}
//}
//
//// ?????? ????????????????????????
//xlsdata::BackpackTableDefine* UPlayerEntityBackpackComponent::GetBackpackXlsInfoWithXlsId(int32 xlsId)
//{
//	// ??????:?????????????????????string???????????????,??????????????????,???????????????
//
//	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
//	if (gameInstance_->zwXlsDataActorComponent->backpacktMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->backpacktMap.end())
//	{
//		return &gameInstance_->zwXlsDataActorComponent->backpacktMap[xlsId];
//	}
//	return nullptr;
//}
//
//// ?????? ??????????????????
//xlsdata::PlaceTableDefine* UPlayerEntityBackpackComponent::GetPlaceXlsInfoWithXlsId(int32 xlsId)
//{
//	// ??????:?????????????????????string???????????????,??????????????????,???????????????
//
//	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
//	if (gameInstance_->zwXlsDataActorComponent->placeMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->placeMap.end())
//	{
//		return &gameInstance_->zwXlsDataActorComponent->placeMap[xlsId];
//	}
//	return nullptr;
//}
//
//// ?????? ??????????????????
//xlsdata::MaterialTableDefine* UPlayerEntityBackpackComponent::GetMaterialXlsInfoWithXlsId(int32 xlsId)
//{
//	// ??????:?????????????????????string???????????????,??????????????????,???????????????
//
//	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
//	if (gameInstance_->zwXlsDataActorComponent->materialMap.find(xlsId) != gameInstance_->zwXlsDataActorComponent->materialMap.end())
//	{
//		return &gameInstance_->zwXlsDataActorComponent->materialMap[xlsId];
//	}
//	return nullptr;
//}

/*------------------------------------------------------------------*/

// Sets default values for this component's properties
UPlayerEntityBackpackComponent::UPlayerEntityBackpackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPlayerEntityBackpackComponent::BeginPlay()
{
	Super::BeginPlay();

	// this->AddToRoot();

	character = Cast<ABaseCharacter>(GetOwner());

	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEntityBackpackComponent::BeginPlay]   "), this);
}

// EndPlay
void UPlayerEntityBackpackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// this->RemoveFromRoot();

	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEntityBackpackComponent::EndPlay]   "), this);
}

// START
void UPlayerEntityBackpackComponent::Start()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEntityBackpackComponent::Start] "), this);
	character = Cast<ABaseCharacter>(GetOwner());
	//InitBackpackData();
}

// END
void UPlayerEntityBackpackComponent::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [UPlayerEntityBackpackComponent::End]   "), this);
}

/*------------------------------------------------------------------*/

// Called every frame
void UPlayerEntityBackpackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//
//// UI UI?????????
//auto UPlayerEntityBackpackComponent::UI_InitUI(int firstType)
//{
//	if (biiMap.Find(firstType) == false)return;
//
//	auto first = &biiMap[firstType];
//	auto it = biiMap[firstType].CreateConstIterator();
//	auto secondType = it.Key();
//
//	// ????????? ???????????????
//	UI_SetSecondItem(firstType);
//
//	// ????????? ???????????????
//	UI_SetThirdItem(firstType, secondType);
//}
//
//// UI ???????????????
//void UPlayerEntityBackpackComponent::UI_SetSecondItem(int firstType)
//{
//	auto view = Cast<ABaseCharacter>(GetOwner());
//	auto playerController = Cast<APlayerController>(view->Controller);
//	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
//	auto ui_name_backpack = Cast<UEntityBackpackMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_entity_backpack));
//	auto entityBackpackPanle = ui_name_backpack->panelUserWidget;
//
//
//	{
//		FString cmd = FString::Printf(TEXT("Event_SecondIInit"));
//		FOutputDeviceDebug device;
//		entityBackpackPanle->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
//	}
//
//	// ???????????????
//	for (auto it : biiMap[firstType])
//	{
//		auto secondType = it.Key;
//		auto info = it.Value;
//
//		FString cmd = FString::Printf(TEXT("Event_SetSecondItem %d %d %s %s"),
//			info.firstMenuType, info.secondMenuType
//			, *info.name, *info.iconPath);
//		FOutputDeviceDebug device;
//		entityBackpackPanle->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
//	}
//}
//
//// UI ???????????????
//void UPlayerEntityBackpackComponent::UI_SetThirdItem(int firstType, int secondType)
//{
//	auto view = Cast<ABaseCharacter>(GetOwner());
//	auto playerController = Cast<APlayerController>(view->Controller);
//	auto hud = Cast<ABaseHUD>(playerController->GetHUD());
//	auto ui_name_backpack = Cast<UEntityBackpackMainUserWidget>(hud->GetChildUI(EUIChildName::ui_name_entity_backpack));
//	auto entityBackpackPanle = ui_name_backpack->panelUserWidget;
//
//	{
//		FString cmd = FString::Printf(TEXT("Event_ThirdInit"));
//		FOutputDeviceDebug device;
//		entityBackpackPanle->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
//	}
//
//	// ???????????????
//	auto second = biiMap[firstType][secondType];
//	for (int i = 0; i < second.list.Num(); i++)
//	{
//		auto node = &second.list[i];
//		FString cmd = FString::Printf(TEXT("Event_SetThirdItem %d %d %d %d %d %s %s %s"),
//			firstType, secondType, node->listIndex,
//			node->row, node->column
//			, *node->name, *node->description, *node->iconPath);
//		FOutputDeviceDebug device;
//		entityBackpackPanle->CallFunctionByNameWithArguments(*cmd, device, NULL, true);
//	}
//}
//
///*------------------------------------------------------------------*/
//
//// UI?????? ??????UI?????????
//void UPlayerEntityBackpackComponent::UI_InitUI()
//{
//	UI_InitUI((int)xlsdata::FirstMenuType::FirstMenuType_BasePlace);
//}
//
//// UI?????? ??????????????????
//void UPlayerEntityBackpackComponent::UICallback_SelectFirstMenu(int firstType)
//{
//	UI_InitUI(firstType);
//}
//
//// UI?????? ??????????????????
//void UPlayerEntityBackpackComponent::UICallback_SelectSecondMenu(int firstType, int secondType)
//{
//	UI_SetThirdItem(firstType, secondType);
//}
//
//// UI?????? ??????????????????
//void UPlayerEntityBackpackComponent::UICallback_SelectItem(int firstType, int secondType, int listIndex)
//{
//	auto view = Cast<ABaseCharacter>(GetOwner());
//
//	auto& second = biiMap[firstType][secondType];
//	auto& item = second.list[listIndex];
//
//	// ??????
//	if (firstType == 0)
//	{
//		auto location = view->placeComponent->GetHitLocation();
//		FTransform t;
//		t.SetLocation(location);
//		t.SetRotation(FRotator::ZeroRotator.Quaternion());
//		t.SetScale3D(FVector(1, 1, 1));
//
//		view->placeComponent->SpawnMeshInstance(item.xlsId, false, t);
//	}
//	// ??????
//	else if (firstType == 1)
//	{
//		view->materialComponent->CallInput_SetMaterial(item.xlsId);
//	}
//	// ??????
//	else if (firstType == 2)
//	{
//		view->placeComponent->SpawnImageInstance(item.imageUrl);
//	}
//	// (ABaseCharacter*)view->get
//}
//
//// UI?????? ????????????
//void UPlayerEntityBackpackComponent::UICallback_RefreshPic(UObject* component, FString url)
//{
//	// ??????????????????????????????????????????
//	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
//
//	gameInstance_->onlineResourcesComponent->GameShowImage(component, url, 0, CallBackEventType::Event_UILoad);
//}
//
//// UI?????? ??????????????????
//void UPlayerEntityBackpackComponent::UICallback_CloseImage(UObject* component)
//{
//	// ??????????????????????????????????????????
//	UBaseGameInstance* gameInstance_ = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
//
//	gameInstance_->onlineResourcesComponent->GameCloseImage(component);
//}
//
//// UI?????? ????????????
//void UPlayerEntityBackpackComponent::UICallback_SavePic(FString url, FString name)
//{
//}

/*------------------------------------------------------------------*/