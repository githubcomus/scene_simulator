// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnLineActor.h"
#include "Core/BaseGameInstance.h"
#include "GameMode/BaseGameMode.h"
#include "Yxjs/Pawn/Character/BaseCharacter.h"
#include "Pawn/Character/CharacterComponent/CharacterDataComponent.h"

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
#include <Components/SplineComponent.h>

// 功能模块
// view
/*
 *
 */

 /*------------------------------------------------------------------*/

 //
APawnLineActor::APawnLineActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;


	splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("USplineComponent"));
	splineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	splineComponent->SetHiddenInGame(false);
	splineComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	splineComponent->SetRelativeLocation(FVector::ZeroVector);
}

//
void APawnLineActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void APawnLineActor::BeginPlay()
{
	Super::BeginPlay();
}

//
void APawnLineActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// 
void APawnLineActor::Start(int playerId)
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	character = baseGameMode->GetPlayer(playerId);
}

// 
void APawnLineActor::End()
{
	ABaseGameMode* baseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//baseGameMode->PawnLineActor = nullptr;
}

/*------------------------------------------------------------------*/

// tick 
void APawnLineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	auto nowTime = GetWorld()->TimeSeconds;

	// 速度由当前到最新点的距离+
	if (playerPointIndex < msgPointIndex)
	{
		auto characterLocation = character->GetActorLocation();
		if (playerPointInfo == nullptr)
		{
			playerPointInfo = &moveList[0];
		}

		auto flag = playerPointInfo->point.Equals(characterLocation, 1);

		// 到达当前点
		if (flag)
		{
			playerPointIndex++;
			// 搜寻下一个点
			if (playerPointIndex < msgPointIndex)
			{
				for (auto i = 0; i < moveList.size(); i++)
				{
					auto& pointInfo = moveList[i];
					if (pointInfo.index == playerPointIndex)
					{
						playerPointInfo = &pointInfo;
						playerPointInfo->startTime = nowTime;
						break;
					}
				}
			}
			// 到达终点
			else
			{
				playerPointInfo = nullptr;
			}
		}
		// 追逐当前点
		else
		{
			auto tempTime = nowTime - playerPointInfo->startTime;
			float speed = 200;
			auto tempLocation = FMath::VInterpConstantTo(
				character->GetActorLocation(),
				playerPointInfo->point,
				tempTime,
				speed);
			character->SetActorLocation(tempLocation);
		}

	}


}

/*------------------------------------------------------------------*/

// 刷新节点
void APawnLineActor::OnUpdatePoint(FVector point)
{

	lastTimeUpdate = GetWorld()->TimeSeconds;

	auto moveSize = (point - character->GetActorLocation()).Size();
	// || character->dataComp->IsEditEntityTransform()
	if (moveSize > 2000)
	{

		FRotator rotator2 = (point - character->GetActorLocation()).Rotation();
		character->SetActorLocationAndRotation(point, rotator2);
		moveList.clear();
		lineType = ELineType_None;
		return;
	}

	lineType = ELineType_Update;

	SPointInfo pointInfo;
	pointInfo.point = point;
	pointInfo.index = msgPointIndex;
	msgPointIndex++;

	moveList.push_back(pointInfo);
	if (moveList.size() > 10)
	{
		//moveList.pop_front();
		auto it = moveList.begin();
		moveList.erase(it);
	}

	while (splineComponent->GetNumberOfSplinePoints() > 0)
	{
		splineComponent->RemoveSplinePoint(0, true);
	}

	for (auto it = moveList.begin(); it != moveList.end(); it++)
	{

		CreateSplinePoint(it->point);

		if (it != moveList.begin())
		{
			auto start = (it--)->point;
			auto end = it->point;
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FLinearColor::Red, 10, 3);
			it++;
		}

	}
}

// 样条线网格组件 创建节点
void APawnLineActor::CreateSplinePoint(FVector location)
{
	auto pointIndex = splineComponent->GetNumberOfSplinePoints();
	FSplinePoint p(pointIndex, location);
	splineComponent->AddPoint(p, true);
}

/*------------------------------------------------------------------*/

// 其他 刷新节点
void APawnLineActor::OnUpdatePoint(std::vector<FVector>& pointList)
{

	while (splineComponent->GetNumberOfSplinePoints() > 0)
	{
		splineComponent->RemoveSplinePoint(0, true);
	}

	for (int i = 0; i < pointList.size(); i++)
	{
		CreateSplinePoint(pointList[i]);

		if (i > 0)
		{
			auto start = pointList[i - 1];
			auto end = pointList[i];
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FLinearColor::Red, 10, 3);
		}

	}
}

/*------------------------------------------------------------------*/
