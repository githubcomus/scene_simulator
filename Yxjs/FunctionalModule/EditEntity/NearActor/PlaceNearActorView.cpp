// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceNearActorView.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
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
#include "Engine/AssetManager.h"
#include "../EditEntityManage.h"
//#include "Yxjs/Pawn/Racing/Player/PlayerRacingPawn.h"
#include "PlaceNearDataComponent.h"
#include "PlaceNearControllerComponent.h"

// 功能模块
// view
/*
 *
 */

 /*----------------------------------------------------*/
 /*----------------------------------------------------*/

//
APlaceNearActorView::APlaceNearActorView()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;

	data = CreateDefaultSubobject<UPlaceNearDataComponent>(TEXT("UPlaceNearDataComponent"));
	controller = CreateDefaultSubobject<UPlaceNearControllerComponent>(TEXT("UPlaceNearControllerComponent"));
}

//
void APlaceNearActorView::BeginPlay()
{
	Super::BeginPlay();
}

//
void APlaceNearActorView::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

//
void APlaceNearActorView::Start(AEditEntityManage* editEntityManage_)
{
	editEntityManage = editEntityManage_;
	data->Start(controller, this);
	controller->Start(data, this);
}

//
void APlaceNearActorView::End()
{
	data->End();
	controller->End();
}

// tick
void APlaceNearActorView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/
