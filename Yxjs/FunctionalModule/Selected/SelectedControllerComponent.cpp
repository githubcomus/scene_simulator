// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectedControllerComponent.h"
#include "FunctionalModule/Selected/SelectedDataComponent.h"
#include "FunctionalModule/Selected/SelectedViewActor.h"
#include "Pawn/Character/BaseCharacter.h"
#include "GameMode/BaseGameMode.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"
#include "Core/BaseGameInstance.h"
#include "Core/SoundEffect/MySoundEffect.h"

#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Pawn/Character/BaseCharacter.h"

/*
controller
*/

/*------------------------------------------------------------------*/

// 获取本地玩家
ABaseCharacter* USelectedControllerComponent::GetLocalCharacter()
{
	//auto gm = GetWorld()->GetAuthGameMode();
	//if(gm)
	//{
	//    auto gm_ = Cast<ABaseGameMode>(gm);
	//    if(gm_)
	//    {
	//        auto selfCharacter = gm_->GetLocalPlayer();
	//        if(selfCharacter.IsValid())
	//        {
	//            return selfCharacter.Get();
	//        }
	//    }
	//}
	//return nullptr;
	return Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
}

// 获取当前工作模式
ESelectedWorkMode USelectedControllerComponent::GetWorkMode()
{
	return data->selectedWorkMode;

}

/*------------------------------------------------------------------*/

//
USelectedControllerComponent::USelectedControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// START
void USelectedControllerComponent::Start(USelectedDataComponent* data_, ASelectedViewActor* view_)
{
	data = data_;
	view = view_;

	data->changeAxisArrowColorType = ESelectedMoveStrategy::None;
	data->selectedWorkMode = ESelectedWorkMode::WorldLocation;
	view->SetWorkMode();
}

// END
void USelectedControllerComponent::End()
{
}

//
void USelectedControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto localCharacter = GetLocalCharacter();
	if (localCharacter == nullptr)return;

	float nowTime = data->GetWorld()->TimeSeconds;
	if (localCharacter->myData->IsEditEntityTransform() || localCharacter->myData->IsEditMoveTimeline())
	{

		if ((nowTime - cvTickInfo.lastTime) >= delayTime)
		{
			cvTickInfo.lastTime = nowTime;
			FrameSetScale();
		}
	}
}

// 帧函数
void USelectedControllerComponent::FrameSetScale()
{
	// 玩家移动 或者 actor移动
	auto localCharacter = GetLocalCharacter();
	if (localCharacter == nullptr)return;

	auto size = (localCharacter->GetActorLocation() - view->GetActorLocation()).Size();
	float standardDistance = 500;
	// 标准距离内1
	if (size <= standardDistance)
	{
		//if(data->scale !=1)
		//{
		//    data->scale = 1;
		//    view->SetActorScale3D(FVector(data->scale, data->scale, data->scale));
		//}
		//else
		//{
		//}
		auto multiple = 1 + ((size - standardDistance) / standardDistance);

		if (data->scale != multiple)
		{
			data->scale = multiple;
			view->SetActorScale3D(FVector(data->scale, data->scale, data->scale));
		}
	}
	// 大于标准距离
	else
	{
		auto multiple = 1 + ((size - standardDistance) / standardDistance) / 2;

		if (data->scale != multiple)
		{
			data->scale = multiple;
			view->SetActorScale3D(FVector(data->scale, data->scale, data->scale));
		}
	}
}

/*------------------------------------------------------------------*/

// 设置 还原所有坐标轴箭头颜色
void USelectedControllerComponent::RestoreAxisArrowColor(ESelectedMoveStrategy type)
{
	if (data->changeAxisArrowColorType == ESelectedMoveStrategy::None
		|| type == data->changeAxisArrowColorType)
	{
		return;
	}

	// 对旋转特殊处理
	if (data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorPitch
		|| data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorYaw
		|| data->changeAxisArrowColorType == ESelectedMoveStrategy::RotatorRoll)
	{
		view->SetShowSelectedMoveStrategyAll();
		data->changeAxisArrowColorType = ESelectedMoveStrategy::None;
		return;
	}

	// 原有流程
	view->RestoreAxisArrowColor();
	data->changeAxisArrowColorType = ESelectedMoveStrategy::None;


}

// 设置 改变坐标轴箭头颜色
void USelectedControllerComponent::ChangeAxisArrowColor(ESelectedMoveStrategy type)
{
	if (type == data->changeAxisArrowColorType)
	{
		return;
	}

	// 对旋转特殊处理
	if (type == ESelectedMoveStrategy::RotatorPitch
		|| type == ESelectedMoveStrategy::RotatorYaw
		|| type == ESelectedMoveStrategy::RotatorRoll)
	{

		view->SetShowSelectedMoveStrategySingle(type);
		data->changeAxisArrowColorType = type;
		return;
	}

	// 原有流程
	if (type == ESelectedMoveStrategy::LocationX
		|| type == ESelectedMoveStrategy::LocationY
		|| type == ESelectedMoveStrategy::LocationZ
		//|| type == ESelectedMoveStrategy::RotatorPitch
		//|| type == ESelectedMoveStrategy::RotatorYaw
		//|| type == ESelectedMoveStrategy::RotatorRoll
		|| type == ESelectedMoveStrategy::ScaleX
		|| type == ESelectedMoveStrategy::ScaleY
		|| type == ESelectedMoveStrategy::ScaleZ

		)
	{
		//if (auto gameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this))) {
		//	gameInstance->mySoundEffect->PlaySound(MySoundEffectType::MySoundEffectType_ui确认);
		//}
		data->changeAxisArrowColorType = type;
		view->ChangeAxisArrowColor();


	}
}

