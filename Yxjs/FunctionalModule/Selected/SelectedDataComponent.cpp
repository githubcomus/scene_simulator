// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectedDataComponent.h"
#include "FunctionalModule/Selected/SelectedDataComponent.h"
#include "FunctionalModule/Selected/SelectedControllerComponent.h"
#include "Core/BaseGameInstance.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"

#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Camera/CameraComponent.h"
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
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

/*
data
*/

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

// USelectedDataComponent
USelectedDataComponent::USelectedDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	scale = 1;
	isSelected = false;
}

// BeginPlay
void USelectedDataComponent::Start(USelectedControllerComponent* controller_, ASelectedViewActor* view_)
{
	controller = controller_;
	view = view_;
}

// EndPlay
void USelectedDataComponent::End()
{
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/