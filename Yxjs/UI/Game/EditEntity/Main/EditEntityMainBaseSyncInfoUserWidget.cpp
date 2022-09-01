// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityMainBaseSyncInfoUserWidget.h"
#include "Pawn/Character/CharacterComponent/PlayerSelectedComponent.h"
#include "Pawn/Character/BaseCharacter.h"

#include "Kismet/KismetInputLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/CommonlyObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Engine/Texture2D.h>
#include <Engine/AssetManager.h>
#include "../../../../GameMode/BaseGameMode.h"
#include "../../../../FunctionalModule/EditEntity/EditEntityManage.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManageData.h"
#include "Components/EditableText.h"
#include "Yxjs/Tools/CalculationObject.h"
#include "Yxjs/FunctionalModule/EditEntity/Actor/EditEntityActor.h"
#include "Yxjs/FunctionalModule/EditEntity/EditEntityManageController.h"
#include "Yxjs/FunctionalModule/Selected/SelectedViewActor.h"

/*/////////////////////////////////////////////////////////////*/
// data
/*/////////////////////////////////////////////////////////////*/

// 获取当前选中的actor
TWeakObjectPtr<AEditEntityActor> UEditEntityMainBaseSyncInfoUserWidget::GetSelectEntityActor()
{

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto character = Cast<ABaseCharacter>(player->GetPawn());
	if (character == nullptr)return nullptr;

	auto editEntityManageData = baseGameMode->editEntityManage->data;

	auto entityList = &character->selectComponent->cvMultipleInstanceInfo.selectedList;
	auto& entityList_ = *entityList;
	if (entityList_.Num() == 0)return nullptr;
	int masterEntityId = entityList_[entityList_.Num() - 1].entityId;

	auto actor = editEntityManageData->GetEntityActor(masterEntityId);
	return actor;
}

/*/////////////////////////////////////////////////////////////*/
// view
/*/////////////////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////////////////*/
// controller
/*/////////////////////////////////////////////////////////////*/

//
void UEditEntityMainBaseSyncInfoUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

//
void UEditEntityMainBaseSyncInfoUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

//
void UEditEntityMainBaseSyncInfoUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

//
FReply UEditEntityMainBaseSyncInfoUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//
FReply UEditEntityMainBaseSyncInfoUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//
void UEditEntityMainBaseSyncInfoUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

//
void UEditEntityMainBaseSyncInfoUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

//
void UEditEntityMainBaseSyncInfoUserWidget::Init()
{
	if (isInit)
	{
		return;
	}
	isInit = true;

	EditableText_locationX = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_locationX")));
	EditableText_locationY = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_locationY")));
	EditableText_locationZ = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_locationZ")));
	EditableText_rotationX = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_rotationX")));
	EditableText_rotationY = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_rotationY")));
	EditableText_rotationZ = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_rotationZ")));
	EditableText_scaleX = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_scaleX")));
	EditableText_scaleY = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_scaleY")));
	EditableText_scaleZ = Cast<UEditableText>(GetWidgetFromName(TEXT("EditableText_scaleZ")));

	if (
		EditableText_locationX.IsValid() == false ||
		EditableText_locationY.IsValid() == false ||
		EditableText_locationZ.IsValid() == false ||
		EditableText_rotationX.IsValid() == false ||
		EditableText_rotationY.IsValid() == false ||
		EditableText_rotationZ.IsValid() == false ||
		EditableText_scaleX.IsValid() == false ||
		EditableText_scaleY.IsValid() == false ||
		EditableText_scaleZ.IsValid() == false

		)
	{
		return;
	}

	FScriptDelegate func;
	func.BindUFunction(this, TEXT("MyTextCommitted"));
	EditableText_locationX->OnTextCommitted.AddUnique(func);
	EditableText_locationY->OnTextCommitted.AddUnique(func);
	EditableText_locationZ->OnTextCommitted.AddUnique(func);
	EditableText_rotationX->OnTextCommitted.AddUnique(func);
	EditableText_rotationY->OnTextCommitted.AddUnique(func);
	EditableText_rotationZ->OnTextCommitted.AddUnique(func);
	EditableText_scaleX->OnTextCommitted.AddUnique(func);
	EditableText_scaleY->OnTextCommitted.AddUnique(func);
	EditableText_scaleZ->OnTextCommitted.AddUnique(func);

}

// 提交的时候保存数据
void UEditEntityMainBaseSyncInfoUserWidget::MyTextCommitted(const FText& Text, ETextCommit::Type  CommitMethod)
{

	SaveData();

}

