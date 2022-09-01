// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorTrajectoryView.h"
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
#include "PawnLineActor.h"

// 功能模块
// view
/*
 *
 */

 /*------------------------------------------------------------------*/

 //
AActorTrajectoryView::AActorTrajectoryView()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;
}

//
void AActorTrajectoryView::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void AActorTrajectoryView::BeginPlay()
{
	Super::BeginPlay();
	Start();
}

//
void AActorTrajectoryView::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	End();
	Super::EndPlay(EndPlayReason);
}

// 
void AActorTrajectoryView::Start()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//baseGameMode->ActorTrajectoryView = this;

}

// 
void AActorTrajectoryView::End()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//baseGameMode->ActorTrajectoryView = nullptr;
}

/*------------------------------------------------------------------*/

// tick 
void AActorTrajectoryView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//if (createCount == 0 && destroyCount == 0)
	//{
	//	return;
	//}

	//int count = 0;
	//for (auto it = componentMap.CreateConstIterator(); it; ++it)
	//{
	//	FString key = it.Key();
	//	FSceneComponentInfo& node = componentMap[key];
	//	if (node.isLoadRes != ESceneResLoadType::ResLoadType_None)
	//	{
	//		if (createCount > 0 && node.createInstanceList.size() > 0)
	//		{
	//			Tick_CreateInstance(count, node);
	//		}
	//		if (destroyCount > 0 && node.destroyInstanceList.size() > 0)
	//		{
	//			Tick_DestroyInstance(count, node);
	//		}
	//	}
	//}
}

/*------------------------------------------------------------------*/

// 玩家创建
void AActorTrajectoryView::OnPlayerCreate(int playerId)
{

	return;

	if (lineActorMap.Find(playerId))
	{
		lineActorMap[playerId]->Destroy();
		lineActorMap.Remove(playerId);
	}

	FTransform t;
	t.SetLocation(FVector::ZeroVector);
	t.SetRotation(FRotator::ZeroRotator.Quaternion());
	auto actor = Cast<APawnLineActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), APawnLineActor::StaticClass(), t, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (actor)
	{
		actor->Start(playerId);
		UGameplayStatics::FinishSpawningActor(actor, t);
	}
	lineActorMap.Add(playerId, actor);
}

// 玩家销毁
void AActorTrajectoryView::OnPlayerDestroy(int playerId)
{

	return;
	if (lineActorMap.Find(playerId))
	{
		lineActorMap[playerId]->End();
		lineActorMap[playerId]->Destroy();
		lineActorMap.Remove(playerId);
	}

}

// 刷新节点
void AActorTrajectoryView::OnUpdatePoint(int playerId, std::vector<FVector>& pointList)
{

	if (lineActorMap.Find(playerId))
	{
		auto& actor = lineActorMap[playerId];
		actor->OnUpdatePoint(pointList);
	}
}

// 刷新节点
void AActorTrajectoryView::OnUpdatePoint(int playerId, FVector point)
{

	if (lineActorMap.Find(playerId))
	{
		auto& actor = lineActorMap[playerId];
		actor->OnUpdatePoint(point);
	}
}

/*------------------------------------------------------------------*/
