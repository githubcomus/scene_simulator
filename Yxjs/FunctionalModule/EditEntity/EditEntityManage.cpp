// Fill out your copyright notice in the Description page of Project Settings.

#include "EditEntityManage.h"
#include "FunctionalModule/EditEntity/EditEntityManageData.h"
#include "FunctionalModule/EditEntity/EditEntityManageController.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewMeshComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewImageComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewCollisionComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewLaserLightActor.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceViewLaserBeamComponent.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "Tools/CommonlyObject.h"
#include "NearActor/PlaceNearActorView.h"
#include "Actor/EditEntityActor.h"
#include "FunctionalModule/EditEntity/Timeline/EditEntityManageTimelineComponent.h"
#include "FunctionalModule/EditEntity/TempComponent/PlaceStaticMeshComponent.h"

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
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
#include "Actor/ControllerComponent/EditEntityChildComponent.h"
#include "MyPython/MyPythonActor.h"
#include "Command/EditEntityManageCommandComponent.h"
#include "ScanMesh/ScanMesh.h"
#include "ResourceIcon/EditEntityResourceIcon.h"

/*
view
*/

/*----------------------------------------------------*/
/*----------------------------------------------------*/

//
AEditEntityManage::AEditEntityManage()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;

	data = CreateDefaultSubobject<UEditEntityManageData>(TEXT("UEditEntityManageData"));

	controller = CreateDefaultSubobject<UEditEntityManageController>(TEXT("UEditEntityManageController"));

	timeline = CreateDefaultSubobject<UEditEntityManageTimelineComponent>(TEXT("UEditEntityManageTimelineComponent"));

	command = CreateDefaultSubobject<UEditEntityManageCommandComponent>(TEXT("UEditEntityManageCommandComponent"));

}

//
void AEditEntityManage::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void AEditEntityManage::BeginPlay()
{
	Super::BeginPlay();
	Start();
}

// START
void AEditEntityManage::Start()

{
	UE_LOG(LogTemp, Log, TEXT("[%x] [AEditEntityManage::Start] "), this);

	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (baseGameMode == nullptr)return;

	data->Start(controller, this);
	controller->Start(data, this);
	timeline->Start(this);
	command->Start(this);
	scanMesh = NewObject<UScanMesh>(this);
	scanMesh->Start(this);

	resourceIcon = NewObject<UEditEntityResourceIcon>(this);
	resourceIcon->Start(this);

	{
		FTransform t;
		t.SetLocation(FVector::ZeroVector);
		t.SetRotation(FRotator::ZeroRotator.Quaternion());
		placeNearActorView = Cast<APlaceNearActorView>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), APlaceNearActorView::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (placeNearActorView)
		{
			UGameplayStatics::FinishSpawningActor(placeNearActorView, t);
			placeNearActorView->Start(this);
		}
	}
	{
		FTransform t;
		t.SetLocation(FVector::ZeroVector);
		t.SetRotation(FRotator::ZeroRotator.Quaternion());
		myPythonActor = Cast<AMyPythonActor>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AMyPythonActor::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (myPythonActor)
		{
			UGameplayStatics::FinishSpawningActor(myPythonActor, t);
			myPythonActor->Start(this);
		}
	}
}

//
void AEditEntityManage::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	End();
}

// END
void AEditEntityManage::End()
{
	resourceIcon->End();
	resourceIcon = nullptr;
	scanMesh->End();
	scanMesh = nullptr;
	timeline->End();
	data->End();
	controller->End();
	command->End();

	if (placeNearActorView)
	{
		placeNearActorView->End();
		placeNearActorView->Destroy();
		placeNearActorView = nullptr;
	}

	if (myPythonActor)
	{
		myPythonActor->End();
		myPythonActor->Destroy();
		myPythonActor = nullptr;
	}
	UE_LOG(LogTemp, Log, TEXT("[%x] [AEditEntityManage::End]   "), this);
}

// tick
void AEditEntityManage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (controller)
	{
	}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/

// 外部调用 新增数据
void AEditEntityManage::EntityAdd(Gamedata::EntityInfo* newSyncInfo)
{
	if (actorMap.Find(newSyncInfo->entityid()))
	{
		return;
	}

	auto newT = UCommonlyObject::PBToUe4Transform(newSyncInfo->mutable_transform());
	newT.SetScale3D(FVector(1, 1, 1));
	newT.SetRotation(FRotator::ZeroRotator.Quaternion());
	auto actor = Cast<AEditEntityActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AEditEntityActor::StaticClass(), newT, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (actor)
	{
		actorMap.Add(newSyncInfo->entityid(), actor);
		actor->SetOwner(this);
		UGameplayStatics::FinishSpawningActor(actor, newT);
		actor->Start(newSyncInfo);
		// 只有编辑器执行
#if WITH_EDITORONLY_DATA
		FString name = FString::Printf(TEXT("EditEntity_%s"), *actor->GetMyName());
		actor->SetActorLabel(name);
#endif
	}
}

// 外部调用 移除数据
void AEditEntityManage::EntityRemove(int32 entityId)
{
	if (actorMap.Find(entityId))
	{
		actorMap[entityId]->End();
		actorMap[entityId]->Destroy();
		actorMap.Remove(entityId);
	}
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/