// 保存数据
void UEditEntityMainBaseSyncInfoUserWidget::SaveData()
{

	auto character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (character == nullptr)return;
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto selectedView = baseGameMode->selectedViewActor;
	UEditEntityManageController* placeController = baseGameMode->editEntityManage->controller;

	//1. 找到选中实例
	TWeakObjectPtr<AEditEntityActor> actor = GetSelectEntityActor();
	if (actor.IsValid() == false)
	{
		return;
	}

	//2. 获取面板数据
	std::string location_x(TCHAR_TO_UTF8(*EditableText_locationX->GetText().ToString()));
	std::string location_y(TCHAR_TO_UTF8(*EditableText_locationY->GetText().ToString()));
	std::string location_z(TCHAR_TO_UTF8(*EditableText_locationZ->GetText().ToString()));
	std::string pitch(TCHAR_TO_UTF8(*EditableText_rotationX->GetText().ToString()));
	std::string yaw(TCHAR_TO_UTF8(*EditableText_rotationY->GetText().ToString()));
	std::string roll(TCHAR_TO_UTF8(*EditableText_rotationZ->GetText().ToString()));
	std::string scale_x(TCHAR_TO_UTF8(*EditableText_scaleX->GetText().ToString()));
	std::string scale_y(TCHAR_TO_UTF8(*EditableText_scaleY->GetText().ToString()));
	std::string scale_z(TCHAR_TO_UTF8(*EditableText_scaleZ->GetText().ToString()));

	//3. 检测输入非法字符串
	{
		bool flag1 = UCalculationObject::StrIsNumber(location_x);
		bool flag2 = UCalculationObject::StrIsNumber(location_y);
		bool flag3 = UCalculationObject::StrIsNumber(location_z);
		bool flag4 = UCalculationObject::StrIsNumber(pitch);
		bool flag5 = UCalculationObject::StrIsNumber(yaw);
		bool flag6 = UCalculationObject::StrIsNumber(roll);
		bool flag7 = UCalculationObject::StrIsNumber(scale_x);
		bool flag8 = UCalculationObject::StrIsNumber(scale_y);
		bool flag9 = UCalculationObject::StrIsNumber(scale_z);
		if (
			flag1 == false ||
			flag2 == false ||
			flag3 == false ||
			flag4 == false ||
			flag5 == false ||
			flag6 == false ||
			flag7 == false ||
			flag8 == false ||
			flag9 == false
			)
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [编辑实例] [输入的字符为非法数字] [变换面板] "), this);

			SetData(oldTransform);

			return;
		}
	}

	//4. 转换输入值
	FTransform transform;
	transform.SetLocation(FVector(atof(location_x.c_str()), atof(location_y.c_str()), atof(location_z.c_str())));
	transform.SetRotation(FRotator(atof(pitch.c_str()), atof(yaw.c_str()), atof(roll.c_str())).Quaternion());
	transform.SetScale3D(FVector(atof(scale_x.c_str()), atof(scale_y.c_str()), atof(scale_z.c_str())));


	//5. 检测输入值最大范围
	{
		if (
			(-10000 * 100 < transform.GetLocation().X && transform.GetLocation().X < 10000 * 100)
			&& (-10000 * 100 < transform.GetLocation().Y && transform.GetLocation().Y < 10000 * 100)
			&& (-10000 * 100 < transform.GetLocation().Z && transform.GetLocation().Z < 10000 * 100)
			)
		{
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [编辑实例] [坐标超出最大范围] [变换面板] "), this);
			SetData(oldTransform);
			return;
		}

		auto rotator = transform.GetRotation().Rotator();
		if (
			(-360 <= atof(pitch.c_str()) && atof(pitch.c_str()) <= 360)
			&& (-360 <= atof(yaw.c_str()) && atof(yaw.c_str()) <= 360)
			&& (-360 <= atof(roll.c_str()) && atof(roll.c_str()) <= 360)
			)
		{
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [编辑实例] [旋转超出最大范围] [变换面板] "), this);
			SetData(oldTransform);
			return;
		}

		auto scale = transform.GetScale3D();
		if (
			(0.001 <= scale.X && scale.X <= 1500)
			&& (0.001 <= scale.Y && scale.Y <= 1500)
			&& (0.001 <= scale.Z && scale.Z <= 500)
			)
		{
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [编辑实例] [缩放超出最大范围] [变换面板] "), this);
			SetData(oldTransform);
			return;
		}
	}

	//6. 检测输入值距离玩家距离
	{
		auto location = character->GetActorLocation();
		auto len = (location - transform.GetLocation()).Size();
		if (len > 2000 * 100)
		{
			UE_LOG(LogTemp, Log, TEXT("[%x] [编辑实例] [与玩家距离超出最大范围] [变换面板] "), this);
			SetData(oldTransform);
			return;
		}
	}

	//7. 保存数据
	{

		if (oldTransform.Equals(transform) == false)
		{
			oldTransform = transform;

			// 选中实例
			auto syncInfo = &actor->syncInfo;
			UCommonlyObject::Ue4ToPbTransform(transform, syncInfo->mutable_transform());
			TArray<int32> entityIdList;
			entityIdList.Add(actor->entityId);
			placeController->NetCall_UpdateData(entityIdList);

			// 坐标轴
			selectedView->SetActorLocation(transform.GetLocation());
		}

	}

}

// 设置数据
void UEditEntityMainBaseSyncInfoUserWidget::SetData(FTransform transform)
{

	EditableText_locationX->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetLocation().X)));
	EditableText_locationY->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetLocation().Y)));
	EditableText_locationZ->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetLocation().Z)));

	EditableText_rotationX->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetRotation().Rotator().Pitch)));
	EditableText_rotationY->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetRotation().Rotator().Yaw)));
	EditableText_rotationZ->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetRotation().Rotator().Roll)));

	EditableText_scaleX->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetScale3D().X)));
	EditableText_scaleY->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetScale3D().Y)));
	EditableText_scaleZ->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), transform.GetScale3D().Z)));

	oldTransform = transform;
}