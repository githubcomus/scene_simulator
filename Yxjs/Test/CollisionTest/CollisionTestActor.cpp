// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionTestActor.h"

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
#include "Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


/*
view
*/


/*------------------------------------------------------------------*/

//
ACollisionTestActor::ACollisionTestActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	sceneRoot->SetShouldUpdatePhysicsVolume(true);
	sceneRoot->SetCanEverAffectNavigation(false);
	RootComponent = sceneRoot;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComponent"));
	boxComponent->SetMobility(EComponentMobility::Movable);
	boxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	boxComponent->SetHiddenInGame(false);
	boxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxComponent->SetBoxExtent(FVector(100, 200, 300));
}

//
void ACollisionTestActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//
void ACollisionTestActor::BeginPlay()
{
	Super::BeginPlay();
	Start();
}

//
void ACollisionTestActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	End();
}

// tick
void ACollisionTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	TickTest();
}

/*------------------------------------------------------------------*/

// START
void ACollisionTestActor::Start()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ACollisionTestActor::Start] "), this);
}

// END
void ACollisionTestActor::End()
{
	UE_LOG(LogTemp, Log, TEXT("[%x] [ACollisionTestActor::End]   "), this);
}

//
void ACollisionTestActor::TickTest()
{
	auto nowTime = GetWorld()->TimeSeconds;
	if (nowTime < lastTimeTickTest + 0.2)
	{
		return;
	}
	lastTimeTickTest = nowTime;


	auto character = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if (character == nullptr)return;

	FLinearColor color = FLinearColor::Blue;
	float cylinderHalfHeight = 10;
	auto r = 100;


	// 圆柱与长方体重叠
	{
		// 平面测试
		// flag = CircleAndRectangleOverlap(FVector2D(GetActorLocation().X, GetActorLocation().Y),
		//                                  FVector2D(100, 200),
		//                                  FVector2D(character->GetActorLocation().X, character->GetActorLocation().Y),
		//                                  r);

		bool flag = CylinderAndCuboidOverlap(character->GetActorLocation(), cylinderHalfHeight, r, GetActorLocation(),
			boxComponent->GetScaledBoxExtent());

		if (flag)
		{
			color = FLinearColor::Red;
		}
	}

	// 圆柱与圆柱重叠
	{
		bool flag = false;
		float cylinderHalfHeight2 = 100;
		auto r2 = 500;

		auto newCenter = GetActorLocation() + FVector::LeftVector * 1500;
		auto start = newCenter + FVector::UpVector * cylinderHalfHeight2;
		auto end = newCenter + FVector::DownVector * cylinderHalfHeight2;
		UKismetSystemLibrary::DrawDebugCylinder(GetWorld(), start, end, r2, 12, FLinearColor::Black, 1, 1);


		flag = CylinderAndCylinderOverlap(newCenter, cylinderHalfHeight2, r2,
			character->GetActorLocation(), cylinderHalfHeight, r);
		if (flag)
		{
			color = FLinearColor::Yellow;
		}
	}


	auto start = character->GetActorLocation() + FVector::UpVector * cylinderHalfHeight;
	auto end = character->GetActorLocation() + FVector::DownVector * cylinderHalfHeight;
	UKismetSystemLibrary::DrawDebugCylinder(GetWorld(), start, end, r, 12, color, 0.2, 1);
}

/*------------------------------------------------------------------*/

// 重叠 以角色a为原点,重新计算角色b坐标
void ACollisionTestActor::UseActorAConter(FTransform actorA, FTransform& actorB)
{
}

// 重叠 圆柱与长方体重叠(长方体/圆柱 0,0,0旋转)
bool ACollisionTestActor::CylinderAndCuboidOverlap(FVector cylinderConter, float cylinderHalfHeight, float circleRadius,
	FVector cuboidConter, FVector cuboidExtion)
{
	// 圆柱中心点,圆柱半高,圆柱半径,长方体中心点,长方体伸展

	bool flag = false;

	// z轴 线段重叠
	{
		float start1 = cylinderConter.Z + cylinderHalfHeight;
		float end1 = cylinderConter.Z - cylinderHalfHeight;
		float start2 = cuboidConter.Z + cuboidExtion.Z;
		float end2 = cuboidConter.Z - cuboidExtion.Z;
		flag = LineSegmentOverlaps(start1, end1, start2, end2);
	}

	if (flag == false)return false;

	// xy轴 面重叠
	{
		flag = CircleAndRectangleOverlap(FVector2D(cuboidConter.X, cuboidConter.Y),
			FVector2D(cuboidExtion.X, cuboidExtion.Y),
			FVector2D(cylinderConter.X, cylinderConter.Y),
			circleRadius);
	}

	return flag;
}

// 重叠 1D线段与线段重叠(单轴)
bool ACollisionTestActor::LineSegmentOverlaps(float start1, float end1, float start2, float end2)
{
	if (start1 > start2 && start2 > end1)
	{
		return true;
	}
	else if (start1 > end2 && end2 > end1)
	{
		return true;
	}
	else if (start2 > start1 && start1 > end2)
	{
		return true;
	}
	else if (start2 > end1 && end1 > end2)
	{
		return true;
	}
	else if (start1 == start2 && end1 == end2)
	{
		return true;
	}

	return false;
}

// 重叠 2D圆与矩形重叠(矩形处于坐标系0,0,0旋转)
bool ACollisionTestActor::CircleAndRectangleOverlap(FVector2D RectangleCenter, FVector2D RectangleRadius,
	FVector2D circleCenter, float circleRadius)
{
	// 矩形中心 矩形半径 圆中心 圆半径
	auto v = (circleCenter - RectangleCenter).GetAbs();

	auto u = (v - RectangleRadius);
	if (u.X < 0)u.X = 0;
	if (u.Y < 0)u.Y = 0;
	if (u.Size() < circleRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 重叠 圆柱与圆柱重叠
bool ACollisionTestActor::CylinderAndCylinderOverlap(FVector cylinderConter1, float cylinderHalfHeight1,
	float circleRadius1, FVector cylinderConter2,
	float cylinderHalfHeight2, float circleRadius2)
{
	// 圆柱中心点,圆柱半高,圆柱半径,圆柱中心点,圆柱半高,圆柱半径

	bool flag = false;

	// z轴 线段重叠
	{
		float start1 = cylinderConter1.Z + cylinderHalfHeight1;
		float end1 = cylinderConter1.Z - cylinderHalfHeight1;
		float start2 = cylinderConter2.Z + cylinderHalfHeight2;
		float end2 = cylinderConter2.Z - cylinderHalfHeight2;
		flag = LineSegmentOverlaps(start1, end1, start2, end2);
	}

	if (flag == false)return false;

	// xy轴 面重叠
	{
		flag = CircleAndCircleOverlap(
			FVector2D(cylinderConter1.X, cylinderConter1.Y), circleRadius1,
			FVector2D(cylinderConter2.X, cylinderConter2.Y), circleRadius2);
	}

	return flag;
}

// 重叠 2D圆与圆重叠
bool ACollisionTestActor::CircleAndCircleOverlap(FVector2D circleCenter1, float circleRadius1,
	FVector2D circleCenter2, float circleRadius2)
{
	// 圆中心 圆半径 圆中心 圆半径

	auto len = (circleCenter1 - circleCenter2).Size();
	if ((circleRadius1 + circleRadius2) > len)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------*/