// 设置 工作模式
void USelectedControllerComponent::SetWorkMode(ESelectedWorkMode selectedWorkMode_)
{
	data->selectedWorkMode = selectedWorkMode_;

	view->SetWorkMode();

}

// 设置 只显示当前选中策略
void USelectedControllerComponent::SetShowSelectedMoveStrategySingle(ESelectedMoveStrategy selectedMoveStrategy_)
{
	// 对旋转特殊处理
	tempSelectedMoveStrategy = selectedMoveStrategy_;

	if (selectedMoveStrategy_ == ESelectedMoveStrategy::RotatorPitch
		|| selectedMoveStrategy_ == ESelectedMoveStrategy::RotatorYaw
		|| selectedMoveStrategy_ == ESelectedMoveStrategy::RotatorRoll)
	{
		return;
	}

	// 原有流程
	view->SetShowSelectedMoveStrategySingle(selectedMoveStrategy_);
}

// 设置 还原当前工作模式显示
void USelectedControllerComponent::SetShowSelectedMoveStrategyAll()
{
	// 对旋转特殊处理
	if (tempSelectedMoveStrategy == ESelectedMoveStrategy::RotatorPitch
		|| tempSelectedMoveStrategy == ESelectedMoveStrategy::RotatorYaw
		|| tempSelectedMoveStrategy == ESelectedMoveStrategy::RotatorRoll)
	{
		tempSelectedMoveStrategy = ESelectedMoveStrategy::None;
		return;
	}

	// 原有流程
	view->SetShowSelectedMoveStrategyAll();
	tempSelectedMoveStrategy = ESelectedMoveStrategy::None;
}

// 设置 将旋转轴切换到指定方向
void USelectedControllerComponent::SetScaleAxis(FTransform originWTransform)
{
	{
		view->myScaleX->SetWorldRotation(originWTransform.GetRotation());
	}
	{
		FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
			FRotator(0, 90, 0));
		view->myScaleY->SetWorldRotation(worldRotator);
	}
	{
		FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
			FRotator(90, 0, 0));
		view->myScaleZ->SetWorldRotation(worldRotator);
	}
}

// 设置 将旋转轴切换到指定方向
void USelectedControllerComponent::SetWorldLocation()
{
	view->myLocationX->SetWorldRotation(FRotator(0, 0, 0));
	view->myLocationY->SetWorldRotation(FRotator(0, 90, 0));
	view->myLocationZ->SetWorldRotation(FRotator(90, 0, 0));
	view->myRotatorPitch->SetWorldRotation(FRotator(0, 0, 90));
	view->myRotatorYaw->SetWorldRotation(FRotator(0, 0, 0));
	view->myRotatorRoll->SetWorldRotation(FRotator(90, 0, 0));
}

// 设置 将旋转轴切换到指定方向
void USelectedControllerComponent::SetRelativelyLocation(FTransform originWTransform)
{
	{
		view->myLocationX->SetWorldRotation(originWTransform.GetRotation());
	}
	{
		FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
			FRotator(0, 90, 0));
		view->myLocationY->SetWorldRotation(worldRotator);
	}
	{
		FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
			FRotator(90, 0, 0));
		view->myLocationZ->SetWorldRotation(worldRotator);
	}

	{
		FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
			FRotator(0, 0, 90));
		view->myRotatorPitch->SetWorldRotation(worldRotator);
	}
	{
		FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
			FRotator(0, 0, 0));
		view->myRotatorYaw->SetWorldRotation(worldRotator);
	}
	{
		FRotator worldRotator = UKismetMathLibrary::TransformRotation(originWTransform,
			FRotator(90, 0, 0));
		view->myRotatorRoll->SetWorldRotation(worldRotator);
	}
}

// 设置 选中与否
void USelectedControllerComponent::SetIsSelected(bool flag)
{
	if (flag)
	{
		data->isSelected = true;
		view->SetActorHiddenInGame(false);
	}
	else
	{
		data->isSelected = false;
		view->SetActorHiddenInGame(true);
		view->SetActorLocation(FVector(0, 0, 0));
	}
}

// 设置 当前为相对旋转状态则将ui调整
void USelectedControllerComponent::SetRelativelyRotation(FTransform worldTransform)
{
	if (ESelectedWorkMode::RelativelyRotator == data->selectedWorkMode
		|| ESelectedWorkMode::RelativelyLocation == data->selectedWorkMode)
	{
		SetRelativelyLocation(worldTransform);
	}
}

/*------------------------------------------------------------------*/